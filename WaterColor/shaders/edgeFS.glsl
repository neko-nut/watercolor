#version 330

in vec2 TexCoord;

uniform sampler2D objectTexture;

uniform int Width = 800; // The pixel width
uniform int Height = 600; // The pixel height

// kernel
float kernel[9] = float[](
    1,  1,  1,
    1, -8,  1,
    1,  1,  1
);

void main()
{

    // calcualet the distance between pixels
    float dx = 1.0 / float(Width);
    float dy = 1.0 / float(Height);
    
    // clalculate the position of the nearby pixels
    vec2 offsets[9] = vec2[](
        vec2(-dx, dy),    vec2( 0.0f, dy),    vec2( dx, dy),
        vec2(-dx, 0.0f),  vec2( 0.0f, 0.0f),  vec2( dx, 0.0f),
        vec2(-dx, -dy),   vec2( 0.0f, -dy),   vec2( dx, -dy)
    );

    vec3 sampleTex[9];
    // get the nearby pixel
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(objectTexture, TexCoord.st + offsets[i]));
    }
    // multiply by the kernel
    vec3 result = vec3(0.0);
    for(int i = 0; i < 9; i++)
        result = result + sampleTex[i] * kernel[i];
    
    // detect the edge
    result = step(vec3(0.2, 0.2, 0.2), abs(result));
    result = vec3(max(max(result.x, result.y), result.z));

    gl_FragColor = vec4(result, 1.0);
}