/*
 Name: test01.2.Two.Windows.cpp
 Author: Rodrigo Luis de Souza da Silva
 Author: Bruno José Dembogurski
 Date: 27/01/2011
 Last Update: 27/01/2016
 Description: Managing two windows using PixelLab.
 */

#include "pixelLab.h"
#ifdef WIN32
#include <windows.h>
#endif
#include "GL/glut.h"
#include <stdio.h>
#include <stdlib.h>

#include "Debug.h"

#include <iostream>
#include <vector>

#define CWIDTH 256
#define CHEIGHT 256

int WIDTH = 0;
int HEIGHT = 0;

#define BOX_SIZE 232

#define DASHED_AMOUNT 4

#define BOX_LEFT (CWIDTH - BOX_SIZE)
#define BOX_RIGHT CWIDTH
#define BOX_TOP 0
#define BOX_BOTTOM BOX_SIZE

#define DASHED_SPACING (BOX_SIZE / DASHED_AMOUNT)

typedef struct {
	float x;
	float y;
} Point;

// Image Objects
PixelLab *img = NULL, *imgOriginal = NULL, *img2 = NULL, *imgOriginal2 = NULL;
int window2 = 0;
int brightnessSlider = 128;

std::vector<Point> pts;

void idle() {
	glutPostRedisplay();
}

static void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1);
	glLoadIdentity();

	glPushMatrix();

	// View first image
	float factor = std::min(WIDTH / ((float) img2->GetWidth() * 2),
			HEIGHT / ((float) img2->GetHeight() * 2));

	float x = -1
				+ ((WIDTH  - (img2->GetWidth()) * factor * 2)) / (WIDTH * 2.0f);
		float y = ((HEIGHT - (img2->GetHeight()) * factor * 2)) / (HEIGHT * 2.0f);

	glRasterPos2f(x, y);
	glPixelZoom(factor, factor);

	img2->ViewImage();

	// View second image
	factor = std::min(WIDTH / ((float) img->GetWidth() * 2),
			HEIGHT / ((float) img->GetHeight() * 2));

	x = ((WIDTH  - (img->GetWidth()) * factor * 2) / 2.0f) / (WIDTH * 2.0f);
	y = ((HEIGHT  - (img->GetWidth()) * factor * 2) / 2.0f) / (HEIGHT * 2.0f);

	glRasterPos2f(x, y);
	glPixelZoom(factor, factor);

	img->ViewImage();

	glPopMatrix();

	glutSwapBuffers();
}

void modifyImage() {
	int value = brightnessSlider - 128; // To make the middle of the slider, zero
	int pixelVal;

	for (int y = 0; y < imgOriginal->GetHeight(); y++) {
		for (int x = 0; x < imgOriginal->GetWidth(); x++) {
			pixelVal = imgOriginal->GetGrayValue(x, y);
			pixelVal += value;
			if (pixelVal >= 255)
				pixelVal = 255;
			if (pixelVal < 0)
				pixelVal = 0;
			img->SetGrayValue(x, y, (uByte) pixelVal);
		}
	}

	// Update both windows
	glutPostWindowRedisplay(window2);
}

// Keyboard
static void key(unsigned char key, int x, int y) {
	switch (key) {
	case 27:

	case 'q':
		exit(0);
		break;
	}

	glutPostRedisplay();
}

// Special Keys callback
void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		brightnessSlider = (brightnessSlider < 0) ? 0 : brightnessSlider - 1;
		modifyImage();
		break;

	case GLUT_KEY_DOWN:
		brightnessSlider =
				(brightnessSlider > 255) ? 255 : brightnessSlider + 1;
		modifyImage();
		break;

	case 'q':
		exit(0);
	}

	glutPostRedisplay();
}

// Mouse callback - Capture mouse click in the brightness window
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN)
			brightnessSlider = y;
	modifyImage();
}

// Motion callback - Capture mouse motion when left button is clicked
void motion(int x, int y) {
	brightnessSlider = y;
	modifyImage();
}

void init() {
	imgOriginal = new PixelLab();
	imgOriginal->Read("figs/lenaGray.png");

	imgOriginal2 = new PixelLab();
//	imgOriginal2->Read("figs/seila.bmp");
	imgOriginal2->Read("figs/woman.png");

	Point p1 = { 0.0f, 0.0f };
	pts.push_back(p1);

	Point p2 = { 0.3f * 256, 0.6f * 256 };
	pts.push_back(p2);

	Point p3 = { 0.6f * 256, 0.3f * 256 };
	pts.push_back(p3);

	Point pn = { 256.0f, 256.0f };
	pts.push_back(pn);

	img = new PixelLab();
	img->Copy(imgOriginal);

	img2 = new PixelLab();
	img2->Copy(imgOriginal2);

	printf(
			"Change brightness clicking on the left window\n or using the 'up' and 'down' keyboard keys.\n");
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	// Init image objects
	init();

	// Create two windows
	window2 = glutCreateWindow("Display Window");

	///////////////////////////////////////////////////////////
	// Display Window
	glutSetWindow(window2); // Change current window to 2
	glutDisplayFunc(display);
	glutReshapeWindow(imgOriginal->GetWidth(), imgOriginal->GetHeight());
	glutPositionWindow(25 + CWIDTH, 30);
	glutKeyboardFunc(key);

	WIDTH = imgOriginal->GetWidth();
	HEIGHT = imgOriginal->GetHeight();

	glutMainLoop();

	return 0;
}
