#pragma once

#include "Component.h"

namespace GLUI {

	// Movable, collapsable and minimizable panel, that can store and display various components
	// Added components needs a (0,20) offset to their positions because of window title
	class Window : public EventListener, public Panel {
	protected:
		enum class C_STATE {	// States of window collapsing
			EXPANDED,
			COLLAPSING,
			COLLAPSED,
			EXPANDING
		};

		enum class M_STATE {	// States of window minimizing
			ENLARGED,
			MINIMIZING,
			MINIMIZED,
			ENLARGING
		};

		C_STATE c_state;		// Stores that the window is expanded, collapsing, collapsed, or expanding
		M_STATE m_state;		// Stores that the window is enlarged, minimizing, minimized, or enlarging

		Button* btn_title;		// Title of the panel (the window can be moved by dragging this)
		Button* btn_collapse;	// Button to collapse the window
		Button* btn_minimize;	// Button to minimize the window

		float2 pos_offset;		// Position offset for animation
		float2 size_offset;		// Size offset for animation
		float2 orig_pos;		// Original position for animation
		float2 orig_size;		// Original size for animation
		float2 move_offset;		// Position offset for moving window

		bool grabbed;			// Indicates that the window is grabbed by the mouse or not

		Stopwatch watch;		// Timer for animations
		float2 acc_size;		// Stores accumulated size value for animation
		float2 acc_pos;			// Stores accumulated position values for animation
		float anim_time;		// Determines how long the animations last (not accurate because of non linearity)

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
		if (this->grabbed && e.mouse_moved) {																			// If the user grabbed the window and moved the mouse
			this->set_position(float2(e.x, e.y) - this->move_offset);													// Reposition the panel
			this->orig_pos = this->get_position();																		// To enlarge the window to the new position from minimized state
		}
	}

