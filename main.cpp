
#include "bsplinegenerator.h"

#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
using namespace std;
using namespace glm;


BSplineGenerator * bsplineGeometry;

GLFWwindow *window;
int w, h;
double mouseX, mouseY;

vector<vec2> control;
float cRadius = 0.01f;
int selected = -1;


void render () {
	glEnable (GL_DEPTH_TEST);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Functions for changing transformation matrix
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	/*
	glTranslatef (0.0f, 0.5f, 0.0f);
	glRotatef (30.0f, 0.0f, 0.0f, 1.0f);
	glScalef (2.0f, 2.0f, 2.0f);
	*/
	//Functions for changing projection matrix
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (-1, 1, -1, 1, -1, 1);
	//gluPerspective (fov, aspect ratio, near plane, far plane)
	//glFrustum


	//We draw a line on the screen, which gets transformed by the modelview matrix
	glBegin (GL_QUADS); //GL_LINE_STRIP, GL_POINTS, GL_QUADS, etc...
	/*
	glColor3f (1.0f, 0.0f, 0.0f);
	glVertex2f (-0.5f, 0.0f);
	glColor3f (0.0f, 1.0f, 0.0f);
	glVertex2f (0.5f, 0.0f);
	*/
	for (int i = 0; i < control.size(); i++)
	{
		glVertex2f(control[i].x + cRadius, control[i].y + cRadius);
		glVertex2f(control[i].x + cRadius, control[i].y - cRadius);
		glVertex2f(control[i].x - cRadius, control[i].y - cRadius);
		glVertex2f(control[i].x - cRadius, control[i].y + cRadius);

	}
	glEnd ();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < bsplineGeometry->graphData.size(); i++)
	{
		glm::vec3 vector = bsplineGeometry->graphData.at(i);
		glVertex2f(vector.x, vector.y);
	}
	glEnd();

}


void keyboard (GLFWwindow *sender, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		cout << "A was pressed.\n";
}

void mouseClick (GLFWwindow *sender, int button, int action, int mods) {

	selected = -1;
	double x = ( 2 * mouseX/w) - 1;	//x will be between -1 and 1
  double y = (-2 * mouseY/h) + 1; //y will be flipped and be between -1 and 1

	if (action == GLFW_PRESS)
	{
		for (int i = 0; i < control.size(); i++)
		{
			if (abs (control[i].x - x ) <= cRadius
			 		&& abs(control[i].y - y) <= cRadius)
			{
				selected = i;
				cout << selected << endl;
			}
		}
		}


	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//cout << mouseX << ' ' << mouseY << '\n';
		if (selected == -1)
		{
				control.push_back(vec2(x,y));
				//add the control point to the bsplineGeometry
				bsplineGeometry->addControlPoint(vec3(x,y,0));
				bsplineGeometry->generateGraph(0.001);
		}
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		if (selected != -1)
		{
			control.erase(control.begin() + selected);
			bsplineGeometry->deleteControlPoint(selected);
			bsplineGeometry->generateGraph(0.001);
			//remove the control point from the bsplineGeometry
			selected = -1;
		}
	}


}

void mousePos (GLFWwindow *sender, double x, double y) {
	mouseX = x;
	mouseY = y;

	//move a control points
	if (selected != -1)
	{
		double x_screen = ( 2 * mouseX/w) - 1;	//x will be between -1 and 1
	  double y_screen = (-2 * mouseY/h) + 1; //y will be flipped and be between -1 and 1

		control[selected].x = x_screen;
		control[selected].y = y_screen;


		//move the point from the bsplineGeometry
		bsplineGeometry->moveControlPoint(vec3(x_screen,y_screen,0), selected);
		bsplineGeometry->generateGraph(0.001);
	}

  //bsplineGeometry->generateGraph();
}


int main () {
	if (!glfwInit())
		return 1;

	window = glfwCreateWindow (640, 480, "My Window", NULL, NULL);
	bsplineGeometry = new BSplineGenerator(4);
	if (!window)
		return 1;


	glfwMakeContextCurrent (window);
	glfwSetKeyCallback (window, keyboard);
	glfwSetMouseButtonCallback (window, mouseClick);
	glfwSetCursorPosCallback (window, mousePos);
	while (!glfwWindowShouldClose (window)) {
		glfwGetFramebufferSize (window, &w, &h);
		glViewport (0, 0, w, h);

		render ();

		glfwSwapBuffers (window);
		glfwPollEvents();
	}

	glfwDestroyWindow (window);
	glfwTerminate();
	return 0;
}
