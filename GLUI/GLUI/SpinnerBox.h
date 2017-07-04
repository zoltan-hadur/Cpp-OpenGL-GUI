#pragma once

#include <sstream>
#include <regex>
#include "Component.h"
#include "TextBox.h"
#include "Button.h"
#include "..\Utility\Stopwatch.h"

namespace GLUI {

	// SpinnerBox that lets you increase/decrease a value in a range, and also lets you to directly edit the value like a text box
	// Use usual types, like char, short, int, long, float, double, etc
	template<typename T> class SpinnerBox : public EventListener, public Component {
	protected:
		Button* btn_left;
		Button* btn_right;
		TextBox* txt_box;

		Stopwatch watch_wait;		// Timer to watch when should the button start repeating button presses
		Stopwatch watch_repeat;		// Timer to watch when should the button raise the next button press event
		float wait_time;			// Time until repeating button presses after the first button press
		float repeat_time;			// Time between repeated button presses

		T min;						// Allowed minimal value
		T max;						// Allowed maximal value
		T value;					// Current value, it's between min and max
		T increment;				// Increases or decreases the value by this value on left or right button press

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		// To listen on inner component events
		virtual void action_performed(void* sender, Event& e) override;

		// Min, max, coordinates, size, border's width
		SpinnerBox(T min, T max, float x = 0, float y = 0, float width = 100, float height = 20, float border_width = 1);

		// Sets the wait time
		void set_wait_time(float wait_time);
		// Sets the repeat time
		void set_repeat_time(float repeat_time);
		// Sets the min value
		void set_min(T min);
		// Sets the max value
		void set_max(T max);
		// Sets the current value
		void set_value(T value);
		// Sets the increment value
		void set_increment(T increment);
		// Gets the wait time
		float get_wait_time();
		// Gets the repeat time
		float get_repeat_time();
		// Gets the min value
		T get_min();
		// Gets the max value
		T get_max();
		// Gets the current calue
		T get_value();
		// Gets the increment value
		T get_increment();
	};

	template<typename T> void SpinnerBox<T>::handle_event(Event& e) {

	}

	template<typename T> void SpinnerBox<T>::draw(bool draw_background) {
		this->btn_left->set_position(0, 0);																// Left button is always on the left side
		this->btn_left->set_size(20, this->height);

		this->btn_right->set_position(this->width - 20, 0);												// Right button is always on the right side
		this->btn_right->set_size(20, this->height);

		this->txt_box->set_position(20-default_border_width, 0);										// Text box is always in the middle
		this->txt_box->set_size(this->width - 40 + default_border_width*2, this->height);

		std::string value = this->txt_box->get_text();													// Current string in the text box
		if (std::regex_match(value, std::regex(R"(^[+-]?(\d+(\.\d+)?|\.\d+)([eE][+-]?\d+)?$)"))) {		// If it's a valid number
			this->txt_box->set_background_color(this->get_background_color());							// Set the color to normal
			T val;
			std::stringstream(value) >> val;															// Read the string into the variable
			this->value = std::max(std::min(val, this->max), this->min);
		} else {
			this->txt_box->set_background_color(255, 0, 0, 255);										// Red background if number is not valid
		}
		if (!this->txt_box->has_focus()) {																// Update the string in the text box according to the current value if it's not focused (because when focused, the user probably typing in it)
			this->txt_box->set_text(std::to_string(this->value));
		}

		Component::draw(draw_background);
	}

