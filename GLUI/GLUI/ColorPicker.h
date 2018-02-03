#pragma once

#include <cmath>
#include "Panel.h"
#include "Button.h"
#include "SpinnerBox.h"
#include "..\Event\ActionEvent.h"
#include "..\Event\ActionListener.h"
#include "..\Event\ActionPerformer.h"
#include "..\Event\Event.h"
#include "..\Utility\Animator.h"
#include "..\Utility\float2.h"
#include "..\Utility\Color.h"
#include "..\Utility\Stopwatch.h"

namespace GLUI {

	// The ColorPicker user control lets you set a color with 3 spinner box (RGB)
	// and performs actions when the color changes
	class ColorPicker : public ActionListener, public Panel {
	protected:
		enum class STATE {
			NORMALIZED,
			MAXIMIZED,
			NORMALIZING,
			MAXIMIZING
		};

		STATE state;						// Stores the state of the ColorPicker

		Button* btn_indicator;				// Button to indicate the picked color, and to maximize/normalize the ColorPicker
		SpinnerBox<int>* spb_r;				// SpinnerBox for RED channel
		SpinnerBox<int>* spb_g;				// SpinnerBox for GREEN channel
		SpinnerBox<int>* spb_b;				// SpinnerBox for BLUE channel

		Animator* pos_animator;
		Animator* size_animator;

		float2 orig_pos;					// Original position for animation
		float2 orig_size;					// Original size for animation

		Duration<Time::SECONDS> anim_time;	// Length of the animation

		Color color;

		void normalize();
		void maximize();

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		// To listen on inner component's actions
		virtual void action_performed(void* sender, ActionEvent& e) override;

		// Position, size, border's width
		ColorPicker(float x = 0, float y = 0, float width = 100, float height = 100, float border_width = 1);

		// Sets the animation length
		template<typename Time_TYPE> void set_anim_time(Duration<Time_TYPE> anim_time);
		// Sets color (NOT performs any action)
		void set_color(Color c);
		// Gets the animation length
		Duration<Time::SECONDS> get_anim_time();
		// Gets color
		Color get_color();

		// Performs a color changed action
		void change_color(Color c);
	};

	void ColorPicker::normalize() {
		//this->pos_animator->stop();
		this->size_animator->stop();

		this->state = STATE::NORMALIZING;

		//this->pos_animator->set_curr(this->get_position());
		//this->pos_animator->set_src(this->pos_animator->get_curr());
		//this->pos_animator->set_dst(this->orig_pos);

		this->size_animator->set_curr(this->get_size());
		this->size_animator->set_src(this->size_animator->get_curr());
		this->size_animator->set_dst(this->orig_size);

		//this->pos_animator->start();
		this->size_animator->start();
	}

	void ColorPicker::maximize() {
		//this->pos_animator->stop();
		this->size_animator->stop();

		this->state = STATE::MAXIMIZING;

		//this->pos_animator->set_curr(this->get_position());
		//this->pos_animator->set_src(this->pos_animator->get_curr());
		//this->pos_animator->set_dst(this->parent->get_base());

		this->size_animator->set_curr(this->get_size());
		this->size_animator->set_src(this->size_animator->get_curr());
		this->size_animator->set_dst(float2(this->get_width() + this->spb_b->get_width() - this->default_border_width,
											std::max(this->get_height(), 60.0f - this->spb_r->get_default_border_width() * 3)));

		//this->pos_animator->start();
		this->size_animator->start();
	}

	void ColorPicker::handle_event(Event& e) {
		if (!e.mouse_is_inside && e.mouse_pressed && e.mouse_left) {	// If the user pressed the left mouse button outside the color picker
			if (this->state != STATE::NORMALIZED) {						// And if the color picker was not normalized
				this->normalize();
			}
		}
	}

