#pragma once

#include "Component.h"
#include "Button.h"
#include "..\Event\ActionEvent.h"
#include "..\Event\ActionListener.h"
#include "..\Event\ActionPerformer.h"
#include "..\Event\Event.h"
#include "..\Utility\float2.h"
#include "..\Utility\Stopwatch.h"

namespace GLUI {

	// Slider / ScrollBar
	class Slider : public ActionListener, public Component {
	private:
		float calc_percent();
	protected:
		enum class ALIGN;				// Forward declaration

		ALIGN align;					// Determines the alignment, either horizontal of vertical

		Button* btn_left;				// Button on the left when horizontal
		Button* btn_right;				// Button on the right when horizontal
		Button* btn_indicator;			// Button between left and right, the slider itself

		float2 indicator_pos_offset;	// For moving the indicator with the mouse

		bool dragged;					// Becomes true when the user clicks on the indicator button, and false when releases the left mouse button

		Stopwatch watch;				// Timer to watch when should the indicator start moving towards the mouse
		float wait_time;				// Time until the indicator starts to move towards the mouse
		float speed;					// Determines how fast the indicator moves towards the mouse

		float min;						// The minimum value that the slider can hold
		float max;						// The maximum value that the slider can hold
		float value;					// The value that the slider holds, between min and max
		float increment;				// Determines how much the value changes

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		enum class ALIGN {
			HORIZONTAL,					// Horizontal alignment
			VERTICAL					// Vertical alignment
		};

		// To listen on inner component actions
		virtual void action_performed(void* sender, ActionEvent& e) override;

		// Min, max, coordinates, size, border's width
		Slider(float min = 0, float max = 1, float x = 0, float y = 0, float width = 100, float height = 20, float border_width = 1);
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

		// Increases the value defined by increment and performs an action
		void inc_value();
		// Decreases the value defined by increment and performs an action
		void dec_value();
		// Changes the value and performs a slider changed action
		void change_value(float value);
	};

	float Slider::calc_percent() {
		return (this->value - this->min) / (this->max - this->min);
	}

	void Slider::handle_event(Event& e) {
		if (this->dragged) {																			// If the indicator is dragged
			float2 pos = float2(e.x, e.y) - this->indicator_pos_offset;
			switch (this->align) {																		// Move it according to the mouse position
				case ALIGN::HORIZONTAL:
				{
					pos.x = std::min(pos.x, (float)(this->get_width() - 20 - 20));							// The indicator has a 20 width/height according to the alignment
					pos.x = std::max(pos.x, (float)20);
					this->change_value((this->max - this->min)*(pos.x - 20) / (this->get_width() - 60));
					break;
				}
				case ALIGN::VERTICAL:
				{
					pos.y = std::min(pos.y, (float)(this->get_height() - 20 - 20));
					pos.y = std::max(pos.y, (float)20);
					this->change_value((this->max - this->min)*(pos.y - 20) / (this->get_height() - 60));
					break;
				}
			}
		} else if (e.mouse_is_inside && !this->btn_left->is_inside(e) && !this->btn_right->is_inside(e) &&
				   !this->btn_indicator->is_inside(e) && !e.mouse_is_covered) {							// Else if the mouse is inside the slider, but not above the left/right/indicator buttons
			if (e.mouse_pressed && e.mouse_left) {														// And if the user pressed the left mouse button
				this->watch.start();																	// Then start the watch to animate slider moving
			} else if (e.mouse_left_down) {																// If the user still holds down the left mouse button
				if (this->watch.is_running()) {															// And the watch is running
					float dt = this->watch.get_delta<Time::SECONDS>() * this->speed;								// Then calc a dt to how much move the slider towards the mouse
					if (this->watch.get_elapsed<Time::SECONDS>() > this->wait_time) {								// If enough time elapsed for the indicator to starts moving towards the mouse
						switch (this->align) {
							case ALIGN::HORIZONTAL:
							{
								if (e.x < this->btn_indicator->get_absolute_position().x) {
									this->change_value(this->get_value() - dt);
								} else if (e.x > this->btn_indicator->get_absolute_position().x + this->btn_indicator->get_width()) {
									this->change_value(this->get_value() + dt);
								}
								break;
							}
							case ALIGN::VERTICAL:
							{
								if (e.y < this->btn_indicator->get_absolute_position().y) {
									this->change_value(this->get_value() - dt);
								} else if (e.y > this->btn_indicator->get_absolute_position().y + this->btn_indicator->get_height()) {
									this->change_value(this->get_value() + dt);
								}
								break;
							}
						}
					}
				}
			}
		} else {																						// If the mouse is not inside the scroll bar
			this->watch.stop();																			// Stop moving the slider towards the mouse
		}
	}

