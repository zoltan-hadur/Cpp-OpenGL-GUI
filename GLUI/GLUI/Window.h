#pragma once

#include "Component.h"

namespace GLUI {

	// Panel, that can store and display various components, movable, collapsable, minimizable
	class Window : public EventListener, public Panel {
	protected:
		enum class STATE_0 {	// States of window collapsing
			EXPANDED,
			COLLAPSING,
			COLLAPSED,
			EXPANDING
		};

		enum class STATE_1 {	// States of window minimizing
			ENLARGED,
			MINIMIZING,
			MINIMIZED,
			ENLARGING
		};

		STATE_0 state_0;		// Stores that the window is expanded, collapsing, collapsed, or expanding
		STATE_1 state_1;		// Stores that the window is enlarged, minimizing, minimized, or enlarging

		Button* btn_title;		// Title of the panel (can be grabbed and move the window by the mouse)
		Button* btn_collapse;	// Button to collapse the window
		Button* btn_minimize;	// Button to minimize the window

		float2 pos_offset;		// Position offset for moving the window and for minimize animation
		float2 size_offset;		// Size offset for collapsing 
		float2 orig_pos;		// Original position for minimize
		float2 orig_size;		// Original size for minimize and collapse
		bool dragged;			// Becomes true when the user clicks on the title button (when the window is not minimized), and false when releases the left mouse button

		Stopwatch watch;		// Timer for animations
		float2 acc_delta_time;	// Stores accumulated non linear delta times
		float2 acc_delta_time_2;// Stores accumulated non linear delta times
		float2 acc_elapsed_time;// Stores elapsed time

		float anim_time;

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		// To listen on inner component events
		virtual void action_performed(void* sender, Event& e) override;
		// Title, the coordinates, the size, and the border's width
		Window(std::string title = "", float x = 0, float y = 0, float width = 100, float height = 100, float border_width = 2);
		// Sets the title of the panel
		void set_title(std::string title);
		// Gets the title of the panel
		std::string get_title();
	};

	void Window::handle_event(Event& e) {
		if (this->dragged) {														// If dragged
			if (e.mouse_moved) {													// And the mouse moved
				this->set_position(float2(e.x, e.y) - this->pos_offset);			// Reposition the panel
				this->orig_pos = this->get_position();
			}
		}
	}

