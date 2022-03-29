#version 330

in vec3 vertex_position;
in vec3 vertex_normal;

out vec3 worldPosition;
out vec3 normalVector;

uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;



void main(){

	// the position of the vertexes in the world coordinates
	worldPosition = vec3(model * vec4(vertex_position, 1.0));

	// normalize the normal vector into unit vector
	// use inverse() and transpose() function to avoid the scale problems
	normalVector = normalize(inverse(transpose(mat3(model))) * vertex_normal).rgb;


	// Convert position to clip coordinates and pass along
	gl_Position = proj * view * model * vec4(vertex_position,1.0);
}


  