	void Window::draw(bool draw_background) {
		this->btn_collapse->bring_front();																				// Bring to the front
		this->btn_minimize->bring_front();																				// Bring to the front
		this->btn_title->bring_front();																					// Bring to the front to draw last (hence it is in front of every object in the window)

		if (this->m_state == M_STATE::MINIMIZED) {																		// If the window is minimized
			float offset = 0;
			for (int i = 0; i < this->parent->get_minimized_windows().size(); ++i) {									// Get how far the window needs to be from the parent component's left side according to the minimized windows that the parent holds
				if (this->parent->get_minimized_windows()[i] == this) {													// Therefore is this break
					break;
				}
				offset = offset + this->parent->get_minimized_windows()[i]->get_width() - this->parent->get_minimized_windows()[i]->get_default_border_width();
			}
			float2 pos_offset = float2(offset, this->parent->get_height() - 20) - this->orig_pos;						// Not setting the offset yet if it's not modified
			this->set_position(this->pos.x, this->parent->get_height() - 20);											// Always at the bottom of the parent
			this->acc_pos.y = pos_offset.y;																				// Always at the bottom of the parent
			if (this->pos_offset.x != pos_offset.x) {																	// If the offset modified (a minimized window was removed from the list, thus the window needs to move to the left)
				this->pos_offset = pos_offset;																			// The position where the window moves
				this->m_state = M_STATE::MINIMIZING;																	// Modify the state to start animating again
				if (this->watch.is_running()) {																			// Restart the timer
					this->watch.reset();
				} else {
					this->watch.start();
				}
			} else {																									// If the minimized window not moved
				bool visibility = this->is_visible();
				this->set_visible(false);																				// Every component is invisibile
				this->visible = visibility;
				this->btn_title->set_visible(visibility);																// Except the title (needs to be visible, else how the user could enlarge the window)
				this->btn_title->set_size(this->width, 20);
			}
		} else {																										// If the window is not minimized
			this->set_visible(this->is_visible());																		// Update the visibility tree
			this->btn_title->set_size(this->orig_size.x - 50 + this->default_border_width * 2, 20);						// Set the original size of the title
			if (this->c_state == C_STATE::COLLAPSED) {																	// If the window was collapsed
				bool visibility = this->is_visible();
				this->set_visible(false);																				// Every component is invisible
				this->visible = visibility;
				this->btn_title->set_visible(visibility);																// Exceot the title,
				this->btn_minimize->set_visible(visibility);															// Minimize button
				this->btn_collapse->set_visible(visibility);															// And collapse button
			}
		}

		if (this->watch.is_running()) {																					// Animate if the watch is running
			float dt = this->watch.get_delta_time();																	// Get delta time
			float et = this->watch.get_elapsed_time();																	// Get elapsed time

			float2 speed_size = (this->size_offset - this->acc_size) * 20 * et / this->anim_time;						// Calculate the current speed to modify the window's size
			float2 speed_pos = (this->pos_offset - this->acc_pos) * 20 * et / this->anim_time;							// Calculate the current speed to modify the window's position

			this->acc_size = this->acc_size + speed_size * dt;															// Accumulate the window's size
			this->acc_pos = this->acc_pos + speed_pos * dt;																// Accumulate the window's position

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

			bool x_in_pos = false;
			bool y_in_pos = false;
			if (std::fabs(this->pos_offset.x - this->acc_pos.x) < 1.0f) {												// Check if the x coordinate of the window's position has reached it's target value
				this->acc_pos.x = this->pos_offset.x;
				x_in_pos = true;
			}
			if (std::fabs(this->pos_offset.y - this->acc_pos.y) < 1.0f) {												// Check if the y coordinate of the window's position has reached it's target value
				this->acc_pos.y = this->pos_offset.y;
				y_in_pos = true;
			}

			this->set_size(this->orig_size.x + this->acc_size.x, this->orig_size.y + this->acc_size.y);					// Modifiy the window's size according to the animation
			this->set_position(this->orig_pos.x + this->acc_pos.x, this->orig_pos.y + this->acc_pos.y);					// Modify the window's position according to the animation

			if (x_in_size && y_in_size && x_in_pos && y_in_pos) {														// If the window's position and it's size reached their target value
				this->watch.stop();																						// The animation is over

				if (this->c_state == C_STATE::COLLAPSING) {																// Set the states
					this->c_state = C_STATE::COLLAPSED;
				}
				if (this->c_state == C_STATE::EXPANDING) {
					this->c_state = C_STATE::EXPANDED;
				}

				if (this->m_state == M_STATE::MINIMIZING) {
					this->m_state = M_STATE::MINIMIZED;
				}
				if (this->m_state == M_STATE::ENLARGING) {
					this->m_state = M_STATE::ENLARGED;
				}
			}
		}
		Panel::draw(draw_background);
	}

