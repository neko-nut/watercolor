#pragma once

#pragma once
// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
// Project includes
#include "maths_funcs.h"
#include "Shader.h"
// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
class Cube
{
private:

	GLuint numVertices = 36;

	vector<vec3> vertices = {
	 vec3(-1.0f, -1.0f, -1.0f), 
	 vec3(1.0f, -1.0f, -1.0f),
	 vec3(1.0f,  1.0f, -1.0f),
	 vec3(1.0f,  1.0f, -1.0f),
	 vec3(-1.0f,  1.0f, -1.0f),
	 vec3(-1.0f, -1.0f, -1.0f),

	 vec3(-1.0f, -1.0f,  1.0f),
	 vec3(1.0f, -1.0f,  1.0f),
	 vec3(1.0f,  1.0f,  1.0f),
	 vec3(1.0f,  1.0f,  1.0f),
	 vec3(-1.0f,  1.0f,  1.0f),
	 vec3(-1.0f, -1.0f,  1.0f),

	 vec3(-1.0f,  1.0f,  1.0f),
	 vec3(-1.0f,  1.0f, -1.0f),
	 vec3(-1.0f, -1.0f, -1.0f),
	 vec3(-1.0f, -1.0f, -1.0f),
	 vec3(-1.0f, -1.0f,  1.0f),
	 vec3(-1.0f,  1.0f,  1.0f),

	 vec3(1.0f,  1.0f,  1.0f),
	 vec3(1.0f,  1.0f, -1.0f),
	 vec3(1.0f, -1.0f, -1.0f),
	 vec3(1.0f, -1.0f, -1.0f),
	 vec3(1.0f, -1.0f,  1.0f),
	 vec3(1.0f,  1.0f,  1.0f),

	 vec3(-1.0f, -1.0f, -1.0f),
	 vec3(1.0f, -1.0f, -1.0f),
	 vec3(1.0f, -1.0f,  1.0f),
	 vec3(1.0f, -1.0f,  1.0f),
	 vec3(-1.0f, -1.0f,  1.0f),
	 vec3(-1.0f, -1.0f, -1.0f),

	 vec3(-1.0f,  1.0f, -1.0f),
	 vec3(1.0f,  1.0f, -1.0f),
	 vec3(1.0f,  1.0f,  1.0f),
	 vec3(1.0f,  1.0f,  1.0f),
	 vec3(-1.0f,  1.0f,  1.0f),
	 vec3(-1.0f,  1.0f, -1.0f),
	};




	vector<vec3> normal = {
	 vec3(0.0f,  0.0f, -1.0f),
	 vec3(0.0f,  0.0f, -1.0f),
	 vec3(0.0f,  0.0f, -1.0f),
	 vec3(0.0f,  0.0f, -1.0f),
	 vec3(0.0f,  0.0f, -1.0f),
	 vec3(0.0f,  0.0f, -1.0f),

	 vec3(0.0f,  0.0f,  1.0f),
	 vec3(0.0f,  0.0f,  1.0f),
	 vec3(0.0f,  0.0f,  1.0f),
	 vec3(0.0f,  0.0f,  1.0f),
	 vec3(0.0f,  0.0f,  1.0f),
	 vec3(0.0f,  0.0f,  1.0f),

	 vec3(-1.0f,  0.0f,  0.0f),
	 vec3(-1.0f,  0.0f,  0.0f),
	 vec3(-1.0f,  0.0f,  0.0f),
	 vec3(-1.0f,  0.0f,  0.0f),
	 vec3(-1.0f,  0.0f,  0.0f),
	 vec3(-1.0f,  0.0f,  0.0f),

	 vec3(1.0f,  0.0f,  0.0f),
	 vec3(1.0f,  0.0f,  0.0f),
	 vec3(1.0f,  0.0f,  0.0f),
	 vec3(1.0f,  0.0f,  0.0f),
	 vec3(1.0f,  0.0f,  0.0f),
	 vec3(1.0f,  0.0f,  0.0f),

	 vec3(0.0f, -1.0f,  0.0f),
	 vec3(0.0f, -1.0f,  0.0f),
	 vec3(0.0f, -1.0f,  0.0f),
	 vec3(0.0f, -1.0f,  0.0f),
	 vec3(0.0f, -1.0f,  0.0f),
	 vec3(0.0f, -1.0f,  0.0f),

	 vec3(0.0f,  1.0f,  0.0f),
	 vec3(0.0f,  1.0f,  0.0f),
	 vec3(0.0f,  1.0f,  0.0f),
	 vec3(0.0f,  1.0f,  0.0f),
	 vec3(0.0f,  1.0f,  0.0f),
	 vec3(0.0f,  1.0f,  0.0f),
	};


	vector<vec2> texture = {
	 vec2(0.0f, 0.0f),
	 vec2(1.0f, 0.0f),
	 vec2(1.0f, 1.0f),
	 vec2(1.0f, 1.0f),
	 vec2(0.0f, 1.0f),
	 vec2(0.0f, 0.0f),

	 vec2(0.0f, 0.0f),
	 vec2(1.0f, 0.0f),
	 vec2(1.0f, 1.0f),
	 vec2(1.0f, 1.0f),
	 vec2(0.0f, 1.0f),
	 vec2(0.0f, 0.0f),

	 vec2(0.0f, 0.0f),
	 vec2(1.0f, 0.0f),
	 vec2(1.0f, 1.0f),
	 vec2(1.0f, 1.0f),
	 vec2(0.0f, 1.0f),
	 vec2(0.0f, 0.0f),

	 vec2(0.0f, 0.0f),
	 vec2(1.0f, 0.0f),
	 vec2(1.0f, 1.0f),
	 vec2(1.0f, 1.0f),
	 vec2(0.0f, 1.0f),
	 vec2(0.0f, 0.0f),

	 vec2(0.0f, 1.0f),
	 vec2(1.0f, 1.0f),
	 vec2(1.0f, 0.0f),
	 vec2(1.0f, 0.0f),
	 vec2(0.0f, 0.0f),
	 vec2(0.0f, 1.0f),

	 vec2(0.0f, 1.0f),
	 vec2(1.0f, 1.0f),
	 vec2(1.0f, 0.0f),
	 vec2(1.0f, 0.0f),
	 vec2(0.0f, 0.0f),
	 vec2(0.0f, 1.0f)
	};

	vector<vec3> tangents;
	vector<vec3> bitangents;
	GLuint loc1, loc2, loc3, loc4, loc5;
	unsigned int vp_vbo;
	unsigned int vn_vbo;
	unsigned int vt_vbo;
	unsigned int vtan_vbo;
	unsigned int vbit_vbo;


public:
	void generateObjectBuffer();
	void linkCurrentBuffertoShader(GLuint shaderProgramID);

};