	void Window::draw(bool draw_background) {
		this->set_use_scissor(true);

		if (this->state_0 == STATE_0::EXPANDED) {
			if (this->state_1 == STATE_1::MINIMIZED) {
				bool visibility = this->is_visible();
				this->set_visible(false);
				this->visible = visibility;
				this->btn_title->set_visible(visibility);
				this->btn_title->set_size(this->btn_title->get_label()->get_text().size()*this->char_width + 2 * this->char_width, 20);

				float offset = 0;
				for (int i = 0; i < this->parent->get_minimized_windows().size(); ++i) {
					if (this->parent->get_minimized_windows()[i] == this) {
						break;
					}
					offset = offset + this->parent->get_minimized_windows()[i]->get_width() - this->parent->get_minimized_windows()[i]->get_default_border_width();
				}
				this->set_position(offset, this->parent->get_height() - 20);
				this->pos_offset = this->pos - this->orig_pos;
				this->acc_delta_time_2 = this->pos_offset;

			} else if (this->state_1 != STATE_1::ENLARGED) {
				this->btn_title->set_size(this->orig_size.x - 50 + this->default_border_width * 2, 20);
				this->set_visible(this->is_visible());
				if (this->watch.is_running()) {
					float dt = this->watch.get_delta_time();
					this->acc_elapsed_time.x = this->watch.get_elapsed_time();
					this->acc_elapsed_time.y = this->acc_elapsed_time.x;

					float2 speed_size = float2(20 * (std::fabs(this->size_offset.x - this->acc_delta_time.x) * this->acc_elapsed_time.x) / this->anim_time,
											   20 * (std::fabs(this->size_offset.y - this->acc_delta_time.y) * this->acc_elapsed_time.y) / this->anim_time);

					float2 speed_pos = float2(20 * (std::fabs(this->pos_offset.x - this->acc_delta_time_2.x) * this->acc_elapsed_time.x) / this->anim_time,
											  -20 * (std::fabs(this->pos_offset.y - this->acc_delta_time_2.y) * this->acc_elapsed_time.y) / this->anim_time);

					if (this->state_1 == STATE_1::MINIMIZING) {
						speed_size.x = -speed_size.x;
						speed_size.y = -speed_size.y;
						speed_pos.x = -speed_pos.x;
						speed_pos.y = -speed_pos.y;
					}

					this->acc_delta_time = float2(this->acc_delta_time.x + speed_size.x * dt,
												  this->acc_delta_time.y + speed_size.y * dt);

					this->acc_delta_time_2 = float2(this->acc_delta_time_2.x + speed_pos.x * dt,
													this->acc_delta_time_2.y + speed_pos.y * dt);

					bool size_in_x = false;
					bool size_in_y = false;
					if (std::fabs(this->size_offset.x - this->acc_delta_time.x) < 1.0f) {
						this->acc_delta_time.x = this->size_offset.x;
						size_in_x = true;
					}
					if (std::fabs(this->size_offset.y - this->acc_delta_time.y) < 1.0f) {
						this->acc_delta_time.y = this->size_offset.y;
						size_in_y = true;
					}

					bool x_in_pos = true;
					bool y_in_pos = true;
					if (std::fabs(this->pos_offset.x - this->acc_delta_time_2.x) < 1.0f) {
						this->acc_delta_time_2.x = this->pos_offset.x;
						x_in_pos = true;
					}
					if (std::fabs(this->pos_offset.y - this->acc_delta_time_2.y) < 1.0f) {
						this->acc_delta_time_2.y = this->pos_offset.y;
						y_in_pos = true;
					}

					if (size_in_x && size_in_y && x_in_pos && y_in_pos) {
						this->watch.stop();

						if (this->state_1 == STATE_1::MINIMIZING) {
							this->state_1 = STATE_1::MINIMIZED;
						} else if (this->state_1 == STATE_1::ENLARGING) {
							this->state_1 = STATE_1::ENLARGED;
						}
					}

					this->set_size(this->orig_size.x + this->acc_delta_time.x, this->orig_size.y + this->acc_delta_time.y);
					this->set_position(this->orig_pos + float2(acc_delta_time_2.x, acc_delta_time_2.y));
				}
			} else {
				this->set_position(this->orig_pos);
				this->set_size(this->orig_size.x, this->orig_size.y);
			}
		}

		if (this->state_1 == STATE_1::ENLARGED) {
			if (this->state_0 == STATE_0::COLLAPSED) {
				bool visibility = this->is_visible();
				this->set_visible(false);
				this->visible = visibility;
				this->btn_title->set_visible(visibility);
				this->btn_minimize->set_visible(visibility);
				this->btn_collapse->set_visible(visibility);
			} else if (this->state_0 != STATE_0::EXPANDED) {
				this->set_visible(this->is_visible());
				if (this->watch.is_running()) {
					float dt = this->watch.get_delta_time();
					this->acc_elapsed_time.x = this->watch.get_elapsed_time();
					this->acc_elapsed_time.y = this->acc_elapsed_time.x;
					float2 speed = float2(20 * (std::fabs(this->size_offset.x - this->acc_delta_time.x) * this->acc_elapsed_time.x) / this->anim_time,
										  20 * (std::fabs(this->size_offset.y - this->acc_delta_time.y) * this->acc_elapsed_time.y) / this->anim_time);

					if (this->state_0 == STATE_0::COLLAPSING) {
						speed.x = -speed.x;
						speed.y = -speed.y;
					}

					this->acc_delta_time = float2(this->acc_delta_time.x + speed.x * dt,
												  this->acc_delta_time.y + speed.y * dt);

					bool size_in_x = false;
					bool size_in_y = false;
					if (std::fabs(this->size_offset.x - this->acc_delta_time.x) < 1.0f) {
						this->acc_delta_time.x = this->size_offset.x;
						size_in_x = true;
					}
					if (std::fabs(this->size_offset.y - this->acc_delta_time.y) < 1.0f) {
						this->acc_delta_time.y = this->size_offset.y;
						size_in_y = true;
					}

					if (size_in_x && size_in_y) {
						this->watch.stop();

						if (this->state_0 == STATE_0::COLLAPSING) {
							this->state_0 = STATE_0::COLLAPSED;
						} else if (this->state_0 == STATE_0::EXPANDING) {
							this->state_0 = STATE_0::EXPANDED;
						}
					}

					this->height = this->orig_size.y + this->acc_delta_time.y;
				}
			}
		}

		Panel::draw(this->draw_background);
	}

