#version 330

in vec2 TexCoord;

uniform sampler2D objectTexture;
void main()
{
	vec4 result = texture(objectTexture, TexCoord);
	//result = result * vec4(1.0, 0.0, 0.0, 1.0);
    gl_FragColor = result;
}