	// To listen on inner component events
	void Window::action_performed(void* sender, Event& e) {
		if (sender == this->btn_title) {																				// If the title
			if (e.button_pressed) {																						// Was pressed
				if (this->m_state == M_STATE::ENLARGED) {																// And the window is fully enlarged
					this->move_offset = (float2(e.x, e.y) - this->get_absolute_position() +								// Calculate the mouse position relative the window's parent's absolute position
										 this->parent->get_absolute_position());
					this->grabbed = true;																				// Because the user possibly wants to move the window with the mouse
				}
			} else if (e.button_released) {																				// If the title was released
				this->grabbed = false;																					// The user finished moving the window

				if (this->m_state == M_STATE::MINIMIZED || this->m_state == M_STATE::MINIMIZING) {						// If the window is minimized or minimizing
					this->pos_offset = float2(0, 0);																	// Start enlarging it (a (0,0) offset means there is no difference between the original and the destination values, so the window will animate to it's original state from it's current state)
					if (this->c_state == C_STATE::COLLAPSED || this->c_state == C_STATE::COLLAPSING) {					// Target window height is 20 when collapsing or collapsed
						this->size_offset = float2(0, 20 - this->orig_size.y);
					} else {
						this->size_offset = float2(0, 0);																// Target window size is the original when expanding or expanded
					}
					this->parent->remove_minimized_window(this);														// The window is not minimized now, so remove it from the list

					this->m_state = M_STATE::ENLARGING;

																														// Reset the watch at every button press
					if (this->watch.is_running()) {																		// If the watch is running
						this->watch.reset();																			// Reset it
					} else {																							// Else
						this->watch.start();																			// Start it
					}
				}
			}
		}
		if (sender == this->btn_collapse) {																				// If the collapse button
			if (e.button_released) {																					// Was pressed
				if (this->c_state == C_STATE::EXPANDED || this->c_state == C_STATE::EXPANDING) {						// Start collapsing the window if it was expanded or it was expanding
					this->size_offset = float2(this->width, 20) - this->orig_size;										// The height is 20 while collapsed, so the offset in y is orig height - 20
					this->c_state = C_STATE::COLLAPSING;
					this->btn_collapse->get_label()->set_text(R"(\/)");
				} else {																								// Start expanding the window if it was collapsed or it was collapsing
					this->size_offset = float2(0, 0);																	// The height is orig height while expanded or expanding, so the offset in y is 0 (because orig height (target value) minus orig height (initial value) is zero)
					this->c_state = C_STATE::EXPANDING;
					this->btn_collapse->get_label()->set_text(R"(/\)");
				}
																														// Reset the watch at every button press
				if (this->watch.is_running()) {																			// If the watch is running
					this->watch.reset();																				// Reset it
				} else {																								// Else
					this->watch.start();																				// Start it
				}
			}
		}
		if (sender == this->btn_minimize) {																				// If the minimize button
			if (e.button_released) {																					// Was pressed
				if (this->m_state == M_STATE::ENLARGED) {																// And the window is fully enlarged

					float offset = 0;																					// Get how far the window needs to be from the parent component's left side according to the minimized windows that the parent holds
					for (int i = 0; i < this->parent->get_minimized_windows().size(); ++i) {
						offset = (offset +																				// A minimized window's width is determined by it's title text's length
								  this->parent->get_minimized_windows()[i]->btn_title->get_label()->get_text().size() * this->char_width + 2 * this->char_width -
								  this->parent->get_minimized_windows()[i]->get_default_border_width());
					}
					// A minimized window's width is determined by it's title text's length
					this->size_offset = float2(this->btn_title->get_label()->get_text().size() * this->char_width + 2 * this->char_width, 20) - this->orig_size;
					this->pos_offset = float2(offset, this->parent->get_height() - 20) - this->orig_pos;

					this->parent->add_minimized_window(this);															// Add the minimized window to the list

					this->m_state = M_STATE::MINIMIZING;

																														// Reset the watch at every button press
					if (this->watch.is_running()) {																		// If the watch is running
						this->watch.reset();																			// Reset it
					} else {																							// Else
						this->watch.start();																			// Start it
					}
				}
			}
		}
	}

	// Title, what is displayed in the top left corner, the coordinates, the size, and the border's width
	Window::Window(std::string title, float x, float y, float width, float height, float border_width) : Panel(true, x, y, width, height, border_width) {
		this->c_state = C_STATE::EXPANDED;
		this->m_state = M_STATE::ENLARGED;

		this->pos_offset = float2(0, 0);
		this->size_offset = float2(0, 0);
		this->orig_pos = float2(x, y);
		this->orig_size = float2(width, height);
		this->move_offset = float2(0, 0);
		this->grabbed = false;

		this->watch = Stopwatch();
		this->acc_size = float2(0, 0);
		this->acc_pos = float2(0, 0);
		this->anim_time = 1;

		this->btn_title = new Button(" " + title, 0, 0, width - 50 + border_width * 2, 20, border_width);
		this->btn_title->get_label()->set_h_align(Label::H_ALIGN::LEFT);
		this->btn_title->set_wait_time(std::numeric_limits<float>::max());
		this->btn_title->add_event_listener(this);
		this->add_component(this->btn_title);

		this->btn_collapse = new Button(R"(/\)", width - 25, 0, 25, 20, border_width);
		this->btn_collapse->set_wait_time(std::numeric_limits<float>::max());
		this->btn_collapse->add_event_listener(this);
		this->add_component(this->btn_collapse);

		this->btn_minimize = new Button("_", width - 50 + border_width, 0, 25, 20, border_width);
		this->btn_minimize->set_wait_time(std::numeric_limits<float>::max());
		this->btn_minimize->add_event_listener(this);
		this->add_component(this->btn_minimize);
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