#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

class FrameBuffer
{	
public:
	GLuint Framebuffer = 0;
	GLuint renderedTexture;
	void generateTextureBuffer(int weight, int hight);
	void bindBuffer();
	void viewBuffer(int width, int height);
	void bindTexture(GLenum TextureUnit);
};

