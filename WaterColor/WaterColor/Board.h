#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

class Board
{

private:
	GLuint numVertices = 6;
	float vertices[30] = {
		// positions        // texture coords
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f, // top right
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // bottom right
		 -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,  // top left 

		 -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // bottom left
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // bottom right
		 -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,  // top left 
	};

public:
	GLuint VBO;
	GLuint generateObjectBuffer();
	void linkCurrentBuffertoShader(GLuint shaderProgramID);
};

