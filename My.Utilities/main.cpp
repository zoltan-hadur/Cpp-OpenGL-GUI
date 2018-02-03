#include <GL\glew.h>
#include <GL\glext.h>
#include <GL\freeglut.h>
#include "GLConsole.h"
#include "Stopwatch.h"
#include "BMP.h"

GLConsole console;							// GLConsole instance
Stopwatch watch;							// Stopper for controlling fps
const int screen_width = 192 * 5;			// Width of the window
const int screen_height = 108 * 5;			// Height of the window
int keys_down[256];							// For easy check of pressed keys
float max_fps;								// Maximum allowed fps of the application
int window_id;								// Main window's id

void capture_screen() {
	static unsigned int id = 0;

	std::vector<unsigned char> image;
	int width, height;
	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);
	image.resize(width*height * 3);
	glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, image.data());

	CreateDirectoryA("captured", NULL);
	std::string file_name = "image_" + std::to_string(id++) + ".bmp";
	BMP::write("captured/" + file_name, image, width, height);
	GLConsole::cout << file_name + " created!\n";
}

void onInitialization() {
	glViewport(0, 0, screen_width, screen_height);

	console.init();
	max_fps = 60;
	GLConsole::cvars.attach_cvar<float>("app.max_fps", &max_fps, "Determines the maximum allowed frames per second. Interval: [30, infty), 0 for unlimited.");
	GLConsole::add_function("app.capture_screen()", std::bind(&capture_screen));
	console.print_help();

	watch.start();
}

void onDisplay() {
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	console.draw();

	glutSwapBuffers();
}

void onKeyboard(unsigned char key, int x, int y) {
	if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
		console.shift_pressed();
	} else {
		console.shift_released();
	}

	if (key == 27) {	// Esc key
		glutLeaveFullScreen();
		glutDestroyWindow(window_id);
		exit(EXIT_SUCCESS);
	}

	if (console.is_open()) {
		console.on_keyboard(key);
	} else {
		switch (key) {
			case ' ':
				glutFullScreenToggle();
				break;
			default:
				keys_down[key] = true;
				break;
		}
	}
}

void onKeyboardUp(unsigned char key, int x, int y) {
	keys_down[key] = false;
}

void onSpecial(int key, int x, int y) {
	if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
		console.shift_pressed();
	} else {
		console.shift_released();
	}

	if (key == GLUT_KEY_F1) {
		console.toggle();
	} else {
		if (console.is_open()) {
			console.on_special(key);
		} else {
			switch (key) {
				// Own handler
			}
		}
	}
}

void onSpecialUp(int key, int x, int y) {
	if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
		console.shift_pressed();
	} else {
		console.shift_released();
	}
}

int last_x, last_y;
void onMouse(int button, int state, int x, int y) {
	if ((button == 3) || (button == 4)) {	// Mouse wheel event
		if (state == GLUT_UP) return;
		if (console.is_open()) {
			(button == 3) ? console.scroll_up() : console.scroll_down();
		} else {
			// Own handler
		}
	} else {								// Normal click event
		last_x = x; last_y = y;
	}
}

void onMouseMotion(int x, int y) {
	int dx = x - last_x;
	int dy = y - last_y;
	last_x = x;
	last_y = y;
}

float wait_time = 0;
float time_elapsed = 0;
void onIdle() {
	float dt = watch.get_delta_time();
	time_elapsed = time_elapsed + dt;

	console.process();

	if (max_fps == 0) {						// Maxmimum fps is unlimited
		glutPostRedisplay();
	} else if (time_elapsed >= wait_time) {	// Control the fps
		max_fps = std::max(max_fps, 30.0f);	// For practical reasons
		wait_time = std::max((1.0f - (time_elapsed * max_fps / 1 - max_fps * wait_time)) / max_fps, 0.0f);	// Calculate the dt between frames
		time_elapsed = 0;
		glutPostRedisplay();
	}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	window_id = glutCreateWindow("OpenGL Console Application");

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	onInitialization();

	glutDisplayFunc(onDisplay);
	glutMouseFunc(onMouse);
	glutMotionFunc(onMouseMotion);
	glutIdleFunc(onIdle);
	glutKeyboardFunc(onKeyboard);
	glutKeyboardUpFunc(onKeyboardUp);
	glutSpecialFunc(onSpecial);
	glutSpecialUpFunc(onSpecialUp);

	glutMainLoop();

	return 0;
}
