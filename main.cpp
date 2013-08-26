#include <cmath>
#include <iostream>
#include <cstdlib>
#include <string>
#include "required.h"
#include <GL/glut.h>

using namespace std;

/* Global vars */
int win_width = 1024;
int win_height = 768;
bool line_drawing_mode = true;
bool fill_mode = false;
int window_id;

line_t curr_line;
int num_points = 0;
point_t first_point;
polygon_t curr_poly;

canvas_t* canvas = NULL;

/* Function declarations */
void make_new_canvas();
void save_drawing();
void load_drawing();

/* Callbacks */
void display(void);
void reshape(int, int);
void keyboard(unsigned char, int, int);
void mouse(int, int, int, int);

/* Functions */
void make_new_canvas() {
	if (!canvas) {
		cout << "Making a new canvas. Enter RGB for background color (0-255):\n";
		int r; cout << "red:   "; cin >> r;
		int g; cout << "green: "; cin >> g;
		int b; cout << "blue:  "; cin >> b;
		color_t bg_c(r, g, b);

		canvas = new canvas_t(win_width, win_height, bg_c, NULL);

		glClearColor(r/255.0, g/255.0, b/255.0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glutPostRedisplay();
		cout << "Initialized new canvas\n";
	}
	else {
		cerr << "A canvas already exists\n";
	}
}

void save_drawing() {
	cout << "Enter filename to save to (no spaces): ";
	string filename; cin >> filename;
	cout << "Saved to file '" << filename << "'\n";
}

void load_drawing() {
	string filename;
	cout << "Enter drawing file to load: ./drawings/";
	cin >> filename;
}

/* Callbacks */
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	/* Test code here */
	// color_t c(255, 255, 255);
	// pen_t pen(c, 1.0);
	// line_t l(200, 200, 300, 300, pen);
	// l.draw();
	if (canvas) {
	}

	glFlush();
}

void reshape(int w, int h) {
	if (h == 0) h = 1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho( 0.0, (GLdouble)w, 0.0, (GLdouble)h, -1., 1. );
	glViewport( 0, 0, w, h );

	win_width = w;
	win_height = h;

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch(key) {

	// Create new canvas
	case 'n': {
		make_new_canvas();
	}
	break;

	// Initialize new drawing
	case 'd': {
		if (!canvas) {
			cerr << "ERROR: No canvas to draw on. Make a new canvas by pressing 'n'\n";
		}
		else {
			if (canvas->drawing) {
				canvas->drawing = NULL;
			}
			canvas->drawing = new drawing_t();
			cout << "Initialized new drawing\n";
		}
	}
	break;

	// Save drawing
	case 's': {
		if (canvas and canvas->drawing) {
			save_drawing();
		}
		else {
			cerr << "ERROR: No canvas exists or no drawing has been initialized\n";
		}
	}
	break;

	// Load drawing
	case 'l': {
		if (canvas) {
			if (canvas->drawing) {
				cout << "Destroying current drawing... ";
				canvas->drawing = NULL;
			}
			load_drawing();
		}
		else {
			cerr << "ERROR: No canvas to load to. Make a new canvas by pressing 'n'\n";
		}
	}
	break;

	//Toggle fill mode
	case 'f': {
		cout << "Fill mode turned " << (fill_mode ? "off" : "on") << "\n";
		fill_mode = not fill_mode;
	}
	break;

	// Toggle line drawing mode
	case '1': {
		if (canvas) {
			line_drawing_mode = true;
			cout << "Line drawing mode on\n";
		}
	}
	break;

	// Toggle polygon drawing mode
	case '2': {
		if (canvas) {
			line_drawing_mode = false;
			cout << "Polygon drawing mode on\n";	
		}
	}
	break;

	// Exit the program using Esc
	case 27: {
		cout << "Exiting myDraw...\n";
		glutDestroyWindow(window_id);
		exit(0);
	}
	break;

	}
}

void mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_DOWN) {
			cout << x << " " << y << "\n";
			if (line_drawing_mode) {
				if (num_points == 0) {
					first_point.set(x, win_height-y);
				}
				else if (num_points == 1){
					curr_line.set_xy(first_point.x, first_point.y, x, win_height-y);
					color_t black(0, 0, 0);
					pen_t p(black, 1.0, false);
					curr_line.set_pen(p);

						// cout << (int)curr_line.pen.color.red << "\n";
						// cout << (int)curr_line.pen.color.green << "\n";
						// cout << (int)curr_line.pen.color.blue << "\n";

					curr_line.draw();
				}
				num_points = 1 - num_points;				
			}
		}
	}
	glFlush();
}

int main(int argc, char* argv[]) {
	cout << "Welcome to myDraw!\n";

	cout << "Enter window dimensions:\n";
	cout << "Width: "; cin >> win_width;
	cout << "Height: "; cin >> win_height;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitWindowSize(win_width, win_height);

	window_id = glutCreateWindow("myDraw");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	glutMainLoop();
	return 0;
}