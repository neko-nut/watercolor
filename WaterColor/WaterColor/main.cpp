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

#include "../imgui-master/imgui.h"
#include "../imgui-master/imgui_impl_glut.h"
#include "../imgui-master/imgui_impl_opengl3.h"

// Project includes
#include "maths_funcs.h"

#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Cube.h"
#include "Board.h"

#include "frameBuffer.h"


#ifdef _MSC_VER
#pragma warning (disable: 4505) // unreferenced local function has been removed
#endif

using namespace std;

int width = 800;
int height = 600;

GLfloat lightGreenColor[3] = { 0.6f, 0.8f, 0.6f };
GLfloat lightRedColor[3] = { 0.8f, 0.6f, 0.6f };
GLfloat lightBlueColor[3] = { 0.6f, 0.6f, 0.8f };
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
Shader* extractEdgeShader;
Shader* smoothEdgeShader;
Shader* intensityShader;
Shader* boardShader;

Mesh* cubemesh;
Texture* paperTexture;
Texture* watercolorTexture;
Cube* cube;
Board* board;
FrameBuffer* objectBuffer;
FrameBuffer* edgeBuffer;
FrameBuffer* darkEdgeBuffer;

GLfloat threshold = 0.1;
int smoothPixel = 10.0;
bool displayOrigin = false;
bool displayEdge = false;
bool displayExtractedEdge = false;
bool displaySmoothEdge = false;
bool displayDarkerEdgeCube = false;


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
	model = rotate_y_deg(model, rotate_y);
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



	//darkEdgeBuffer->bindBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(boardShader->ID);
	GLuint testObjectTexture = glGetUniformLocation(boardShader->ID, "objectTexture");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, objectBuffer->renderedTexture);
	glUniform1i(testObjectTexture, 0);
	board->linkCurrentBuffertoShader(boardShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glUseProgram(smoothEdgeShader->ID);
	GLuint objectTexture = glGetUniformLocation(smoothEdgeShader->ID, "objectTexture");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, objectBuffer->renderedTexture);
	glUniform1i(objectTexture, 0);
	GLuint edgeTexture = glGetUniformLocation(smoothEdgeShader->ID, "edgeTexture");
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, edgeBuffer->renderedTexture);
	glUniform1i(edgeTexture, 1);
	GLuint numPixel = glGetUniformLocation(smoothEdgeShader->ID, "numBlurPixelsPerSide");
	glUniform1f(numPixel, (float)smoothPixel);
	GLuint edgethreshold = glGetUniformLocation(smoothEdgeShader->ID, "edgethreshold");
	glUniform1f(edgethreshold, (float)threshold);
	board->linkCurrentBuffertoShader(smoothEdgeShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	//darkEdgeBuffer->viewBuffer(width, height);


	glUseProgram(intensityShader->ID);
	GLuint intensityPaperTexture = glGetUniformLocation(intensityShader->ID, "intensityTexture");
	glActiveTexture(GL_TEXTURE0);
	paperTexture->Bind(GL_TEXTURE0);
	glUniform1i(intensityPaperTexture, 0);
	board->linkCurrentBuffertoShader(intensityShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 6);



	if (displayOrigin) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(boardShader->ID);
		GLuint testObjectTexture = glGetUniformLocation(boardShader->ID, "objectTexture");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, objectBuffer->renderedTexture);
		glUniform1i(testObjectTexture, 0);
		board->linkCurrentBuffertoShader(boardShader->ID);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}


	if (displayEdge) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(boardShader->ID);
		GLuint testObjectTexture = glGetUniformLocation(boardShader->ID, "objectTexture");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, edgeBuffer->renderedTexture);
		glUniform1i(testObjectTexture, 0);
		board->linkCurrentBuffertoShader(boardShader->ID);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	if (displayExtractedEdge) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(extractEdgeShader->ID);
		GLuint objectTexture = glGetUniformLocation(extractEdgeShader->ID, "objectTexture");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, objectBuffer->renderedTexture);
		glUniform1i(objectTexture, 0);
		GLuint edgeTexture = glGetUniformLocation(extractEdgeShader->ID, "edgeTexture");
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, edgeBuffer->renderedTexture);
		glUniform1i(edgeTexture, 1);
		GLuint edgethreshold = glGetUniformLocation(extractEdgeShader->ID, "edgethreshold");
		glUniform1f(edgethreshold, (float)threshold);
		GLuint numPixel = glGetUniformLocation(extractEdgeShader->ID, "numBlurPixelsPerSide");
		glUniform1f(numPixel, (float)smoothPixel);
		board->linkCurrentBuffertoShader(extractEdgeShader->ID);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
	}


	if (displaySmoothEdge) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		glUseProgram(smoothEdgeShader->ID);
		GLuint objectTexture = glGetUniformLocation(smoothEdgeShader->ID, "objectTexture");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, objectBuffer->renderedTexture);
		glUniform1i(objectTexture, 0);
		GLuint edgeTexture = glGetUniformLocation(smoothEdgeShader->ID, "edgeTexture");
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, edgeBuffer->renderedTexture);
		glUniform1i(edgeTexture, 1);
		GLuint numPixel = glGetUniformLocation(smoothEdgeShader->ID, "numBlurPixelsPerSide");
		glUniform1f(numPixel, (float)smoothPixel);
		GLuint edgethreshold = glGetUniformLocation(smoothEdgeShader->ID, "edgethreshold");
		glUniform1f(edgethreshold, (float)threshold);
		board->linkCurrentBuffertoShader(smoothEdgeShader->ID);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
	}

	
	if (displayDarkerEdgeCube) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(boardShader->ID);
		GLuint testObjectTexture = glGetUniformLocation(boardShader->ID, "objectTexture");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, darkEdgeBuffer->renderedTexture);
		glUniform1i(testObjectTexture, 0);
		board->linkCurrentBuffertoShader(boardShader->ID);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	//glutSwapBuffers();
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

