#pragma once

#include <GL\freeglut.h>
#include <string>
#include "Component.h"

namespace GLUI {

	class MainWindow : public Component {
	private:
		int id;
	protected:
		virtual float get_width() override;
		virtual float get_height() override;
	public:
		MainWindow(std::string title = "MainWindow", float pos_x = 100, float pos_y = 100, float width = 960, float height = 540);
		void destroy();
		virtual void draw(bool draw_background = true) override;
		virtual void event_handler(Event& e) override;
	};
	
	float MainWindow::get_width() {
		return glutGet(GLUT_WINDOW_WIDTH);
	}

	float MainWindow::get_height() {
		return glutGet(GLUT_WINDOW_HEIGHT);
	}

	MainWindow::MainWindow(std::string title, float pos_x, float pos_y, float width, float height) : Component(0, 0, width, height, 0) {
		glutInitWindowSize(width, height);
		glutInitWindowPosition(pos_x, pos_y);
		id = glutCreateWindow(title.c_str());
	}

	void MainWindow::destroy() {

	}

	void MainWindow::draw(bool draw_background) {

	}

	// Handles input events (keyboard, mouse)
	void MainWindow::event_handler(Event& e) {
		if (e.mouse_left && e.mouse_pressed) {
			this->bring_front(e);
		}
		Component::event_handler(e);
	}

}