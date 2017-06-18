#pragma once

#include "Component.h"
#include "../Stopwatch.h"

namespace GLUI {

	class Slider : public EventListener, public Component {
	protected:
		enum class ALIGN;				// Forward declaration

		ALIGN align;					// Determines the alignment, either horizontal of vertical
		Button* btn_left;				// Button on the left when horizontal
		Button* btn_right;				// Button on the right when horizontal
		Button* btn_indicator;			// Button between left and right, the slider itself
		Stopwatch watch;				// Timer to watch when should the indicator start moving towards the mouse
		float wait_time;				// Time until the indicator starts to move towards the mouse
		float speed;					// Determines how fast the indicator moves towards the mouse
		bool dragged;					// Becomes true when the user clicks on the indicator button, and false when releases the left mouse button
		float min;						// The minimum value that the slider can hold
		float max;						// The maximum value that the slider can hold
		float value;					// The value that the slider holds, between min and max
		float increment;				// Determines how much the value changes
		float2 indicator_pos_offset;	// For moving the indicator with the mouse

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		enum class ALIGN {
			HORIZONTAL,					// Horizontal alignment
			VERTICAL					// Vertical alignment
		};

		// To listen on inner component events
		virtual void action_performed(void* sender, Event& e) override;

		// Min, max, coordinates, size, border's width
		Slider(float min = 0, float max = 1, float x = 0, float y = 0, float width = 100, float height = 20, float border_width = 1);
		// Increases the value defined by increment
		void inc_value();
		// Decreases the value defined by increment
		void dec_value();
		// Sets the min
		void set_min(float min);
		// Sets the max
		void set_max(float max);
		// Sets the value (will be between min and max)
		void set_value(float value);
		// Sets the increments
		void set_increment(float increment);
		// Sets the wait time
		void set_wait_time(float wait_time);
		// Sets the speed
		void set_speed(float speed);
		// Gets the min
		float get_min();
		// Gets the max
		float get_max();
		// Gets the value
		float get_value();
		// Gets the increment
		float get_increment();
		// Gets the wait time
		float get_wait_time();
		// Gets the speed
		float get_speed();
	};

	// To listen on inner component events
	void Slider::action_performed(void* sender, Event& e) {
		if (e.button_pressed) {																			// Check if any component button was pressed
			if (sender == this->btn_left) {																// If the left button was pressed,
				this->dec_value();																		// Decrease the value
			} else if (sender == this->btn_right) {														// If the right button was pressed,
				this->inc_value();																		// Increase the value
			} else if (sender == this->btn_indicator) {													// If the indicator button was pressed
				this->dragged = true;																	// The user possibly wants to drag it with the mouse
				float2 pos = this->get_position();
				this->indicator_pos_offset = float2(e.x, e.y) - this->btn_indicator->get_position();	// Thus the offset
			}
		}
	}

	void Slider::handle_event(Event& e) {
		float old_val = this->value;																	// To determine at the end of this method if the value has changed
		float2 pos = this->get_absolute_position();
		float2 pos_offset = (this->align == ALIGN::HORIZONTAL) ? float2(15, 0) : float2(0, 15);			// The left and right buttons has a 15 width/height according to the alignment

		if (this->dragged) {																			// If the indicator is dragged
			float2 pos = float2(e.x, e.y) - this->indicator_pos_offset;
			switch (this->align) {																		// Move it according to the mouse positios
				case ALIGN::HORIZONTAL:
				{
					pos.x = std::min(pos.x, (float)(this->width - 15 - 20));							// The indicator has a 20 width/height according to the alignment
					pos.x = std::max(pos.x, (float)15);
					this->value = (this->max - this->min)*(pos.x - 15) / (this->width - 50);
					break;
				}
				case ALIGN::VERTICAL:
				{
					pos.y = std::min(pos.y, (float)(this->height - 15 - 20));
					pos.y = std::max(pos.y, (float)15);
					this->value = (this->max - this->min)*(pos.y - 15) / (this->height - 50);
					break;
				}
			}
			// This long one checks if the mouse is above the slider, and it's position is between the left and right buttons
		} else if (pos.x + pos_offset.x < e.x && e.x < pos.x + this->width - pos_offset.x && pos.y + pos_offset.y < e.y && e.y < pos.y + this->height - pos_offset.y) {
			if (e.mouse_left && e.mouse_pressed) {																					// Start a watch
				this->watch.start();																								// If the user pressed the left mouse button
			} else if (e.mouse_left_down) {																							// If the user still holds the left mouse button down
				if (this->watch.is_running()) {
					float dt = this->watch.get_delta_time() * this->speed;
					if (this->watch.get_elapsed_time() > this->wait_time) {															// If enough time elapsed for the indicator to starts moving towards the mouse
						switch (this->align) {
							case ALIGN::HORIZONTAL:
							{
								if (e.x < this->btn_indicator->get_absolute_position().x) {
									this->set_value(this->get_value() - dt);
								} else if (e.x > this->btn_indicator->get_absolute_position().x + this->btn_indicator->get_width()) {
									this->set_value(this->get_value() + dt);
								}
								break;
							}
							case ALIGN::VERTICAL:
							{
								if (e.y < this->btn_indicator->get_absolute_position().y) {
									this->set_value(this->get_value() - dt);
								} else if (e.y > this->btn_indicator->get_absolute_position().y + this->btn_indicator->get_height()) {
									this->set_value(this->get_value() + dt);
								}
								break;
							}
						}
					}
				}
			} else if (e.mouse_left && e.mouse_released) {	// The indicator not moving towards to the mouse
				this->dragged = false;
				this->watch.stop();
			}
		} else {											// The indicator not moving towards to the mouse
			this->watch.stop();
		}
		if (!e.mouse_left_down) {							// The indicator not moving towards to the mouse
			this->dragged = false;
			this->watch.stop();
		}
		if (old_val - this->value != 0) {
			e.slider_value = this->value;
			e.slider_dvalue = old_val - this->value;
			this->raise_event(this, e);						// Raise an event if the value has changed
			e.slider_dvalue = 0;
		}
	}

