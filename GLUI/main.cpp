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
GLUI::MainWindow* window;
Event event;

class ASD : public EventListener {
private:
	GLUI::Button* btn_start_progress;
	GLUI::Button* btn_progress;
	GLUI::Button* btn_stop_progress;
	GLUI::ProgressBar* bar;
	int value;
public:
	ASD(GLUI::Button* start, GLUI::Button* progress, GLUI::Button* stop, GLUI::ProgressBar* bar) {
		this->value = 0;

		this->btn_start_progress = start;
		this->btn_progress = progress;
		this->btn_stop_progress = stop;
		this->bar = bar;
		this->bar->set_min(0);
		this->bar->set_max(13);
	}
	void action_performed(void* sender, Event& e) override {
		if (e.button_pressed) {
			if (sender == btn_start_progress) {
				bar->start_progress();
				this->value = 0;
			}
			if (sender == btn_progress) {
				bar->set_value(++value);
				if (value == 13) {
					bar->stop_progress();
				}
			}
			if (sender == btn_stop_progress) {
				bar->stop_progress();
			}
		}
	}
};

void onInitialization() {
	glViewport(0, 0, screen_width, screen_height);

	max_fps = 0;

	GLUI::Window* window_0 = new GLUI::Window("Window 0", 10, 10, 800, 500);
	GLUI::Window* window_1 = new GLUI::Window("Window 1", 500, 10, 800, 500);
	window->add_component(window_0);
	//window->add_component(window_1);

	GLUI::TabbedPanel* tabbed_panel = new GLUI::TabbedPanel(20, 40, 600, 400);
	window_0->add_component(tabbed_panel);

	GLUI::ScrollPanel* panel0 = new GLUI::ScrollPanel(GLUI::ScrollPanel::ALIGN::VERTICAL, 10, 10, 500, 350);
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 20; ++j) {
			panel0->add_component(new GLUI::Button("B" + std::to_string(j * 6 + i), 10 + 75 * i, 10 + 25 * j, 70, 20));
		}
	}
	tabbed_panel->add_tab("Tab names", panel0);

	GLUI::ScrollPanel* panel1 = new GLUI::ScrollPanel(GLUI::ScrollPanel::ALIGN::HORIZONTAL, 10, 10, 500, 350);
	for (int i = 0; i < 12; ++i) {
		for (int j = 0; j < 12; ++j) {
			panel1->add_component(new GLUI::Button("B" + std::to_string(j * 12 + i), 10 + 75 * i, 10 + 25 * j, 70, 20));
		}
	}
	tabbed_panel->add_tab("has", panel1);

	tabbed_panel->add_tab("automatic length");
	GLUI::Panel* tab2 = tabbed_panel->get_tab("automatic length");

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

	//GLUI::Label* label = new GLUI::Label("CheckBox", 230, 10, 20, 20);
	//tab2->add_component(label);

	//GLUI::CheckBox* check_box = new GLUI::CheckBox(310, 10, 20, 20);
	//tab2->add_component(check_box);

	//GLUI::TextBox* text_box0 = new GLUI::TextBox(GLUI::TextBox::TYPE::MULTI_LINE, 340, 10, 250, 100);
	//tab2->add_component(text_box0);

	//GLUI::TextBox* text_box1 = new GLUI::TextBox(GLUI::TextBox::TYPE::SINGLE_LINE, 340, 120, 250, 20);
	//tab2->add_component(text_box1);

	//GLUI::ProgressBar* bar = new GLUI::ProgressBar(0, 100, 10, 230, 500, 20, 1);
	//tab2->add_component(bar);

	//GLUI::Button* btn_start_progress = new GLUI::Button("Start progress", 10, 260, 150, 20);
	//GLUI::Button* btn_progress = new GLUI::Button("Progress", 185, 260, 150, 20);
	//GLUI::Button* btn_stop_progress = new GLUI::Button("Stop progress", 360, 260, 150, 20);
	//tab2->add_component(btn_start_progress);
	//tab2->add_component(btn_progress);
	//tab2->add_component(btn_stop_progress);

	//ASD* asd = new ASD(btn_start_progress, btn_progress, btn_stop_progress, bar);
	//btn_start_progress->add_event_listener(asd);
	//btn_progress->add_event_listener(asd);
	//btn_stop_progress->add_event_listener(asd);

	GLUI::SpinnerBox<int>* spb_spinner = new GLUI::SpinnerBox<int>(0, 255, 10, 290, 200, 20);
	tab2->add_component(spb_spinner);

	watch.start();
}

void onDisplay() {
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_QUADS);
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			float x = -1.0f + 2.0f / 10.0f * i;
			float y = -1.0f + 2.0f / 10.0f * j;
			float w = 2.0f / 10.0f;
			float h = 2.0f / 10.0f;

			if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
				glColor3f(1, 0, 0);
				glVertex2f(x, y);

				glColor3f(0, 1, 0);
				glVertex2f(x, y + h);

				glColor3f(1, 0, 0);
				glVertex2f(x + w, y + h);

				glColor3f(0, 1, 0);
				glVertex2f(x + w, y);
			} else {
				glColor3f(0, 1, 0);
				glVertex2f(x, y);

				glColor3f(1, 0, 0);
				glVertex2f(x, y + h);

				glColor3f(0, 1, 0);
				glVertex2f(x + w, y + h);

				glColor3f(1, 0, 0);
				glVertex2f(x + w, y);
			}
		}
	}
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
	event.dx = 0;
	event.dy = 0;
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

	window = new GLUI::MainWindow("MainWindow", 100, 100, screen_width, screen_height);

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
