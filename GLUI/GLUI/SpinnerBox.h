#pragma once

#include <sstream>
#include <regex>
#include "Component.h"
#include "TextBox.h"
#include "Button.h"
#include "../Stopwatch.h"

namespace GLUI {

	template<typename T> class SpinnerBox : public EventListener, public Component {
	protected:
		Button* btn_left;
		Button* btn_right;
		TextBox* txt_box;

		Stopwatch watch_wait;							// Timer to watch when should the button start repeating button presses
		Stopwatch watch_repeat;							// Timer to watch when should the button raise the next button press event
		float wait_time;								// Time until repeating button presses after the first button press
		float repeat_time;								// Time between repeated button presses

		T min;
		T max;
		T value;
		T increment;

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
		void set_min(T min);
		void set_max(T max);
		void set_value(T value);
		void set_increment(T increment);
		// Gets the wait time
		float get_wait_time();
		// Gets the repeat time
		float get_repeat_time();
		T get_min();
		T get_max();
		T get_value();
		T get_increment();
	};

	// To listen on inner component events
	template<typename T> void SpinnerBox<T>::action_performed(void* sender, Event& e) {
		if (e.button_pressed) {
			if (sender == this->btn_left) {
				this->set_value(this->value - this->increment);
			} else if (sender == this->btn_right) {
				this->set_value(this->value + this->increment);
			}
			this->txt_box->set_text(std::to_string(this->value));
		}
	}

	template<typename T> void SpinnerBox<T>::handle_event(Event& e) {

	}

	template<typename T> void SpinnerBox<T>::draw(bool draw_background) {
		this->btn_left->set_position(this->default_border_width / 2, this->default_border_width / 2);
		this->btn_left->set_size(20, this->height - this->default_border_width);

		this->btn_right->set_position(this->width - 20 - this->default_border_width / 2, this->default_border_width / 2);
		this->btn_right->set_size(20, this->height - this->default_border_width);

		this->txt_box->set_position(20, this->default_border_width / 2);
		this->txt_box->set_size(this->width - 40, this->height - this->default_border_width);

		std::string value = this->txt_box->get_text();
		if (std::regex_match(value, std::regex(R"(^[+-]?(\d+(\.\d+)?|\.\d+)([eE][+-]?\d+)?$)"))) {
			this->txt_box->set_background_color(this->get_background_color());
			T val;
			std::stringstream(value) >> val;
			this->set_value(val);
		} else {
			this->txt_box->set_background_color(255, 0, 0, 255);
		}
		if (!this->txt_box->has_focus()) {
			this->txt_box->set_text(std::to_string(this->value));
		}

		Component::draw(draw_background);
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

	template<typename T> void SpinnerBox<T>::set_min(T min) {
		this->min = std::min(min, this->max);			// Can't be bigger than max
		this->value = std::max(this->value, min);		// Adjust the value if new min is bigger than value
	}

	template<typename T> void SpinnerBox<T>::set_max(T max) {
		this->max = std::max(max, this->min);			// Can't be smaller than min
		this->value = std::min(this->value, max);		// Adjust eh value if new max is smaller than value
	}

	template<typename T> void SpinnerBox<T>::set_value(T value) {
		this->value = std::max(std::min(value, this->max), this->min);	// Must be between min and max
	}

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

	template<typename T> T SpinnerBox<T>::get_min() {
		return this->min;
	}

	template<typename T> T SpinnerBox<T>::get_max() {
		return this->max;
	}

	template<typename T> T SpinnerBox<T>::get_value() {
		return this->value;
	}

	template<typename T> T SpinnerBox<T>::get_increment() {
		return this->increment;
	}

}