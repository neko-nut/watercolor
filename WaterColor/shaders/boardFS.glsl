#version 330

in vec2 TexCoord;

uniform sampler2D objectTexture;
void main()
{
	// dispaly the original texture on the screen
	vec4 result = texture(objectTexture, TexCoord);
    gl_FragColor = result;
}