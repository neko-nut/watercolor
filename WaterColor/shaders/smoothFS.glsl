#version 330

in vec2 TexCoord;

uniform sampler2D objectTexture;
uniform sampler2D edgeTexture;

uniform int Width = 800; // The pixel width
uniform int Height = 600; // The pixel height

// Gaussian distribuation parameters
uniform float sigma = 5;
const float pi = 3.14159265f;

// threshold and neumber of relevate neighbors
uniform float numBlurPixelsPerSide = 10.0f;
uniform float edgethreshold = 0.3;

void main()
{
    // calcualet the distance between pixels
    float dx = 1.0 / float(Width);
    float dy = 1.0 / float(Height);
    // the useful prameter for Gaussian distribuation
    float denominator= 2.0f * pi * sigma * sigma;
    float numerator = exp(0);

    // the result
    vec4 sumValue = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    // 2D Gaussian distribuation
    // access the neighbor in four quadrants
    for (float i = 1.0f; i <= numBlurPixelsPerSide; i++) { 
        for(float j = 1.0f; j <= numBlurPixelsPerSide; j++){
            float numerator = exp(-(i * i + j * j) / (2 * sigma * sigma));
            vec2 offsets[4] = vec2[](
                vec2( i * dx,  j * dy),
                vec2(-i * dx,  j * dy),
                vec2( i * dx, -j * dy),
                vec2(-i * dx, -j * dy)
            );
            for(int k = 0; k < 4; k++){
                sumValue += texture2D(edgeTexture, TexCoord.st + offsets[k]) * numerator / denominator;
            }
        }
    }
    // access the neighbors in the axis
    for(int i = 0; i < numBlurPixelsPerSide; i++){
        float numerator = exp(-(i * i) / (2 * sigma * sigma));
        vec2 offsets[4] = vec2[](
            vec2( i * dx, 0),
            vec2(-i * dx, 0),
            vec2(0,  i * dy),
            vec2(0, -i * dy)
        );
        for(int k = 0; k < 4; k++){
            sumValue += texture2D(edgeTexture, TexCoord.st + offsets[k]) * numerator / denominator;
        }
    }

    // Shape Extression
    vec4 result = texture(objectTexture, TexCoord);
    result.rgb = result.rgb - vec3(0.2, 0.2, 0.2);
    result.a = step(edgethreshold, sumValue.x * 10);
   
    gl_FragColor = result;
    
}