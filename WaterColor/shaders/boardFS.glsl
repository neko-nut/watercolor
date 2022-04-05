#version 330

in vec2 TexCoord;

uniform sampler2D objectTexture;
uniform sampler2D edgeTexture;

uniform int Width = 800; // The pixel width
uniform int Height = 600; // The pixel height

uniform float sigma = 5;
const float pi = 3.14159265f;
const float numBlurPixelsPerSide = 8.0f;
const vec2 blurMultiplyVecV = vec2(0.0f, 1.0f);
const vec2 blurMultiplyVecH = vec2(1.0f, 0.0f);

// max: 0.65, min: 0.25

float interpolation(float val){
    //if more then 0.65
    //step(0.65, val);
    //return (val - 0.2) / (0.65 - 0.2);
    val = (val - 0.3) / (0.8 - 0.3);
    //return step(0.0, val);
    val = val * step(0, val);
    val = min(1.0, val);
    return 2 * val - val * val;
}
uniform float edgethreshold = 0.3;
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

    // Shape Extression
    // vec4 result = texture(objectTexture, TexCoord);
    // result.a = 1 - step(edgethreshold, max(max(avgValue.x, avgValue.y), avgValue.z) * 10);
    // gl_FragColor = result;

    vec4 result = texture(objectTexture, TexCoord);
    //result.a = 0.5 + step(edgethreshold, max(max(avgValue.x, avgValue.y), avgValue.z) * 10) * 0.5;
    //result.a = 0.5 + max(max(avgValue.x, avgValue.y), avgValue.z) * 10;
    //result.a = step(edgethreshold, max(max(avgValue.x, avgValue.y), avgValue.z) * 10) * 0.5;
    //result.a = 0.5 + avgValue.x * 5;
    result.rgb = result.rgb - vec3(0.2, 0.2, 0.2);
    //result.a = 0;
    result.a = interpolation(avgValue.x * 10);
    //result.a = interpolation(avgValue.x * 10);
   
    gl_FragColor = result;
    
}