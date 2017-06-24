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
	GLUI::Button* btn_next_anim;
	GLUI::ProgressBar* bar;

	int value;
	int anim_id;
public:
	ASD(GLUI::Button* start, GLUI::Button* progress, GLUI::Button* stop, GLUI::Button* anim, GLUI::ProgressBar* bar) {
		this->value = 0;
		this->anim_id = 0;

		this->btn_start_progress = start;
		this->btn_progress = progress;
		this->btn_stop_progress = stop;
		this->btn_next_anim = anim;
		this->bar = bar;
		this->bar->set_min(0);
		this->bar->set_max(13);
		this->bar->set_animation(GLUI::ProgressBar::ANIMATION::TEXT_DOTS);
	}
	void action_performed(void* sender, Event& e) override {
		if (e.button_pressed) {
			if (sender == btn_start_progress && !e.mouse_covered) {
				bar->start_progress();
				this->value = 0;
			}
			if (sender == btn_progress && !e.mouse_covered) {
				bar->set_value(++value);
				if (value == 13) {
					bar->stop_progress();
				}
			}
			if (sender == btn_stop_progress && !e.mouse_covered) {
				bar->stop_progress();
			}
			if (sender == btn_next_anim && !e.mouse_covered) {
				this->anim_id = ++this->anim_id % 4;
				switch (this->anim_id) {
					case 0:
						this->bar->set_animation(GLUI::ProgressBar::ANIMATION::TEXT_DOTS);
						break;
					case 1:
						this->bar->set_animation(GLUI::ProgressBar::ANIMATION::TEXT_SPINNING);
						break;
					case 2:
						this->bar->set_animation(GLUI::ProgressBar::ANIMATION::STRIPES);
						break;
					case 3:
						this->bar->set_animation(GLUI::ProgressBar::ANIMATION::WINDOWS);
						break;
				}
			}
		}
	}
};

void on_initialization() {
	glViewport(0, 0, screen_width, screen_height);

	max_fps = 0;

	GLUI::Window* window_0 = new GLUI::Window("Window 0", 10, 10, 800, 500);
	GLUI::Window* window_1 = new GLUI::Window("Window 1", 110, 110, 800, 500);
	GLUI::Window* window_10 = new GLUI::Window("ASD", 210, 210, 800, 500);
	GLUI::Window* window_11 = new GLUI::Window("A very long window name", 310, 310, 800, 500);
	window->add_component(window_0);
	window->add_component(window_1);
	window->add_component(window_10);
	window->add_component(window_11);

	GLUI::Window* window_2 = new GLUI::Window("Window 2", 10, 30, 300, 300);
	window_2->set_border_color(255, 0, 0, 255);
	GLUI::Window* window_3 = new GLUI::Window("Window 3", 10, 30, 300, 300);
	window_1->add_component(window_2);
	//window_1->add_component(window_3);

	GLUI::TabbedPanel* tabbed_panel = new GLUI::TabbedPanel(20, 40, 600, 400);
	window_0->add_component(tabbed_panel);
	//window->add_component(tabbed_panel);

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

	GLUI::ComboBox* cbox = new GLUI::ComboBox(10, 10, 200, 200);
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

	GLUI::Label* label = new GLUI::Label("CheckBox", 230, 10, 20, 20);
	tab2->add_component(label);

	GLUI::CheckBox* check_box = new GLUI::CheckBox(310, 10, 20, 20);
	tab2->add_component(check_box);

	GLUI::TextBox* text_box0 = new GLUI::TextBox(GLUI::TextBox::TYPE::MULTI_LINE, 340, 10, 250, 100);
	tab2->add_component(text_box0);

	GLUI::TextBox* text_box1 = new GLUI::TextBox(GLUI::TextBox::TYPE::SINGLE_LINE, 340, 120, 250, 20);
	tab2->add_component(text_box1);

	GLUI::ProgressBar* bar = new GLUI::ProgressBar(0, 100, 10, 230, 500, 20, 1);
	tab2->add_component(bar);

	GLUI::Button* btn_start_progress = new GLUI::Button("Start progress", 10, 260, 150, 20);
	GLUI::Button* btn_progress = new GLUI::Button("Progress", 185, 260, 150, 20);
	GLUI::Button* btn_stop_progress = new GLUI::Button("Stop progress", 360, 260, 150, 20);
	GLUI::Button* btn_next_anim = new GLUI::Button("Next animation", 10, 200, 150, 20);
	tab2->add_component(btn_start_progress);
	tab2->add_component(btn_progress);
	tab2->add_component(btn_stop_progress);
	tab2->add_component(btn_next_anim);

	ASD* asd = new ASD(btn_start_progress, btn_progress, btn_stop_progress, btn_next_anim, bar);
	btn_start_progress->add_event_listener(asd);
	btn_progress->add_event_listener(asd);
	btn_stop_progress->add_event_listener(asd);
	btn_next_anim->add_event_listener(asd);

	GLUI::SpinnerBox<int>* spb_spinner = new GLUI::SpinnerBox<int>(0, 255, 10, 290, 200, 20);
	tab2->add_component(spb_spinner);

	tabbed_panel->add_tab("long tab name 0");
	tabbed_panel->get_tab("long tab name 0")->add_component(window_3);
	tabbed_panel->add_tab("short 1");
	tabbed_panel->add_tab("short 2");

	watch.start();
}