	void Slider::draw(bool draw_background) {
		if (this->min == this->max) {
			this->disable();
		}else{
			this->enable();
		}

		switch (this->align) {																			// Set the position and size of the slider components according to it's alignment
			case ALIGN::HORIZONTAL:
			{
				this->btn_left->get_label()->set_text("<");
				this->btn_left->set_size(float2(20, this->get_height()));
				this->btn_left->set_position(float2(0, 0));

				this->btn_right->get_label()->set_text(">");
				this->btn_right->set_size(float2(20, this->get_height()));
				this->btn_right->set_position(float2(this->get_width() - 20, 0));

				this->btn_indicator->get_label()->set_text("|");
				this->btn_indicator->set_size(float2(20, this->get_height()));
				this->btn_indicator->set_position(float2(19 + (this->get_width() - 58) * this->calc_percent(), 0));
				break;
			}
			case ALIGN::VERTICAL:
			{
				this->btn_right->get_label()->set_text(R"(\/)");
				this->btn_right->set_size(float2(this->get_width(), 20));
				this->btn_right->set_position(float2(0, this->get_height() - 20));

				this->btn_left->get_label()->set_text(R"(/\)");
				this->btn_left->set_size(float2(this->get_width(), 20));
				this->btn_left->set_position(float2(0, 0));

				this->btn_indicator->get_label()->set_text("-");
				this->btn_indicator->set_size(float2(this->get_width(), 20));
				this->btn_indicator->set_position(float2(0, 19 + (this->get_height() - 58) * this->calc_percent()));
				break;
			}
		}

		Component::draw(draw_background);
	}

	// To listen on inner component actions
	void Slider::action_performed(void* sender, ActionEvent& e) {
		if (sender == this->btn_left && e.button_pressed) {											// If the left button was pressed
			this->dec_value();																		// Decrease the value
		} else if (sender == this->btn_right && e.button_pressed) {									// Else if right button was pressed
			this->inc_value();																		// Increase the value
		} else if (sender == this->btn_indicator && e.button_pressed) {								// Else if the indicator button was pressed
			this->dragged = true;																	// The indicator is grabbed
			this->indicator_pos_offset = float2(e.x, e.y) - this->btn_indicator->get_position();
		} else if (sender == this->btn_indicator && e.button_released) {							// Else if the indicator button was released
			this->dragged = false;																	// The indicator is not grabbed anymore
		}
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

		this->btn_left->add_action_listener(this);
		this->btn_right->add_action_listener(this);
		this->btn_indicator->add_action_listener(this);

		this->add_component(this->btn_left);
		this->add_component(this->btn_right);
		this->add_component(this->btn_indicator);

		if (height <= width) {
			this->align = ALIGN::HORIZONTAL;
		} else {
			this->align = ALIGN::VERTICAL;
		}
	}

	// Sets the min
	void Slider::set_min(float min) {
		this->min = std::min(min, this->max);							// Can't be bigger than max
		this->value = std::max(this->value, min);						// Adjust the value if new min is bigger than value
	}

	// Sets the max
	void Slider::set_max(float max) {
		this->max = std::max(max, this->min);							// Can't be smaller than min
		this->value = std::min(this->value, max);						// Adjust eh value if new max is smaller than value
	}

	// Sets the value (will be between min and max)
	void Slider::set_value(float value) {
		this->value = std::max(std::min(value, this->max), this->min);	// Must be between min and max
	}

	// Sets the increment (min 1)
	void Slider::set_increment(float increment) {
		this->increment = std::max(increment, 1.0f);					// Can't be smaller than 1
	}

	// Sets the wait time (min 0)
	void Slider::set_wait_time(float wait_time) {
		this->wait_time = std::max(wait_time, 0.0f);					// Can't be smaller than 0
	}

	// Sets the speed (min 10)
	void Slider::set_speed(float speed) {
		this->speed = std::max(speed, 10.0f);							// Can't be smaller than 10
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

	// Increases the value defined by increment and performs an action
	void Slider::inc_value() {
		this->change_value(this->value + this->increment);
	}

	// Decreases the value defined by increment and performs an action
	void Slider::dec_value() {
		this->change_value(this->value - this->increment);
	}

	// Changes the value and performs a slider changed action
	void Slider::change_value(float value) {
		float old_value = this->value;
		this->set_value(value);

		ActionEvent e;
		e.slider_changed = old_value != this->value;
		e.slider_value = this->value;
		e.slider_dvalue = this->value - old_value;
		this->perform_action(this, e);
	}

}