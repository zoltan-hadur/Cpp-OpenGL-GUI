#pragma once

#include <GL\freeglut.h>
#include <string>
#include "Component.h"

namespace GLUI {

	// A top level window, created by glut
	class MainWindow : public Component {
	private:
		int id;	// Window id created by glut
	protected:
		virtual float get_width() override;
		virtual float get_height() override;
		virtual void draw(bool draw_background = true) override;
	public:
		MainWindow(std::string title = "MainWindow", float pos_x = 100, float pos_y = 100, float width = 960, float height = 540);
		void destroy();

		virtual void render() override;
		virtual void event_handler(Event& e) override;
	};
	
	float MainWindow::get_width() {
		return glutGet(GLUT_WINDOW_WIDTH);
	}

	float MainWindow::get_height() {
		return glutGet(GLUT_WINDOW_HEIGHT);
	}

	void MainWindow::draw(bool draw_background) {

	}

	MainWindow::MainWindow(std::string title, float pos_x, float pos_y, float width, float height) : Component(0, 0, 0, 0, 0) {
		glutInitWindowSize(width, height);
		glutInitWindowPosition(pos_x, pos_y);
		id = glutCreateWindow(title.c_str());
	}

	void MainWindow::destroy() {

	}

	void MainWindow::render() {
		glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();								// Save current projection matrix
		glOrtho(0.0f, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0.0f, -1.0f, 1.0f);	// Transform it to able to draw in pixel coordinates
		glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();								// Save current modelview matrix
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glEnable(GL_BLEND);						// Enable transparency

		glScissor(0, 0, this->get_width(), this->get_height());
		glEnable(GL_SCISSOR_TEST);
		Component::render();

		glDisable(GL_BLEND);
		glPopMatrix(); glMatrixMode(GL_PROJECTION);
		glPopMatrix(); glMatrixMode(GL_MODELVIEW);
	}

	// Handles input events (keyboard, mouse)
	void MainWindow::event_handler(Event& e) {
		if (e.mouse_left && e.mouse_pressed) {
			this->bring_front(e);
		}
		Component::event_handler(e);
	}

}