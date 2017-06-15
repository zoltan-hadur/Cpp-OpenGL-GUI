#pragma once

#include <GL\freeglut.h>
#include <string>
#include "Component.h"

namespace GLUI {

	class Window : public Component {
	private:
		int id;
	public:
		Window(std::string title = "Window", float pos_x = 100, float pos_y = 100, float width = 960, float height = 540);
		void destroy();
		virtual void draw() override;
	};
	
	Window::Window(std::string title, float pos_x, float pos_y, float width, float height) : Component(0, 0, width, height, 0) {
		glutInitWindowSize(width, height);
		glutInitWindowPosition(pos_x, pos_y);
		id = glutCreateWindow("OpenGL Console Application");
	}

	void Window::destroy() {

	}

	void Window::draw() {

	}

}