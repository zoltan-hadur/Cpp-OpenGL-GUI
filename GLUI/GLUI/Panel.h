#pragma once

#include "Component.h"

namespace GLUI {

	// Panel, that can store and display various components, also can be moved with the mouse if the panel has a title
	class Panel : public EventListener, public Component {
	private:
		Button* btn_title;	// Title of the panel
		bool dragged;		// Becomes true when the user clicks on the title button, and false when releases the left mouse button
	protected:
		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		// To listen on inner component events
		virtual void action_performed(void* sender, Event& e) override;
		// Title, the coordinates, the size, and the border's width
		Panel(std::string title = "", float x = 0, float y = 0, float width = 100, float height = 100, float border_width = 2);
		// Sets the title of the panel
		void set_title(std::string title);
		// Gets the title of the panel
		std::string get_title();
	};

	void Panel::handle_event(Event& e) {
		if (this->dragged) {														// If dragged
			if (e.mouse_moved) {													// And the mouse moved
				this->set_position(this->get_position() + float2(e.dx, e.dy));		// Reposition the panel
			}
		}
	}

	void Panel::draw(bool draw_background) {
		std::string title = this->btn_title->get_label()->get_text();				// Get the title
		if (title == "" || title == " ") {											// Don't draw it if it's empty
			this->btn_title->set_visible(false);
		}
		Component::draw(draw_background);
	}

	// To listen on inner component events
	void Panel::action_performed(void* sender, Event& e) {
		if (sender == this->btn_title) {		// If the title
			if (e.button_pressed) {				// Was pressed
				this->dragged = true;			// Then the user probably going to drag the panel to somewhere else
			} else if (e.button_released) {		// The user finished
				this->dragged = false;			// Dragging the window
			}
		}
	}

	// Title, what is displayed in the top left corner, the coordinates, the size, and the border's width
	Panel::Panel(std::string title, float x, float y, float width, float height, float border_width) : Component(x, y, width, height, border_width) {
		this->dragged = false;
		this->btn_title = new Button(" "+title, 0, 0, width, 20, border_width);
		this->btn_title->get_label()->set_h_align(Label::H_ALIGN::LEFT);
		this->btn_title->add_event_listener(this);
		this->add_component(this->btn_title);
	}

	// Sets the title of the panel
	void Panel::set_title(std::string title) {
		this->btn_title->get_label()->set_text(title);
	}

	// Gets the title of the panel
	std::string Panel::get_title() {
		return this->btn_title->get_label()->get_text();
	}

}