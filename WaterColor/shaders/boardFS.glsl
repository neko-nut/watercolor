#version 330

in vec2 TexCoord;

uniform sampler2D objectTexture;
uniform sampler2D edgeTexture;

uniform int Width = 800; // The pixel width
uniform int Height = 600; // The pixel height

uniform float sigma = 20;
const float pi = 3.14159265f;
const float numBlurPixelsPerSide = 10.0f;
const vec2 blurMultiplyVecV = vec2(0.0f, 1.0f);
const vec2 blurMultiplyVecH = vec2(1.0f, 0.0f);
float blurSize = 1.0 / float(Height);
void main()
{
    //gl_FragColor = texture(edgeTexture, TexCoord);
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);


    float dx = 1.0 / float(Width);
    float dy = 1.0 / float(Height);


    // Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
    vec3 incrementalGaussian;
    incrementalGaussian.x = 1.0f / (sqrt(2.0f * pi) * sigma);
    incrementalGaussian.y = exp(-0.5f / (sigma * sigma));
    incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;

    vec4 avgValue = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    float coefficientSum = 0.0f;

    // Take the central sample first...
    avgValue += texture2D(edgeTexture, TexCoord.st) * incrementalGaussian.x;
    coefficientSum += incrementalGaussian.x;
    incrementalGaussian.xy *= incrementalGaussian.yz;

    // Go through the remaining 8 vertical samples (4 on each side of the center)
    for (float i = 1.0f; i <= numBlurPixelsPerSide; i++) { 
        avgValue += texture2D(edgeTexture, TexCoord.st - i * dy * blurMultiplyVecV) * incrementalGaussian.x;         
        avgValue += texture2D(edgeTexture, TexCoord.st + i * dy * blurMultiplyVecV) * incrementalGaussian.x;
        avgValue += texture2D(edgeTexture, TexCoord.st - i * dx * blurMultiplyVecH) * incrementalGaussian.x;         
        avgValue += texture2D(edgeTexture, TexCoord.st + i * dx * blurMultiplyVecH) * incrementalGaussian.x;            
        coefficientSum += 4 * incrementalGaussian.x;
        incrementalGaussian.xy *= incrementalGaussian.yz;
    }

    

    gl_FragColor = avgValue / coefficientSum;
    //gl_FragColor = avgValue;

    //gl_FragColor = normalize(abs(avgValue));
    //avgValue = avgValue / (avgValue + vec4(1.0));
    //gl_FragColor = avgValue / (avgValue + vec4(1.0));
    //gl_FragColor = texture(objectTexture, TexCoord);



    
}