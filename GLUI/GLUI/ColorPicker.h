#pragma once

#include "Panel.h"
#include "Button.h"
#include "SpinnerBox.h"
#include "..\Utility\Stopwatch.h"

namespace GLUI {

	class ColorPicker : public EventListener, public Panel {
	protected:
		enum class STATE {
			EXPANDED,
			SHRINKING,
			SHRANK,
			EXPANDING
		};

		STATE state;

		Button* btn_indicator;
		SpinnerBox<int>* spb_r;
		SpinnerBox<int>* spb_g;
		SpinnerBox<int>* spb_b;

		//float2 pos_offset;		// Position offset for animation
		float2 size_offset;		// Size offset for animation
		//float2 orig_pos;		// Original position for animation
		float2 orig_size;		// Original size for animation

		Stopwatch watch;		// Timer for animations
		float2 acc_size;		// Stores accumulated size value for animation
		//float2 acc_pos;			// Stores accumulated position values for animation
		float anim_time;		// Determines how long the animations last (not accurate because of non linearity)

		Color color;

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		// To listen on inner component events
		virtual void action_performed(void* sender, Event& e) override;

		// Position, size, border's width
		ColorPicker(float x = 0, float y = 0, float width = 100, float height = 100, float border_width = 1);
		// Sets color
		void set_color(Color c);
		// Gets color
		Color get_color();
	};


	void ColorPicker::handle_event(Event& e) {
		float2 pos = this->get_absolute_position();															// The absolute position relative to the top-level window
		if (!(pos.x < e.x && e.x < pos.x + this->width && pos.y < e.y && e.y < pos.y + this->height)) {		// Check if the mouse is above the button
			if (e.mouse_pressed && e.mouse_left) {
				if (this->state != STATE::SHRANK) {
					this->state = STATE::SHRINKING;
					this->size_offset = float2(0, 0);
					if (this->watch.is_running()) {
						this->watch.reset();
					}
					else {
						this->watch.start();
					}
				}
			}
		}
	}

	void ColorPicker::draw(bool draw_background) {
		this->set_visible(!(this->state == STATE::SHRANK));
		this->visible = true;
		this->btn_indicator->set_visible(true);

		if (this->watch.is_running()) {																					// Animate if the watch is running
			float dt = this->watch.get_delta_time();																	// Get delta time
			float et = this->watch.get_elapsed_time();																	// Get elapsed time

			float2 speed_size = (this->size_offset - this->acc_size) * 20 * et / this->anim_time;						// Calculate the current speed to modify the window's size

			this->acc_size = this->acc_size + speed_size * dt;															// Accumulate the window's size

			bool x_in_size = false;
			bool y_in_size = false;
			if (std::fabs(this->size_offset.x - this->acc_size.x) < 1.0f) {												// Check if window's width has reached it's target value
				this->acc_size.x = this->size_offset.x;
				x_in_size = true;
			}
			if (std::fabs(this->size_offset.y - this->acc_size.y) < 1.0f) {												// Check if window's height has reached it's target value
				this->acc_size.y = this->size_offset.y;
				y_in_size = true;
			}

			this->set_size(this->orig_size.x + this->acc_size.x, this->orig_size.y + this->acc_size.y);					// Modifiy the window's size according to the animation

			if (x_in_size && y_in_size) {																				// If the window's position and it's size reached their target value
				this->watch.stop();																						// The animation is over

				if (this->state == STATE::SHRINKING) {																	// Set the states
					this->state = STATE::SHRANK;
				}
				if (this->state == STATE::EXPANDING) {
					this->state = STATE::EXPANDED;
				}
			}
		}

		//this->color = Color(this->spb_r->get_value(), this->spb_g->get_value(), this->spb_b->get_value(), 255);

		this->btn_indicator->set_background_color(this->color);
		this->btn_indicator->set_highlight_color(this->color);
		Panel::draw(draw_background);
	}

	// To listen on inner component events
	void ColorPicker::action_performed(void* sender, Event& e) {
		if (sender == this->btn_indicator) {
			if (e.button_released) {
				if (this->state == STATE::SHRANK || this->state == STATE::SHRINKING) {
					this->state = STATE::EXPANDING;
					this->size_offset = float2(99, 58-this->orig_size.y);
				}
				else {
					this->state = STATE::SHRINKING;
					this->size_offset = float2(0, 0);
				}

				if (this->watch.is_running()) {																		// If the watch is running
					this->watch.reset();																			// Reset it
				}
				else {																							// Else
					this->watch.start();																			// Start it
				}
			}
		} else if (sender == this->spb_r || sender == this->spb_g || sender == this->spb_b) {
			this->color = Color(this->spb_r->get_value(), this->spb_g->get_value(), this->spb_b->get_value(), 255);
		}
		//else {
		//	//this->color = Color(this->spb_r->get_value(), this->spb_g->get_value(), this->spb_b->get_value(), 255);
		//	//unsigned char R = this->color.get_ur();
		//	//unsigned char G = this->color.get_ug();
		//	//unsigned char B = this->color.get_ub();
		//	//if (sender == this->spb_r) {
		//	//	R = e.get_spinnerbox_value<unsigned char>();
		//	//}
		//	//if (sender == this->spb_g) {
		//	//	G = e.get_spinnerbox_value<unsigned char>();
		//	//}
		//	//if (sender == this->spb_b) {
		//	//	B = e.get_spinnerbox_value<unsigned char>();
		//	//}
		//	//this->color = Color(R, G, B, 255);
		//}
	}

	// Position, size, border's width
	ColorPicker::ColorPicker(float x, float y, float width, float height, float border_width) : Panel(true, x, y, width, height, border_width) {
		this->state = STATE::SHRANK;

		this->size_offset = float2(0, 0);
		this->orig_size = float2(width, height);

		this->watch = Stopwatch();
		this->acc_size = float2(0, 0);
		this->anim_time = 1;

		this->btn_indicator = new Button("", 0, 0, width, height, 1);
		this->spb_r = new SpinnerBox<int>(0, 255, width - 1, 0, 100, 20, 1);
		this->spb_g = new SpinnerBox<int>(0, 255, width - 1, 19, 100, 20, 1);
		this->spb_b = new SpinnerBox<int>(0, 255, width - 1, 38, 100, 20, 1);

		this->btn_indicator->set_wait_time(std::numeric_limits<float>::max());
		this->color = this->btn_indicator->get_background_color();
		this->spb_r->set_value(this->color.get_ur());
		this->spb_g->set_value(this->color.get_ug());
		this->spb_b->set_value(this->color.get_ub());

		this->btn_indicator->add_event_listener(this);
		this->spb_r->add_event_listener(this);
		this->spb_g->add_event_listener(this);
		this->spb_b->add_event_listener(this);

		this->add_component(this->btn_indicator);
		this->add_component(this->spb_r);
		this->add_component(this->spb_g);
		this->add_component(this->spb_b);
	}

	// Sets color
	void ColorPicker::set_color(Color c) {
		this->color = c;
	}

	// Gets color
	Color ColorPicker::get_color() {
		return this->color;
	}

}