	void Slider::draw(bool draw_background) {
		switch (this->align) {
			case ALIGN::HORIZONTAL:
			{
				this->btn_left->get_label()->set_text("<");
				this->btn_left->set_size(16, this->height);
				this->btn_left->set_position(0, 0);

				this->btn_right->get_label()->set_text(">");
				this->btn_right->set_size(16, this->height);
				this->btn_right->set_position(this->width - 16, 0);

				this->btn_indicator->get_label()->set_text("|");
				this->btn_indicator->set_size(20, this->height);
				this->btn_indicator->set_position(15 + (this->width - 50)*(this->value + this->min) / (this->max - this->min), 0);
				break;
			}
			case ALIGN::VERTICAL:
			{
				this->btn_right->get_label()->set_text(R"(\/)");
				this->btn_right->set_size(this->width, 16);
				this->btn_right->set_position(0, this->height - 16);

				this->btn_left->get_label()->set_text(R"(/\)");
				this->btn_left->set_size(this->width, 16);
				this->btn_left->set_position(0, 0);

				this->btn_indicator->get_label()->set_text("-");
				this->btn_indicator->set_size(this->width, 20);
				this->btn_indicator->set_position(0, 15 + (this->height - 50)*(this->value + this->min) / (this->max - this->min));
				break;
			}
		}

		Component::draw();

		// Draw the dashed lines
		//float2 pos = this->get_absolute_position() + float2(this->default_border_width, this->default_border_width);
		//glColor4f(1, 1, 1, 0.5);
		//glLineWidth(1);
		//glLineStipple(1, 0b0101010101010101);
		//glEnable(GL_LINE_STIPPLE);
		//glBegin(GL_LINES);
		//for (int i = 0; i < this->width - this->default_border_width; i = i + 2) {
		//	glVertex2f(pos.x + i, pos.y + (i / 2) % 2);
		//	glVertex2f(pos.x + i, pos.y + (i / 2) % 2 + this->height - this->default_border_width * 2);
		//}
		//glEnd();
		//glDisable(GL_LINE_STIPPLE);
	}

	// Min, max, coordinates, size, border's width
	Slider::Slider(float min, float max, float x, float y, float width, float height, float border_width) : Component(x, y, width, height, border_width) {
		this->wait_time = 0.0;
		this->speed = 200;

		this->min = min;
		this->max = max;
		this->value = min;
		this->increment = (max - min) / 10;
		this->dragged = false;

		this->btn_left = new Button();
		this->btn_right = new Button();
		this->btn_indicator = new Button();
		this->add_component(this->btn_left);
		this->add_component(this->btn_right);
		this->add_component(this->btn_indicator);
		this->btn_left->add_event_listener(this);
		this->btn_right->add_event_listener(this);
		this->btn_indicator->add_event_listener(this);

		if (height <= width) {
			this->align = ALIGN::HORIZONTAL;
		} else {
			this->align = ALIGN::VERTICAL;
		}
	}

	// Increases the value defined by increment
	void Slider::inc_value() {
		float old_val = this->value;
		this->value = std::min(this->value + this->increment, this->max);	// Can't be bigger than max
		Event e;
		e.slider_value = this->value;
		e.slider_dvalue = old_val - this->value;
		this->raise_event(this, e);
	}

	// Decreases the value defined by increment
	void Slider::dec_value() {
		float old_val = this->value;
		this->value = std::max(this->value - this->increment, this->min);	// Can't be smaller than min
		Event e;
		e.slider_value = this->value;
		e.slider_dvalue = old_val - this->value;
		this->raise_event(this, e);
	}

	// Sets the min
	void Slider::set_min(float min) {
		this->min = std::min(min, this->max);			// Can't be bigger than max
		this->value = std::max(this->value, min);		// Adjust the value if new min is bigger than value
	}

	// Sets the max
	void Slider::set_max(float max) {
		this->max = std::max(max, this->min);			// Can't be smaller than min
		this->value = std::min(this->value, max);		// Adjust eh value if new max is smaller than value
	}

	// Sets the value (will be between min and max)
	void Slider::set_value(float value) {
		this->value = std::max(std::min(value, this->max), this->min);	// Must be between min and max
	}

	// Sets the increment
	void Slider::set_increment(float increment) {
		this->increment = std::max(increment, 1.0f);	// Can't be smaller than 1
	}

	// Sets the wait time
	void Slider::set_wait_time(float wait_time) {
		this->wait_time = std::max(wait_time, 0.0f);	// Can't be smaller than 0
	}

	// Sets the speed
	void Slider::set_speed(float speed) {
		this->speed = std::max(speed, 10.0f);			// Can't be smaller than 10
	}

	// Gets the min
	float Slider::get_min() {
		return this->min;
	}

	// Gets the max
	float Slider::get_max() {
		return this->max;
	}

	// Gets the value
	float Slider::get_value() {
		return this->value;
	}

	// Gets the increment
	float Slider::get_increment() {
		return this->increment;
	}

	// Gets the wait time
	float Slider::get_wait_time() {
		return this->wait_time;
	}

	// Gets the speed
	float Slider::get_speed() {
		return this->speed;
	}

}