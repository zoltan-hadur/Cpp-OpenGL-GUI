#include <GL\freeglut.h>
#include <algorithm>
#include <iostream>
#include "GLUI\GLUI.h"
#include "Stopwatch.h"

Stopwatch watch;							// Stopper for controlling fps
const int screen_width = 192 * 5;			// Width of the window
const int screen_height = 108 * 5;			// Height of the window
int keys_down[256];							// For easy check of pressed keys
float max_fps;								// Maximum allowed fps of the application
GLUI::Window* window;
Event event;

class fos : public EventListener {
public:
	fos() {

	}
	int szar = 0;
	void action_performed(void* sender, Event& e) override {
		//GLUI::Button* btn = (GLUI::Button*)sender;
		szar++;
		std::cout << szar << std::endl;
		//btn->get_label()->set_text(std::to_string(szar));
		//if (e.mouse_scroll_up && e.mouse_pressed) {
		//	szar++;
		//} else if (e.mouse_scroll_down && e.mouse_pressed) {
		//	szar--;
		//}
		//std::cout << szar << std::endl;
		//btn->get_label()->set_text(std::to_string(szar));
	}
};

void onInitialization() {
	glViewport(0, 0, screen_width, screen_height);

	max_fps = 0;

	GLUI::Panel* panel = new GLUI::Panel("Basic panel", 10, 10, 800, 500);
	window->add_component(panel);

	GLUI::TabbedPanel* panel2 = new GLUI::TabbedPanel(20, 40, 600, 400);
	panel2->add_tab("Tab names");
	panel2->add_tab("has");
	panel2->add_tab("automatic length");
	GLUI::Panel* tab0 = panel2->get_tab("Tab names");
	GLUI::Panel* tab1 = panel2->get_tab("has");
	GLUI::Panel* tab2 = panel2->get_tab("automatic length");
	panel->add_component(panel2);

	GLUI::ScrollPanel* panel0 = new GLUI::ScrollPanel(GLUI::ScrollPanel::ALIGN::VERTICAL, 10, 10, 500, 350);
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 20; ++j) {
			panel0->add_component(new GLUI::Button("B" + std::to_string(j * 6 + i), 10 + 75 * i, 10 + 25 * j, 70, 20));
		}
	}
	tab0->add_component(panel0);

	GLUI::ScrollPanel* panel1 = new GLUI::ScrollPanel(GLUI::ScrollPanel::ALIGN::HORIZONTAL, 10, 10, 500, 350);
	for (int i = 0; i < 12; ++i) {
		for (int j = 0; j < 12; ++j) {
			panel1->add_component(new GLUI::Button("B" + std::to_string(j * 12 + i), 10 + 75 * i, 10 + 25 * j, 70, 20));
		}
	}
	tab1->add_component(panel1);


	GLUI::ComboBox* cbox = new GLUI::ComboBox(10, 10, 200, 20);
	cbox->add_element("1");
	cbox->add_element("2");
	cbox->add_element("3");
	cbox->add_element("4");
	cbox->add_element("5");
	cbox->add_element("6");
	cbox->add_element("7");
	cbox->add_element("8");
	cbox->add_element("9");
	cbox->add_element("10");
	tab2->add_component(cbox);

	GLUI::Button* btn0 = new GLUI::Button("ASD", 220, 10, 80, 20);
	fos* szar = new fos();
	btn0->add_event_listener(szar);
	tab2->add_component(btn0);

	GLUI::CheckBox* check_box = new GLUI::CheckBox(310, 10, 20, 20);
	tab2->add_component(check_box);

	GLUI::TextBox* text_box0 = new GLUI::TextBox(340, 10, 250, 100);
	text_box0->add_event_listener(szar);
	tab2->add_component(text_box0);

	GLUI::TextBox* text_box1 = new GLUI::TextBox(340, 120, 250, 20);
	tab2->add_component(text_box1);

	watch.start();
}

void onDisplay() {
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glVertex2f(-1, -1);

	glColor3f(1, 0, 0);
	glVertex2f(-1, 1);

	glColor3f(0, 1, 0);
	glVertex2f(1, 1);

	glColor3f(0, 1, 0);
	glVertex2f(1, -1);
	glEnd();

	window->render();

	glutSwapBuffers();
}

void onKeyboard(unsigned char key, int x, int y) {
	//keys_down[key] = true;
	event.active_alt = (glutGetModifiers() & GLUT_ACTIVE_ALT);
	event.active_ctrl = (glutGetModifiers() & GLUT_ACTIVE_CTRL);
	event.active_shift = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
	event.keys_down[key] = true;
	event.key_code = key;
	event.key_pressed = true;
	event.key_released = false;
	event.special_pressed = false;
	event.special_released = false;
	event.x = x;
	event.y = y;
	event.dx = 0;
	event.dy = 0;
	window->event_handler(event);
	event.key_pressed = false;
	event.key_released = false;
	event.special_pressed = false;
	event.special_released = false;
}