void on_keyboard_down(unsigned char key, int x, int y) {
	window->on_keyboard_down(key, x, y);
}

void on_keyboard_up(unsigned char key, int x, int y) {
	window->on_keyboard_up(key, x, y);
}

void on_special_down(int key, int x, int y) {
	window->on_special_down(key, x, y);
}

void on_special_up(int key, int x, int y) {
	window->on_special_up(key, x, y);
}

void on_mouse(int button, int state, int x, int y) {
	window->on_mouse(button, state, x, y);
}

void on_mouse_motion(int x, int y) {
	window->on_mouse_motion(x, y);
}

void on_mouse_motion_passive(int x, int y) {
	window->on_mouse_motion_passive(x, y);
}

void on_display() {
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glBegin(GL_QUADS);
	//for (int i = 0; i < 10; ++i) {
	//	for (int j = 0; j < 10; ++j) {
	//		float x = -1.0f + 2.0f / 10.0f * i;
	//		float y = -1.0f + 2.0f / 10.0f * j;
	//		float w = 2.0f / 10.0f;
	//		float h = 2.0f / 10.0f;

	//		if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
	//			glColor3f(1, 0, 0);
	//			glVertex2f(x, y);

	//			glColor3f(0, 1, 0);
	//			glVertex2f(x, y + h);

	//			glColor3f(1, 0, 0);
	//			glVertex2f(x + w, y + h);

	//			glColor3f(0, 1, 0);
	//			glVertex2f(x + w, y);
	//		} else {
	//			glColor3f(0, 1, 0);
	//			glVertex2f(x, y);

	//			glColor3f(1, 0, 0);
	//			glVertex2f(x, y + h);

	//			glColor3f(0, 1, 0);
	//			glVertex2f(x + w, y + h);

	//			glColor3f(1, 0, 0);
	//			glVertex2f(x + w, y);
	//		}
	//	}
	//}
	//glEnd();

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
				glVertex2f(x, y + h);
				glVertex2f(x + w, y + h);
				glVertex2f(x + w, y);
			} else {
				glColor3f(0, 1, 0);
				glVertex2f(x, y);
				glVertex2f(x, y + h);
				glVertex2f(x + w, y + h);
				glVertex2f(x + w, y);
			}
		}
	}
	glEnd();

	window->render();

	glutSwapBuffers();
}

float wait_time = 0;
float time_elapsed = 0;
void on_idle() {
	float dt = watch.get_delta_time();
	time_elapsed = time_elapsed + dt;

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

	on_initialization();

	glutKeyboardFunc(on_keyboard_down);
	glutKeyboardUpFunc(on_keyboard_up);
	glutSpecialFunc(on_special_down);
	glutSpecialUpFunc(on_special_up);
	glutMouseFunc(on_mouse);
	glutMotionFunc(on_mouse_motion);
	glutPassiveMotionFunc(on_mouse_motion_passive);
	glutDisplayFunc(on_display);
	glutIdleFunc(on_idle);

	glutMainLoop();

	return 0;
}