// use ImGui to modify the distance of the cubes
void my_display_code()
{
	ImGui::Begin("Parameters");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::SliderFloat("Threshold", &threshold, 0.0f, 1.0f);
	ImGui::SliderInt("Smooth Pixel", &smoothPixel, 1.0f, 20.0f);
	ImGui::Checkbox("Origin", &displayOrigin);
	ImGui::Checkbox("Edge Detection", &displayEdge);
	ImGui::Checkbox("Edge Extraction", &displayExtractedEdge);
	ImGui::Checkbox("Smooth Edge", &displaySmoothEdge);
	ImGui::Checkbox("Final Result", &displayDarkerEdgeCube);
	ImGui::End();

}


void glut_display_func()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	my_display_code();
	display();
	
	// Rendering
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);

	glUseProgram(0);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glutSwapBuffers();
}





void init()
{
	objectShader = new Shader();
	objectShader->CompileShaders("../shaders/objectVS.glsl", "../shaders/objectFS.glsl");
	edgeShader = new Shader();
	edgeShader->CompileShaders("../shaders/edgeVS.glsl", "../shaders/edgeFS.glsl");
	extractEdgeShader = new Shader();
	extractEdgeShader->CompileShaders("../shaders/extractEdgeVS.glsl", "../shaders/extractEdgeFS.glsl");
	smoothEdgeShader = new Shader();
	smoothEdgeShader->CompileShaders("../shaders/smoothEdgeVS.glsl", "../shaders/smoothEdgeFS.glsl");
	intensityShader = new Shader();
	intensityShader->CompileShaders("../shaders/intensityVS.glsl", "../shaders/intensityFS.glsl");
	boardShader = new Shader();
	boardShader->CompileShaders("../shaders/boardVS.glsl", "../shaders/boardFS.glsl");

	cubemesh = new Mesh();
	cubemesh->generateObjectBufferMesh("../models/Futuristic combat jet.dae");

	paperTexture = new Texture(GL_TEXTURE_2D, "../textures/intensity.jpg");
	paperTexture->Load();
	watercolorTexture = new Texture(GL_TEXTURE_2D, "../textures/watercolor.jpg");
	watercolorTexture->Load();

	cube = new Cube();
	cube->generateObjectBuffer();
	board = new Board();
	board->generateObjectBuffer();

	objectBuffer = new FrameBuffer();
	objectBuffer->generateTextureBuffer(width, height);
	edgeBuffer = new FrameBuffer();
	edgeBuffer->generateTextureBuffer(width, height);
	darkEdgeBuffer = new FrameBuffer();
	darkEdgeBuffer->generateTextureBuffer(width, height);

}
int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
#ifdef __FREEGLUT_EXT_H__
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Watercolor");



	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// init ImGui
	ImGui_ImplGLUT_Init();
	ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL3_Init();

	// Tell glut where the display function is
	glutDisplayFunc(glut_display_func);
	glutIdleFunc(updateScene);


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

	// Cleanup ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();

	return 0;
}
