// Windows includes (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"

#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Cube.h"
#include "Board.h"

#include "frameBuffer.h"



using namespace std;

int width = 800;
int height = 600;

GLfloat lightGreenColor[3] = { 0.8f, 1.0f, 0.8f };
GLfloat lightRedColor[3] = { 1.0f, 0.8f, 0.8f };
GLfloat lightBlueColor[3] = { 0.8f, 0.8f, 1.0f };
GLfloat lightYellowColor[3] = { 0.9f, 0.9f, 0.7f };
GLfloat lightGrayColor[3] = { 0.8f, 0.8f, 0.8f };

GLfloat darkGreenColor[3] = { 0.0f, 0.4f, 0.0f };
GLfloat darkRedColor[3] = { 0.4f, 0.0f, 0.0f };
GLfloat darkBlueColor[3] = { 0.0f, 0.0f, 0.4f };
GLfloat darkYellowColor[3] = { 0.4f, 0.4f, 0.0f };
GLfloat darkGrayColor[3] = { 0.2f, 0.2f, 0.2f };

GLfloat rotate_y = 0.0f;

Shader* objectShader;
Shader* edgeShader;
Shader* boardShader;
Shader* testShader;
Mesh* cubemesh;
Texture* texture;
Texture* texture2;
Cube* cube;
Board* board;
FrameBuffer* objectBuffer;
FrameBuffer* edgeBuffer;

GLfloat lightstate = 0;

void display() {


	objectBuffer->bindBuffer();

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	mat4 view = identity_mat4();
	mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);


	glUseProgram(objectShader->ID);
	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation(objectShader->ID, "model");
	int view_mat_location = glGetUniformLocation(objectShader->ID, "view");
	int proj_mat_location = glGetUniformLocation(objectShader->ID, "proj");
	int objectColor = glGetUniformLocation(objectShader->ID, "objectColor");
	int darkColor = glGetUniformLocation(objectShader->ID, "darkColor");


	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);

	mat4 model = identity_mat4();
	model = scale(model, vec3(1.5f, 1.5f, 1.5f));
	//model = rotate_y_deg(model, rotate_y);
	model = translate(model, vec3(0.0f, 0.0f, -10.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
	glUniform3fv(objectColor, 1, lightBlueColor);
	glUniform3fv(darkColor, 1, darkBlueColor);
	cube->linkCurrentBuffertoShader(objectShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = translate(model, vec3(3.5f, 0.0f, 0.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
	glUniform3fv(objectColor, 1, lightGreenColor);
	glUniform3fv(darkColor, 1, darkGreenColor);
	cube->linkCurrentBuffertoShader(objectShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	model = translate(model, vec3(-7.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
	glUniform3fv(objectColor, 1, lightRedColor);
	glUniform3fv(darkColor, 1, darkRedColor);
	cube->linkCurrentBuffertoShader(objectShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	objectBuffer->viewBuffer(width, height);

	edgeBuffer->bindBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(edgeShader->ID);
	GLuint textureID = glGetAttribLocation(edgeShader->ID, "objectTexture");
	objectBuffer->bindTexture(GL_TEXTURE0);
	glUniform1i(textureID, 0);
	board->linkCurrentBuffertoShader(edgeShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	edgeBuffer->viewBuffer(width, height);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);


	glUseProgram(testShader->ID);
	GLuint testObjectTexture = glGetUniformLocation(testShader->ID, "objectTexture");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, objectBuffer->renderedTexture);
	glUniform1i(testObjectTexture, 0);
	board->linkCurrentBuffertoShader(testShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 6);


	glUseProgram(boardShader->ID);
	GLuint objectTexture = glGetUniformLocation(boardShader->ID, "objectTexture");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, objectBuffer->renderedTexture);
	glUniform1i(objectTexture, 0);
	GLuint edgeTexture = glGetUniformLocation(boardShader->ID, "edgeTexture");
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, edgeBuffer->renderedTexture);
	glUniform1i(edgeTexture, 1);
	board->linkCurrentBuffertoShader(boardShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);




	glutSwapBuffers();
}


// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {

	if (key == ' ') {
		if (lightstate == 0) {
			lightstate = 1;
		} else {
			lightstate = 0;
		}
	}

	glutPostRedisplay();
}


void updateScene() {

	static DWORD last_time = 0;
	DWORD curr_time = timeGetTime();
	if (last_time == 0)
		last_time = curr_time;
	float delta = (curr_time - last_time) * 0.001f;
	last_time = curr_time;

	// Rotate the model slowly around the y axis at 20 degrees per second
	rotate_y += 20.0f * delta;
	rotate_y = fmodf(rotate_y, 360.0f);

	// Draw the next frame
	glutPostRedisplay();
}




void init()
{
	objectShader = new Shader();
	objectShader->CompileShaders("../shaders/objectVS.glsl", "../shaders/objectFS.glsl");
	edgeShader = new Shader();
	edgeShader->CompileShaders("../shaders/edgeVS.glsl", "../shaders/edgeFS.glsl");
	boardShader = new Shader();
	boardShader->CompileShaders("../shaders/boardVS.glsl", "../shaders/boardFS.glsl");
	testShader = new Shader();
	testShader->CompileShaders("../shaders/watercolorVS.glsl", "../shaders/watercolorFS.glsl");

	cubemesh = new Mesh();
	cubemesh->generateObjectBufferMesh("../models/Futuristic combat jet.dae");

	texture = new Texture(GL_TEXTURE_2D, "../textures/Floor_C.jpg");
	texture->Load();
	texture2 = new Texture(GL_TEXTURE_2D, "../textures/Floor_N.jpg");
	texture2->Load();

	cube = new Cube();
	cube->generateObjectBuffer();
	board = new Board();
	board->generateObjectBuffer();

	objectBuffer = new FrameBuffer();
	objectBuffer->generateTextureBuffer(width, height);
	edgeBuffer = new FrameBuffer();
	edgeBuffer->generateTextureBuffer(width, height);

}
int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Hello Triangle");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);
	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
	return 0;
}