	// To listen on inner component events
	template<typename T> void SpinnerBox<T>::action_performed(void* sender, Event& e) {
		if (e.button_pressed) {
			if (sender == this->btn_left) {																// When the left button was pressed
				T old_val = this->value;
				this->set_value(this->value - this->increment);											// Decrease the current value
				e.spinnerbox_changed = old_val != this->value;
				e.spinnerbox_dvalue = old_val - this->value;
				e.spinnerbox_value = this->value;
				this->raise_event(this, e);
				e.spinnerbox_changed = false;
				e.spinnerbox_dvalue = 0;
			} else if (sender == this->btn_right) {														// When the right button was pressed
				T old_val = this->value;
				this->set_value(this->value + this->increment);											// Increase the current value
				e.spinnerbox_changed = old_val != this->value;
				e.spinnerbox_dvalue = old_val - this->value;
				e.spinnerbox_value = this->value;
				this->raise_event(this, e);
				e.spinnerbox_changed = false;
				e.spinnerbox_dvalue = 0;
			}
			this->txt_box->set_text(std::to_string(this->value));										// Update the text box's string
		} else if (sender == this->txt_box) {
			this->raise_event(this, e);
		}
	}

	// Min, max, coordinates, size, border's width
	template<typename T> SpinnerBox<T>::SpinnerBox<T>(T min, T max, float x, float y, float width, float height, float border_width) : Component(x, y, width, height, border_width) {
		this->min = min;
		this->max = max;
		this->value = min;
		this->increment = T(1);

		this->btn_left = new Button("<");
		this->btn_right = new Button(">");
		this->txt_box = new TextBox(TextBox::TYPE::SINGLE_LINE);

		this->btn_left->add_event_listener(this);
		this->btn_right->add_event_listener(this);
		this->txt_box->add_event_listener(this);

		this->add_component(this->btn_left);
		this->add_component(this->btn_right);
		this->add_component(this->txt_box);

		this->set_wait_time(0.5);
		this->set_repeat_time(0.01);

		this->txt_box->set_text(std::to_string(this->value));

		this->background_color = Color(120, 120, 120, 255);
	}

	// Sets the wait time
	template<typename T> void SpinnerBox<T>::set_wait_time(float wait_time) {
		this->btn_left->set_wait_time(wait_time);
		this->btn_right->set_wait_time(wait_time);
		this->wait_time = this->btn_left->get_wait_time();
	}

	// Sets the repeat time
	template<typename T> void SpinnerBox<T>::set_repeat_time(float repeat_time) {
		this->btn_left->set_repeat_time(repeat_time);
		this->btn_right->set_repeat_time(repeat_time);
		this->repeat_time = this->btn_left->get_repeat_time();
	}

	// Sets the min value
	template<typename T> void SpinnerBox<T>::set_min(T min) {
		this->min = std::min(min, this->max);			// Can't be bigger than max
		this->value = std::max(this->value, min);		// Adjust the value if new min is bigger than value
	}

	// Sets the max value
	template<typename T> void SpinnerBox<T>::set_max(T max) {
		this->max = std::max(max, this->min);			// Can't be smaller than min
		this->value = std::min(this->value, max);		// Adjust eh value if new max is smaller than value
	}

	// Sets the current value (will be between min and max)
	template<typename T> void SpinnerBox<T>::set_value(T value) {
		this->value = std::max(std::min(value, this->max), this->min);	// Must be between min and max
		this->txt_box->set_text(std::to_string(this->value));
	}

	// Sets the increment (min 0)
	template<typename T> void SpinnerBox<T>::set_increment(T increment) {
		this->increment = std::max(increment, T(0));	// Can't be smaller than 0
	}

	// Gets the wait time
	template<typename T> float SpinnerBox<T>::get_wait_time() {
		return this->wait_time;
	}

	// Gets the repeat time
	template<typename T> float SpinnerBox<T>::get_repeat_time() {
		return this->repeat_time;
	}

	// Gets the min value
	template<typename T> T SpinnerBox<T>::get_min() {
		return this->min;
	}

	// Gets the max value
	template<typename T> T SpinnerBox<T>::get_max() {
		return this->max;
	}

	// Gets the current value
	template<typename T> T SpinnerBox<T>::get_value() {
		return this->value;
	}

	// Gets the increment value
	template<typename T> T SpinnerBox<T>::get_increment() {
		return this->increment;
	}

}