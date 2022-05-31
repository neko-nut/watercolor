#version 330

in vec2 TexCoord;

uniform sampler2D objectTexture;
uniform sampler2D paperTexture;
uniform sampler2D watercolorTexture;
void main()
{
	vec4 watercolor = texture(watercolorTexture , TexCoord) * 2;
	vec4 paper = texture(paperTexture , TexCoord);
	vec4 result = texture(objectTexture , TexCoord);
	float density = paper.x + paper.y + paper.z;
	density = density / 2;
	result = result - (result - result * result) * (density - 1);

    gl_FragColor = result;
}