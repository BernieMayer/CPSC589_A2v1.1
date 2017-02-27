
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

float u_Step = 0.001;
double geom_u = 0.5f;

bool geometryMode = true;


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
		glColor3f(0.0f, 0.9f, 0.0);
		glVertex2f(control[i].x + cRadius, control[i].y + cRadius);
		glVertex2f(control[i].x + cRadius, control[i].y - cRadius);
		glVertex2f(control[i].x - cRadius, control[i].y - cRadius);
		glVertex2f(control[i].x - cRadius, control[i].y + cRadius);

	}
	glEnd ();


	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < bsplineGeometry->graphData.size(); i++)
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glm::vec3 vector = bsplineGeometry->graphData.at(i);
		glVertex2f(vector.x, vector.y);
	}
	glEnd();

	if (geometryMode)
	{
		for (int k = 0; k < bsplineGeometry->geometryData.size(); k++)
		{
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < bsplineGeometry->geometryData.at(k).size(); i++)
		{
			if (k % 3 == 0)
			{
			glColor3f(1.0f, 0.0f, 0.0f);
		} else if ( k % 3 == 1)
		{
			glColor3f(1.0f, 0.5f, 0.0f);
		} else if (k % 3 == 2)
		{
			glColor3f(1.0f, 0.0f, 0.5f);
		} else {
			glColor3f(1.0f, 0.5f, 0.5f);
		}
			glm::vec3 vector = bsplineGeometry->geometryData.at(k).at(i);
			glVertex2f(vector.x, vector.y);
		}
		glEnd();

		glBegin(GL_QUADS);

		glPointSize( 10.0f);
		glColor3f(1.0f, 0.7f, 0.0f);
		glm::vec3 aPoint = bsplineGeometry->pointOfInterest;
		glVertex2f(aPoint.x + cRadius, aPoint.y + cRadius);
		glVertex2f(aPoint.x + cRadius, aPoint.y - cRadius);
		glVertex2f(aPoint.x - cRadius, aPoint.y - cRadius);
		glVertex2f(aPoint.x - cRadius, aPoint.y + cRadius);

		glEnd();
}



	}
}


void keyboard (GLFWwindow *sender, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		cout << "A was pressed.\n";

	if (key == GLFW_KEY_K && (action == GLFW_PRESS))
	{
		bsplineGeometry->decraseK();
		bsplineGeometry->generateGraph(u_Step);
	} else if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		bsplineGeometry->increaseK();
		bsplineGeometry->generateGraph(u_Step);
	} else if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		geometryMode = !geometryMode;
		bsplineGeometry->generateGeometryModeData(geom_u);
	} else if (key == GLFW_KEY_U && action == GLFW_PRESS)
	{
		std::cout<< "Enter the value for u increment \n";

		float u_inc_temp;
		std::cin >> u_inc_temp;
		u_Step= u_inc_temp;
		bsplineGeometry->generateGraph(u_Step);
	}
	else if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		 u_Step = u_Step + 0.05;

		bsplineGeometry->generateGraph(u_Step);
	} else if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		if (u_Step > 0.05)
		{
			u_Step = u_Step - 0.05;
		} else
		{
			u_Step = 0.04;
		}
		bsplineGeometry->generateGraph(u_Step);
	} else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	} else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		geom_u += 0.05;
		geometryMode = true;

		bsplineGeometry->debug = true;
		bsplineGeometry->generateGeometryModeData(geom_u);
		bsplineGeometry->debug = false;

	} else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
	 geom_u -= 0.05;
	 geometryMode = true;

	 bsplineGeometry->debug = true;
	 bsplineGeometry->generateGeometryModeData(geom_u);
	 bsplineGeometry->debug = false;
 } else if (key == GLFW_KEY_D && action == GLFW_PRESS)
 {
	 bsplineGeometry->debug = true;

	 bsplineGeometry->generateGeometryModeData(0.95f);
	 bsplineGeometry->debug = false;
 }


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
		cout << mouseX << ' ' << mouseY << '\n';
		if (selected == -1)
		{
				control.push_back(vec2(x,y));
				//add the control point to the bsplineGeometry
				bsplineGeometry->addControlPoint(vec3(x,y,0));
				bsplineGeometry->generateGraph(u_Step);
		}
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		if (selected != -1)
		{
			control.erase(control.begin() + selected);
			bsplineGeometry->deleteControlPoint(selected);
			bsplineGeometry->generateGraph(u_Step);
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
		bsplineGeometry->generateGraph(u_Step);
	}

  //bsplineGeometry->generateGraph();
}


int main () {
	if (!glfwInit())
		return 1;

	window = glfwCreateWindow (640, 480, "My Window", NULL, NULL);
	bsplineGeometry = new BSplineGenerator(3);
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