	// To listen on inner component events
	void Window::action_performed(void* sender, Event& e) {
		if (sender == this->btn_title) {		// If the title
			if (e.button_pressed) {				// Was pressed
				if (this->state_1 == STATE_1::ENLARGED) {
					this->pos_offset = float2(e.x, e.y) - this->get_absolute_position() + this->parent->get_absolute_position();
					this->dragged = true;
				}
			} else if (e.button_released) {		// The user finished
				this->dragged = false;			// Dragging the window

				if (this->state_1 == STATE_1::MINIMIZED) {
					this->pos_offset = float2(0, 0);
					this->size_offset = float2(0, 0);
					this->parent->remove_minimized_window(this);

					this->state_1 = STATE_1::ENLARGING;

					if (this->watch.is_running()) {														// If the watch is running
						this->watch.reset();															// Reset it
					} else {																			// Else
						this->watch.start();															// Start it
					}
				}
			}
		}
		if (sender == this->btn_collapse) {															// If the collapse button
			if (e.button_released) {																// Was pressed
				if (this->state_0 == STATE_0::EXPANDED || this->state_0 == STATE_0::EXPANDING) {	// Start collapsing the window if it was expanded or it was expanding
					this->size_offset = float2(this->width, 20) - this->orig_size;					// The height is 20 while collapsed, so the offset in y is orig height - 20
					this->state_0 = STATE_0::COLLAPSING;											// Change state
				} else {																			// Start expanding the window if it was collapsed or it was collapsing
					this->size_offset = float2(0, 0);												// The height is orig height while expanded, so the offset in y is 0 (because orig height minus orig height is zero)
					this->state_0 = STATE_0::EXPANDING;												// Change state
				}
																									// Reset the watch at every button press
				if (this->watch.is_running()) {														// If the watch is running
					this->watch.reset();															// Reset it
				} else {																			// Else
					this->watch.start();															// Start it
				}
			}
		}
		if (sender == this->btn_minimize) {
			if (e.button_released) {
				if (this->state_1 == STATE_1::ENLARGED && this->state_0 == STATE_0::EXPANDED) {

					float offset = 0;
					for (int i = 0; i < this->parent->get_minimized_windows().size(); ++i) {
						offset = offset + this->parent->get_minimized_windows()[i]->get_width() - this->parent->get_minimized_windows()[i]->get_default_border_width();
					}
					this->size_offset = float2(this->btn_title->get_label()->get_text().size()*this->char_width + 2 * this->char_width, 20) - this->orig_size;
					this->pos_offset = float2(offset, this->parent->get_height() - 20) - this->orig_pos;

					this->parent->add_minimized_window(this);

					this->state_1 = STATE_1::MINIMIZING;

					if (this->watch.is_running()) {														// If the watch is running
						this->watch.reset();															// Reset it
					} else {																			// Else
						this->watch.start();															// Start it
					}
				}
			}
		}
	}

	// Title, what is displayed in the top left corner, the coordinates, the size, and the border's width
	Window::Window(std::string title, float x, float y, float width, float height, float border_width) : Panel(true, x, y, width, height, border_width) {
		this->state_0 = STATE_0::EXPANDED;
		this->state_1 = STATE_1::ENLARGED;

		this->pos_offset = float2(0, 0);
		this->size_offset = float2(0, 0);
		this->orig_pos = float2(x, y);
		this->orig_size = float2(width, height);
		this->dragged = false;

		this->watch = Stopwatch();
		this->acc_delta_time = float2(0, 0);
		this->acc_delta_time_2 = float2(0, 0);
		this->acc_elapsed_time = float2(0, 0);
		this->anim_time = 1;

		this->btn_title = new Button(" " + title, 0, 0, width - 50 + border_width * 2, 20, border_width);
		this->btn_title->get_label()->set_h_align(Label::H_ALIGN::LEFT);
		this->btn_title->set_wait_time(std::numeric_limits<float>::max());
		this->btn_title->add_event_listener(this);
		this->add_component(this->btn_title);

		this->btn_collapse = new Button(R"(\/)", width - 25, 0, 25, 20, border_width);
		this->btn_collapse->set_wait_time(std::numeric_limits<float>::max());
		this->btn_collapse->add_event_listener(this);
		this->add_component(this->btn_collapse);

		this->btn_minimize = new Button("_", width - 50 + border_width, 0, 25, 20, border_width);
		this->btn_minimize->set_wait_time(std::numeric_limits<float>::max());
		this->btn_minimize->add_event_listener(this);
		this->add_component(this->btn_minimize);

		this->set_draw_background(true);
		this->set_use_scissor(true);
	}

	// Sets the title of the panel
	void Window::set_title(std::string title) {
		this->btn_title->get_label()->set_text(" " + title);
	}

	// Gets the title of the panel
	std::string Window::get_title() {
		return this->btn_title->get_label()->get_text().substr(1);
	}

}