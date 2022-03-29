#version 330

in vec3 position;
in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{   
	gl_Position = vec4(position.x, position.y, position.z, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	
}