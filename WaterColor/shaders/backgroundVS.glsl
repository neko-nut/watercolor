#version 330

in vec3 position;
in vec2 TexCoordinate;

out vec2 TexCoord;

void main()
{   
    // dispaly the original texture on the screen
	gl_Position = vec4(position.x, position.y, position.z, 1.0);
    TexCoord = vec2(TexCoordinate.x, TexCoordinate.y);
}