void onKeyboardUp(unsigned char key, int x, int y) {
	//keys_down[key] = false;
	event.active_alt = (glutGetModifiers() & GLUT_ACTIVE_ALT);
	event.active_ctrl = (glutGetModifiers() & GLUT_ACTIVE_CTRL);
	event.active_shift = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
	event.keys_down[key] = false;
	event.key_code = key;
	event.key_pressed = false;
	event.key_released = true;
	event.special_pressed = false;
	event.special_released = false;
	event.x = x;
	event.y = y;
	event.dx = 0;
	event.dy = 0;
	window->event_handler(event);
	event.key_pressed = false;
	event.key_released = false;
	event.special_pressed = false;
	event.special_released = false;
}

void onSpecial(int key, int x, int y) {
	event.active_alt = (glutGetModifiers() & GLUT_ACTIVE_ALT);
	event.active_ctrl = (glutGetModifiers() & GLUT_ACTIVE_CTRL);
	event.active_shift = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
	event.special_key_code = key;
	event.key_pressed = false;
	event.key_released = false;
	event.special_pressed = true;
	event.special_released = false;
	event.x = x;
	event.y = y;
	event.dx = 0;
	event.dy = 0;
	window->event_handler(event);
	event.key_pressed = false;
	event.key_released = false;
	event.special_pressed = false;
	event.special_released = false;
}

void onSpecialUp(int key, int x, int y) {
	event.active_alt = (glutGetModifiers() & GLUT_ACTIVE_ALT);
	event.active_ctrl = (glutGetModifiers() & GLUT_ACTIVE_CTRL);
	event.active_shift = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
	event.special_key_code = key;
	event.key_pressed = false;
	event.key_released = false;
	event.special_pressed = false;
	event.special_released = true;
	event.x = x;
	event.y = y;
	event.dx = 0;
	event.dy = 0;
	window->event_handler(event);
	event.key_pressed = false;
	event.key_released = false;
	event.special_pressed = false;
	event.special_released = false;
}

int last_x, last_y;
void onMouse(int button, int state, int x, int y) {
	last_x = x; last_y = y;

	event.active_alt = (glutGetModifiers() & GLUT_ACTIVE_ALT);
	event.active_ctrl = (glutGetModifiers() & GLUT_ACTIVE_CTRL);
	event.active_shift = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
	event.mouse_left = (button == GLUT_LEFT_BUTTON);
	event.mouse_middle = (button == GLUT_MIDDLE_BUTTON);
	event.mouse_right = (button == GLUT_RIGHT_BUTTON);
	event.mouse_scroll_up = (button == 3);
	event.mouse_scroll_down = (button == 4);
	event.mouse_pressed = (state == GLUT_DOWN);
	event.mouse_released = (state == GLUT_UP);
	event.mouse_left_down = (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON);
	event.mouse_middle_down = (state == GLUT_DOWN && button == GLUT_MIDDLE_BUTTON);
	event.mouse_right_down = (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON);
	event.x = x;
	event.y = y;
	event.dx = 0;
	event.dy = 0;
	window->event_handler(event);
	event.mouse_pressed = false;
	event.mouse_released = false;
}

void onMouseMotion(int x, int y) {
	int dx = x - last_x;
	int dy = y - last_y;
	last_x = x;
	last_y = y;

	event.active_alt = (glutGetModifiers() & GLUT_ACTIVE_ALT);
	event.active_ctrl = (glutGetModifiers() & GLUT_ACTIVE_CTRL);
	event.active_shift = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
	event.mouse_moved = true;
	event.x = x;
	event.y = y;
	event.dx = dx;
	event.dy = dy;
	window->event_handler(event);
	event.mouse_moved = false;
}

float wait_time = 0;
float time_elapsed = 0;
void onIdle() {
	float dt = watch.get_delta_time();
	time_elapsed = time_elapsed + dt;

	window->event_handler(event);

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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glEnable(GLUT_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	window = new GLUI::Window("GLUI", 100, 100, screen_width, screen_height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	onInitialization();

	glutDisplayFunc(onDisplay);
	glutMouseFunc(onMouse);
	glutMotionFunc(onMouseMotion);
	glutPassiveMotionFunc(onMouseMotion);
	glutIdleFunc(onIdle);
	glutKeyboardFunc(onKeyboard);
	glutKeyboardUpFunc(onKeyboardUp);
	glutSpecialFunc(onSpecial);
	glutSpecialUpFunc(onSpecialUp);

	glutMainLoop();

	return 0;
}