	void ColorPicker::draw(bool draw_background) {
		//this->pos_animator->update();																						// Update the animator
		this->size_animator->update();																						// Update the animator

		if (this->state == STATE::NORMALIZING || this->state == STATE::MAXIMIZING) {
			//this->set_position(this->pos_animator->get_curr());															// Set the position according to the animator
			this->set_size(this->size_animator->get_curr());																// Set the size according to the animator
		} else switch (this->state) {
			case STATE::NORMALIZED:
			{
				this->set_position(this->orig_pos);
				this->set_size(this->orig_size);
				break;
			}
			case STATE::MAXIMIZED:
			{

				break;
			}
		}


		this->set_visible(!(this->state == STATE::NORMALIZED));
		this->visible = true;
		this->btn_indicator->set_visible(true);

		this->btn_indicator->set_background_color(this->color);
		this->btn_indicator->set_highlight_color(this->color * 1.2);

		Panel::draw(draw_background);
	}

	// To listen on inner component actions
	void ColorPicker::action_performed(void* sender, ActionEvent& e) {
		if (sender == this->pos_animator || sender == this->size_animator) {					// If one of the animators
			if (e.animator_reached) {															// Reached it's destination
				switch (this->state) {															// Finish the animation
					case STATE::NORMALIZING: this->state = STATE::NORMALIZED;
						break;
					case STATE::MAXIMIZING: this->state = STATE::MAXIMIZED;
						break;
				}
			}
		}
		if (sender == this->btn_indicator) {													// If the indicator button
			if (e.button_released) {															// Was pressed
				if (this->state == STATE::NORMALIZED || this->state == STATE::NORMALIZING) {	// And the ColorPicker was normalized or currently normalizing,
					this->maximize();															// Maximize it
				} else {																		// Else
					this->normalize();															// Normalize it
				}
			}
		}
		if (sender == this->spb_r || sender == this->spb_g || sender == this->spb_b) {			// If any of the spinner boxes performed an action, change the color
			this->change_color(Color(this->spb_r->get_value(),									// R
									 this->spb_g->get_value(),									// G
									 this->spb_b->get_value(),									// B
									 255) / 255);												// A
		}
	}

	// Position, size, border's width
	ColorPicker::ColorPicker(float x, float y, float width, float height, float border_width) : Panel(true, x, y, width, height, border_width) {
		this->state = STATE::NORMALIZED;

		this->orig_pos = float2(x, y);
		this->orig_size = float2(width, height);

		this->color = Color(1, 0, 1, 1);

		this->btn_indicator = new Button("", 0, 0, width, height, 1);
		this->spb_r = new SpinnerBox<int>(0, 255, width - 1, 0, 100, 20, 1);
		this->spb_g = new SpinnerBox<int>(0, 255, width - 1, 19, 100, 20, 1);
		this->spb_b = new SpinnerBox<int>(0, 255, width - 1, 38, 100, 20, 1);

		this->btn_indicator->set_wait_time(Duration<Time::HOURS>(24));
		this->spb_r->set_value(this->color.R * 255);
		this->spb_g->set_value(this->color.G * 255);
		this->spb_b->set_value(this->color.B * 255);

		this->btn_indicator->add_action_listener(this);
		this->spb_r->add_action_listener(this);
		this->spb_g->add_action_listener(this);
		this->spb_b->add_action_listener(this);

		this->add_component(this->btn_indicator);
		this->add_component(this->spb_r);
		this->add_component(this->spb_g);
		this->add_component(this->spb_b);

		this->pos_animator = new Animator();
		this->pos_animator->add_action_listener(this);

		this->size_animator = new Animator();
		this->size_animator->add_action_listener(this);

		this->set_anim_time(Duration<Time::MILLI_SECONDS>(500));
	}

	// Sets the animation length
	template<typename Time_TYPE> void ColorPicker::set_anim_time(Duration<Time_TYPE> anim_time) {
		this->pos_animator->set_anim_time(anim_time);
		this->size_animator->set_anim_time(anim_time);
		this->anim_time = anim_time;
	}

	// Sets color (NOT performs any action)
	void ColorPicker::set_color(Color c) {
		this->color = c;
	}

	// Gets the animation length
	Duration<Time::SECONDS> ColorPicker::get_anim_time() {
		return this->anim_time;
	}

	// Gets color
	Color ColorPicker::get_color() {
		return this->color;
	}

	// Performs a color changed action
	void ColorPicker::change_color(Color c) {
		this->color = c;
		ActionEvent e;
		e.colorpicker_color = c;
		this->perform_action(this, e);
	}

}