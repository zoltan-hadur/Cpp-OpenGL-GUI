#pragma once

#include "Component.h"
#include "../Stopwatch.h"

namespace GLUI {

	class Slider : public EventListener, public Component {
	protected:
		enum class ALIGN;

		Button* left;
		Button* right;
		Button* indicator;
		Stopwatch watch;
		float wait_time;
		float speed;

		bool dragged;
		float min;
		float max;
		float value;
		float increment;
		ALIGN align;			// Determines the alignment, either horizontal of vertical

		float2 offset;
	public:
		enum class ALIGN {
			HORIZONTAL,			// Horizontal alignment
			VERTICAL			// Vertical alignment
		};

		Slider(float min = 0, float max = 1, float x = 0, float y = 0, float width = 100, float height = 20, float border_width = 1);
		void inc_value();
		void dec_value();
		void set_min(float min = 0);
		void set_max(float max = 1);
		void set_value(float value = 0);
		void set_increment(float increment = 0.1);
		void set_wait_time(float wait_time);
		void set_speed(float speed);
		float get_min();
		float get_max();
		float get_value();
		float get_increment();
		float get_wait_time();
		float get_speed();
		virtual void action_performed(void* sender, Event& e) override;
		virtual void handle_event(Event& e) override;
		virtual void draw() override;
	};

	Slider::Slider(float min, float max, float x, float y, float width, float height, float border_width) : Component(x, y, width, height, border_width) {
		this->wait_time = 0.0;
		this->speed = 200;

		this->min = min;
		this->max = max;
		this->value = min;
		this->increment = (max - min) / 10;
		this->dragged = false;

		this->left = new Button();
		this->right = new Button();
		this->indicator = new Button();
		this->add_component(left);
		this->add_component(right);
		this->add_component(indicator);
		this->left->add_event_listener(this);
		this->right->add_event_listener(this);
		this->indicator->add_event_listener(this);

		if (height <= width) {	// Horizontal
			this->align = ALIGN::HORIZONTAL;
		} else {				// Vertical
			this->align = ALIGN::VERTICAL;
		}
	}

	void Slider::inc_value() {
		float old_val = value;
		value = std::min(value + increment, max);
		Event e;
		e.slider_value = value;
		e.slider_dvalue = old_val - value;
		this->raise_event(this, e);
	}

	void Slider::dec_value() {
		float old_val = value;
		value = std::max(value - increment, min);
		Event e;
		e.slider_value = value;
		e.slider_dvalue = old_val - value;
		this->raise_event(this, e);
	}

	void Slider::set_min(float min) {
		this->min = std::min(min, max);
		this->value = std::max(value, min);
	}

	void Slider::set_max(float max) {
		this->max = std::max(max, min);
		this->value = std::min(value, max);
	}

	void Slider::set_value(float value) {
		this->value = std::max(std::min(value, max), min);
	}

	void Slider::set_increment(float increment) {
		this->increment = std::max(increment, 1.0f);
	}

	void Slider::set_wait_time(float wait_time) {
		this->wait_time = std::max(wait_time, 0.0f);
	}

	void Slider::set_speed(float speed) {
		this->speed = std::max(speed, 10.0f);
	}

	float Slider::get_min() {
		return min;
	}

	float Slider::get_max() {
		return max;
	}

	float Slider::get_value() {
		return value;
	}

	float Slider::get_increment() {
		return increment;
	}

	float Slider::get_wait_time() {
		return this->wait_time;
	}

	float Slider::get_speed() {
		return this->speed;
	}

	void Slider::action_performed(void* sender, Event& e) {
		if (e.button_pressed) {
			if (sender == left) {
				this->dec_value();
			} else if (sender == right) {
				this->inc_value();
			} else if (sender == indicator) {
				this->dragged = true;
				float2 pos = this->get_position();
				this->offset = float2(e.x, e.y) - indicator->get_position();
			}
		}
	}

