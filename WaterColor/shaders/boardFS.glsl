#version 330

in vec2 TexCoord;

uniform sampler2D objectTexture;
uniform sampler2D edgeTexture;

uniform int Width = 800; // The pixel width
uniform int Height = 600; // The pixel height

uniform float sigma = 5;
const float pi = 3.14159265f;
const float numBlurPixelsPerSide = 7.0f;
const vec2 blurMultiplyVecV = vec2(0.0f, 1.0f);
const vec2 blurMultiplyVecH = vec2(1.0f, 0.0f);

uniform float edgethreshold = 0.5;
void main()
{
    float dx = 1.0 / float(Width);
    float dy = 1.0 / float(Height);


    float denominator= 2.0f * pi * sigma * sigma;
    float numerator = exp(0);

    vec4 avgValue = vec4(0.0f, 0.0f, 0.0f, 1.0f);

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
                avgValue += texture2D(edgeTexture, TexCoord.st + offsets[k]) * numerator / denominator;
            }
        }
    }
    for(int i = 0; i < numBlurPixelsPerSide; i++){
        float numerator = exp(-(i * i) / (2 * sigma * sigma));
        vec2 offsets[4] = vec2[](
            vec2( i * dx, 0),
            vec2(-i * dx, 0),
            vec2(0,  i * dy),
            vec2(0, -i * dy)
        );
        for(int k = 0; k < 4; k++){
            avgValue += texture2D(edgeTexture, TexCoord.st + offsets[k]) * numerator / denominator;
        }
    }

    vec4 result = texture(objectTexture, TexCoord);
    result.a = 1 - step(edgethreshold, max(max(avgValue.x, avgValue.y), avgValue.z) * 10);
    gl_FragColor = result;
    
}