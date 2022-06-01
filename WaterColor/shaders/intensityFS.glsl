#version 330

in vec2 TexCoord;

uniform sampler2D intensityTexture;

void main()
{
	vec4 intensity = texture(intensityTexture , TexCoord);
	vec4 result = vec4(vec3(1.0), 1 - intensity.x);
    gl_FragColor = result;
}