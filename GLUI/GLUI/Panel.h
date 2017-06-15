#pragma once

#include "Component.h"

namespace GLUI {

	// Panel, that can store and display various components
	class Panel : public Component {
	public:
		// Title, what is displayed in the top left corner, the coordinates, the size, and the border's width
		Panel(std::string title = "", float x = 0, float y = 0, float width = 100, float height = 100, float border_width = 2);
	};

	// Title, what is displayed in the top left corner, the coordinates, the size, and the border's width
	Panel::Panel(std::string title, float x, float y, float width, float height, float border_width) : Component(x, y, width, height, border_width) {
		this->set_title(title);
	}

}