	void Slider::handle_event(Event& e) {
		float old_val = value;
		float2 pos = this->get_absolute_position();
		float2 pos_offset = (align == ALIGN::HORIZONTAL) ? float2(15, 0) : float2(0, 15);

		if (this->dragged) {
			float2 pos = float2(e.x, e.y) - this->offset;
			if (height <= width) {	// Horizontal
				pos.x = std::min(pos.x, (float)(width - 15 - 20));
				pos.x = std::max(pos.x, (float)15);
				value = (max - min)*(pos.x - 15) / (width - 50);
			} else {				// Vertical
				pos.y = std::min(pos.y, (float)(height - 15 - 20));
				pos.y = std::max(pos.y, (float)15);
				value = (max - min)*(pos.y - 15) / (height - 50);
			}
		} else if (pos.x + pos_offset.x < e.x && e.x < pos.x + width - pos_offset.x && pos.y + pos_offset.y < e.y && e.y < pos.y + height - pos_offset.y) {
			if (e.mouse_left && e.mouse_pressed) {
				this->watch.start();
				printf("watch started\n");
			} else if (e.mouse_left_down) {
				if (this->watch.is_running()) {
					float dt = this->watch.get_delta_time() *speed;
					if (watch.get_elapsed_time() > this->wait_time) {
						switch (align) {
							case ALIGN::HORIZONTAL:
							{
								if (e.x < indicator->get_absolute_position().x) {
									this->set_value(this->get_value() - dt);
								} else if (e.x > indicator->get_absolute_position().x + indicator->get_width()) {
									this->set_value(this->get_value() + dt);
								}
								break;
							}
							case ALIGN::VERTICAL:
							{
								if (e.y < indicator->get_absolute_position().y) {
									this->set_value(this->get_value() - dt);
								} else if (e.y > indicator->get_absolute_position().y + indicator->get_height()) {
									this->set_value(this->get_value() + dt);
								}
								break;
							}
						}
					}
				}
			} else if (e.mouse_left && e.mouse_released) {
				watch.stop();
			}
		} else {
			watch.stop();
		}
		if (!e.mouse_left_down) {
			dragged = false;
		}
		if (old_val - value != 0) {
			e.slider_value = value;
			e.slider_dvalue = old_val - value;
			this->raise_event(this, e);
			e.slider_dvalue = 0;
		}
	}

	void Slider::draw() {
		if (height <= width) {	// Horizontal
			left->get_label()->set_text("<");
			left->set_size(16, height);
			left->set_position(0, 0);

			right->get_label()->set_text(">");
			right->set_size(16, height);
			right->set_position(width - 16, 0);

			indicator->get_label()->set_text("|");
			indicator->set_size(20, height);
			indicator->set_position(15 + (width - 50)*(value + min) / (max - min), 0);
		} else {				// Vertical
			right->get_label()->set_text(R"(\/)");
			right->set_size(width, 16);
			right->set_position(0, height - 16);

			left->get_label()->set_text(R"(/\)");
			left->set_size(width, 16);
			left->set_position(0, 0);

			indicator->get_label()->set_text("-");
			indicator->set_size(width, 20);
			indicator->set_position(0, 15 + (height - 50)*(value + min) / (max - min));
		}

		Component::draw();

		float2 pos = this->get_absolute_position() + float2(default_border_width, default_border_width);
		glColor4f(1, 1, 1, 0.5);
		glLineWidth(1);
		glLineStipple(1, 0b0101010101010101);
		glEnable(GL_LINE_STIPPLE);
		glBegin(GL_LINES);
		for (int i = 0; i < width - default_border_width; i = i + 2) {
			glVertex2f(pos.x + i, pos.y + (i/2)%2);
			glVertex2f(pos.x + i, pos.y + (i/2)%2 +height - default_border_width*2);
		}
		glEnd();
		glDisable(GL_LINE_STIPPLE);
	}

}