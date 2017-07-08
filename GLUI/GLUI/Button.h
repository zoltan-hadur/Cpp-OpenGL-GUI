#pragma once

#include <string>
#include "Component.h"
#include "Label.h"
#include "..\Event\ActionEvent.h"
#include "..\Event\ActionPerformer.h"
#include "..\Event\Event.h"
#include "..\Utility\Color.h"
#include "..\Utility\Stopwatch.h"

namespace GLUI {

	// A button with a label inside, that can perform actions when pressed/released
	class Button : public Component {
	protected:
		Label* lbl_text;			// The label inside the button, its aligned mid-mid by default

		Stopwatch watch_wait;		// Timer to watch when should the button start repeating button presses
		Stopwatch watch_repeat;		// Timer to watch when should the button perform the next button press action
		float wait_time;			// Time until repeating button presses after the first button press
		float repeat_time;			// Time between repeated button presses

		bool pressed;				// Becomes true when the button is pressed, and remains true until the button is released
		int x;						// Mouse's x coordinate
		int y;						// Mouse's y coordinate

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

		// Performs a button pressed action
		void press();
		// Performs a button released action
		void release();
		// Performs a button pressed then a button released action
		void trigger();
	};

	void Button::handle_event(Event& e) {
		this->x = e.x;																				// Update the mouse coordinates
		this->y = e.y;

		if (e.mouse_is_inside && !e.mouse_is_covered) {												// If the mouse is above the button and check if covered, because one does not simply press a button if it's behind a component
			this->highlighted = true;																// Then highlight the button
			if (e.mouse_pressed && e.mouse_left && !this->pressed) {								// And if the user pressed the left mouse button
				this->press();																		// Then press the button
				this->watch_wait.start();															// And start the timer for repeated button presses
			} else if (e.mouse_left_down && this->watch_wait.is_running()) {						// Else if the user still holds the left mouse button and if wait watch is running
				if (this->watch_wait.get_elapsed_time() > this->wait_time - this->repeat_time) {	// And if enough time has elapsed to start repeating button presses
					if (!this->watch_repeat.is_running()) {											// And if repeat watch is not running
						this->watch_repeat.start();													// Start it
					} else {																		// Else
						if (this->watch_repeat.get_elapsed_time() > this->repeat_time) {			// If enough time has elapsed after the last repeated button event to start new ones
							this->release();														// When the user clicked on the button, it never had the opportunity to release it
							this->press();															// Thus the release-press process
							this->watch_repeat.reset();												// Reset the repeat watch
						}
					}
				}
			}
		} else {																					// IF the mouse is outside the button
			this->watch_wait.stop();																// Then stop repeating button presses
			this->watch_repeat.stop();
			if (!this->pressed) {
				this->highlighted = false;															// Disable the highlight when the mouse is not above the button, and the button is not currently being pressed
			}
		}
		if (e.mouse_left && e.mouse_released && this->pressed) {									// If the user released the left mouse button
			this->release();																		// Then raise the button
			this->highlighted = false;																// And disable the highlight
			this->watch_wait.stop();																// And stop repeating button presses
			this->watch_repeat.stop();
		}
	}

	void Button::draw(bool draw_background) {
		this->lbl_text->set_position(this->default_border_width, this->default_border_width);							// Dynamically set the label's position to match the button's position
		this->lbl_text->set_size(this->width - this->default_border_width, this->height - this->default_border_width);	// Dynamically set the label's size to match the button's size
		this->border_width = this->pressed ? this->active_border_width : this->default_border_width;					// Border width is bigger when clicked (so animation, much wow)
		Component::draw(draw_background);																				// Draw the base component
	}

	// Label inside the button, the coordinates of the button, the size of the button, and the border's width of the button
	Button::Button(std::string text, float x, float y, float width, float height, float border_width) : Component(x, y, width, height, border_width) {
		this->wait_time = 0.5;										// Default 0.5 sec until repeating button presses
		this->repeat_time = 0.10;									// Default 0.1 sec between repeated button pressed
		this->background_color = Color(120, 120, 120, 255) / 255;	// Default grey color
		this->pressed = false;
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

	// Performs a button pressed action
	void Button::press() {
		if (!this->pressed) {
			this->pressed = true;
			ActionEvent e;
			e.x = this->x;
			e.y = this->y;
			e.button_pressed = true;
			this->perform_action(this, e);
		} else {
			throw "Cannot press a button that's already pressed!";
		}
	}

	// Performs a button released action
	void Button::release() {
		if (this->pressed) {
			this->pressed = false;
			ActionEvent e;
			e.x = this->x;
			e.y = this->y;
			e.button_released = true;
			this->perform_action(this, e);
		} else {
			throw "Cannot release a button that's already released!";
		}
	}

	// Performs a button pressed then a button released action
	void Button::trigger() {
		this->press();
		this->release();
	}

}