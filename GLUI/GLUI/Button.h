#pragma once

#include "Component.h"
#include "Label.h"
#include "../Stopwatch.h"

namespace GLUI {

	// A button with a label inside, that can raise events when pressed/released
	class Button : public Component {
	protected:
		bool clicked;									// Becomes true when the user clicks on a button, and remains true until the user released the left mouse button
		Label* lbl_text;								// The label inside the button, its aligned mid-mid by default
		Stopwatch watch_wait;							// Timer to watch when should the button start repeating button presses
		Stopwatch watch_repeat;							// Timer to watch when should the button raise the next button press event
		float wait_time;								// Time until repeating button presses after the first button press
		float repeat_time;								// Time between repeated button presses

		virtual void press_button(Event& e);			// Raises a button pressed event
		virtual void release_button(Event& e);			// Raises a button released event
		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		// Label inside the button, the coordinates of the button, the size of the button, and the border's width of the button
		Button(std::string text = "Button", float x = 0, float y = 0, float width = 100, float height = 20, float border_width = 1);
		// Sets the wait time
		void set_wait_time(float wait_time);
		// Sets the repeat time
		void set_repeat_time(float repeat_time);
		// Gets the wait time
		float get_wait_time();
		// Gets the repeat time
		float get_repeat_time();
		// Returns the label
		Label* get_label();
	};

	// Raises a button pressed event
	void Button::press_button(Event& e) {
		this->clicked = true;
		e.button_pressed = true;
		e.button_released = false;
		this->raise_event(this, e);							// Raise an event that the button was pressed
		e.button_pressed = false;
		e.button_released = false;
		this->watch_wait.start();							// Start waiting for repeated button presses
	}

	// Raises a button released event
	void Button::release_button(Event& e) {
		this->clicked = false;
		e.button_pressed = false;
		e.button_released = true;
		this->raise_event(this, e);							// Raise an event that the button was released
		e.button_pressed = false;
		e.button_released = false;
	}

	void Button::handle_event(Event& e) {
		float2 pos = this->get_absolute_position();															// The absolute position relative to the top-level window
		if (pos.x < e.x && e.x < pos.x + this->width && pos.y < e.y && e.y < pos.y + this->height) {		// Check if the mouse is above the button
			if (!e.mouse_covered) {																			// Check if covered, because one does not simply press a button if it's behind a component
				if (e.mouse_left && e.mouse_pressed & !this->clicked) {										// Check if the user clicked on the button with the left mouse button
					this->press_button(e);																	// Then press the button
				} else if (e.mouse_left_down) {																// Check if the user still holds the left mouse button after down
					if (this->watch_wait.is_running()) {													// Check if wait watch is running
						if (this->watch_wait.get_elapsed_time() > this->wait_time - this->repeat_time) {	// Check if enough time has elapsed to start repeating button presses
							if (!this->watch_repeat.is_running()) {											// Check if repeat watch is running
								this->watch_repeat.start();													// Start it if not running
							} else {
								if (this->watch_repeat.get_elapsed_time() > this->repeat_time) {			// Check if enough time has elapsed after the last repeated button event to start new ones
									this->release_button(e);												// When the user clicked on the button, it never had the opportunity to release it
									this->press_button(e);													// Thus the release-press process
									this->watch_repeat.reset();												// Reset the repeat watch
								}
							}
						}
					}
				} else {
					this->watch_wait.stop();																// Stop repeating events if the user is not holding the left mouse button down anymore
					this->watch_repeat.stop();
				}
			}
			this->highlighted = !e.mouse_covered;															// Highlight the button if the mouse is above the button
		} else {
			this->watch_wait.stop();																		// Stop repeating events if the mouse is not above the button
			this->watch_repeat.stop();
			if (!this->clicked) {
				this->highlighted = false;																	// Disable the highlight when the mouse is not above the button, and the button is not currently being pressed
			}
		}
		if (e.mouse_left && e.mouse_released && this->clicked) {											// Check if the user released the left mouse button
			this->release_button(e);																		// Then raise the button
		}
	}

	void Button::draw(bool draw_background) {
		this->lbl_text->set_position(this->default_border_width, this->default_border_width);							// Dynamically set the label's position to match the button's position
		this->lbl_text->set_size(this->width - this->default_border_width, this->height - this->default_border_width);	// Dynamically set the label's size to match the button's size
		this->border_width = this->clicked ? this->active_border_width : this->default_border_width;					// Border width is bigger when clicked (so animation, much wow)
		Component::draw();																								// Draw the base component
	}

	// Label inside the button, the coordinates of the button, the size of the button, and the border's width of the button
	Button::Button(std::string text, float x, float y, float width, float height, float border_width) : Component(x, y, width, height, border_width) {
		this->wait_time = 0.5;									// Default 0.5 sec until repeating button presses
		this->repeat_time = 0.10;								// Default 0.1 sec between repeated button pressed
		this->background_color = Color(120, 120, 120, 255);		// Default grey color
		this->clicked = false;
		this->lbl_text = new Label(text);
		this->add_component(this->lbl_text);
	}

	// Sets the wait time (minimum is 0.25 sec)
	void Button::set_wait_time(float wait_time) {
		this->wait_time = std::max(wait_time, 0.25f);
	}

	// Sets the repeat time (minimum is 0 sec)
	void Button::set_repeat_time(float repeat_time) {
		this->repeat_time = std::max(repeat_time, 0.0f);
	}

	// Gets the wait time
	float Button::get_wait_time() {
		return this->wait_time;
	}

	// Gets the repeat time
	float Button::get_repeat_time() {
		return this->repeat_time;
	}

	// Returns the label
	Label* Button::get_label() {
		return this->lbl_text;
	}

}