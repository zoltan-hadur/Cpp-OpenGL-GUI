#pragma once

#include "Panel.h"
#include "Button.h"
#include "..\Event\ActionEvent.h"
#include "..\Event\ActionListener.h"
#include "..\Event\Event.h"
#include "..\Utility\Animator.h"
#include "..\Utility\Color.h"
#include "..\Utility\float2.h"

namespace GLUI {

	// Movable, resizable, minimizable, maximizable and closable panel, that can store and display various components
	// Added components needs a (0,20) offset to their positions because of window title
	class Window : public ActionListener, public Panel {
	protected:
		enum class STATE {		// States of window minimizing/maximizing
			MINIMIZED,
			NORMALIZED,
			MAXIMIZED,
			MINIMIZING,
			NORMALIZING,
			MAXIMIZING
		};

		STATE state;			// The current state
		STATE orig_state;		// The original state (only normalized or maximized)

		Button* btn_title;		// Title of the panel (the window can be moved by dragging this)
		Button* btn_minimize;	// Button to minimize the window
		Button* btn_maximize;	// Button to maximize the window
		Button* btn_close;		// Button to close the window

		Animator* pos_animator;
		Animator* size_animator;

		float2 orig_pos;		// Original position for animation
		float2 orig_size;		// Original size for animation
		float2 move_offset;		// Position offset for moving window
		float2 resize_offset;	// Size offset for resizing the window

		bool grabbed_move;		// Indicates that the window is grabbed by the mouse or not for later move
		bool grabbed_resize;	// Indicates that the window is grabbed by the mouse or not for later resize
		bool movable;			// Indicates that the window is movable or not
		bool resizable;			// Indicates that the window is resizable or not
		bool minimizable;		// Indicates that the window is minimizable or not
		bool maximizable;		// Indicates that the window is maximizable or not
		bool closable;			// Indicates that the window is closable or not

		float2 calc_minimized_pos();
		float2 calc_minimized_size();
		void minimize();
		void normalize();
		void maximize();

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		// To listen on inner component actions
		virtual void action_performed(void* sender, ActionEvent& e) override;
		// Gets the base position
		virtual float2 get_base() override;

		// Title, the coordinates, the size, and the border's width
		Window(std::string title = "", float x = 0, float y = 0, float width = 100, float height = 100, float border_width = 2);
		// Sets the movability
		void set_movable(bool movable);
		// Sets the resizability
		void set_resizable(bool resizable);
		// Sets the minimizability
		void set_minimizable(bool minimizable);
		// Sets the maximizability
		void set_maximizable(bool maximizable);
		// Sets the closability
		void set_closable(bool closable);
		// Sets the title of the panel
		void set_title(std::string title);
		// Gets the title of the panel
		std::string get_title();
		// Returns true if movable
		bool is_movable();
		// Returns true resizable
		bool is_resizable();
		// Returns true if minimizable
		bool is_minimizable();
		// Returns true if maximizable
		bool is_maximizable();
		// Return true if closable
		bool is_closable();

	};

	float2 Window::calc_minimized_pos() {
		float offset = 0;
		for (auto window : this->parent->get_minimized_windows()) {
			if (window == this) {
				break;
			} else {
				offset = offset + window->calc_minimized_size().x;
			}
		}
		return float2(offset, this->parent->get_height() - 20);
	}

	float2 Window::calc_minimized_size() {
		return float2(this->btn_title->get_label()->calc_fit_size().x + 2 * this->default_border_width, 20);
	}

	void Window::minimize() {
		this->pos_animator->stop();
		this->size_animator->stop();

		this->state = STATE::MINIMIZING;

		this->pos_animator->set_curr(this->get_position());
		this->pos_animator->set_src(this->pos_animator->get_curr());
		this->pos_animator->set_dst(this->calc_minimized_pos());

		this->size_animator->set_curr(this->get_size());
		this->size_animator->set_src(this->size_animator->get_curr());
		this->size_animator->set_dst(this->calc_minimized_size());

		this->pos_animator->start();
		this->size_animator->start();
	}

	void Window::normalize() {
		this->pos_animator->stop();
		this->size_animator->stop();
		
		this->orig_state = STATE::NORMALIZED;
		this->state = STATE::NORMALIZING;

		this->pos_animator->set_curr(this->get_position());
		this->pos_animator->set_src(this->pos_animator->get_curr());
		this->pos_animator->set_dst(this->orig_pos);

		this->size_animator->set_curr(this->get_size());
		this->size_animator->set_src(this->size_animator->get_curr());
		this->size_animator->set_dst(this->orig_size);

		this->pos_animator->start();
		this->size_animator->start();
	}

	void Window::maximize() {
		this->pos_animator->stop();
		this->size_animator->stop();

		this->orig_state = STATE::MAXIMIZED;
		this->state = STATE::MAXIMIZING;

		this->pos_animator->set_curr(this->get_position());
		this->pos_animator->set_src(this->pos_animator->get_curr());
		this->pos_animator->set_dst(this->parent->get_base());

		this->size_animator->set_curr(this->get_size());
		this->size_animator->set_src(this->size_animator->get_curr());
		this->size_animator->set_dst(this->parent->get_size() - this->parent->get_base());

		this->pos_animator->start();
		this->size_animator->start();
	}

	void Window::handle_event(Event& e) {
		if (this->grabbed_move && e.mouse_moved) {																							// If the user grabbed the window and moved the mouse
			this->set_position(float2(e.x, e.y) - this->move_offset);																		// Reposition the window
			this->orig_pos = this->get_position();																							// To enlarge the window to the new position from minimized state
		}
		if (this->grabbed_resize && e.mouse_moved) {																						// If the user grabbed the little triangle in the window's bottom right right corner and moved the mouse
			//float2 min_size = float2(this->btn_title->get_label()->get_text().size() * this->char_width + 2 * this->char_width + 75, 30);
			float2 min_size = this->calc_minimized_size() + float2(75, 10);
			float2 temp = float2(e.x, e.y) - this->resize_offset;
			this->set_size(float2(std::max(temp.x, min_size.x), std::max(temp.y, min_size.y)));														// Resize the window
			this->orig_size = float2(this->get_size());
		}
		if (e.mouse_is_inside && !e.mouse_is_covered && e.mouse_pressed && e.mouse_left &&
			this->resizable && this->state == STATE::NORMALIZED) {									// Check if the user clicked with the left mosue on the triangle in the window's bottom right corner to start resizing
			float2 p0 = this->get_absolute_position() +
				this->get_size();

			float2 p1 = p0 - float2(10 + 5, 0);
			float2 p2 = p0 - float2(0, 10 + 5);

			float A = 0.5 *(-p1.y*p2.x + p0.y*(-p1.x + p2.x) + p0.x*(p1.y - p2.y) + p1.x*p2.y);
			float s = 1 / (2 * A)*(p0.y*p2.x - p0.x*p2.y + (p2.y - p0.y)*e.x + (p0.x - p2.x)*e.y);
			float t = 1 / (2 * A)*(p0.x*p1.y - p0.y*p1.x + (p0.y - p1.y)*e.x + (p1.x - p0.x)*e.y);

			if (s > 0 && t > 0 && 1 - s - t > 0) {																							// Check if the point is inside the triangle (barycentric)
				this->resize_offset = float2(e.x, e.y) - this->get_size();
				this->grabbed_resize = true;
			}
		}
		if (e.mouse_released && e.mouse_left) {
			this->grabbed_resize = false;
		}
	}

	void Window::draw(bool draw_background) {
		this->pos_animator->update();																						// Update the animator
		this->size_animator->update();																						// Update the animator

		if (this->state == STATE::MINIMIZING || this->state == STATE::NORMALIZING || this->state == STATE::MAXIMIZING) {	// If the window is animating
			this->set_position(this->pos_animator->get_curr());																// Set the position according to the animator
			this->set_size(this->size_animator->get_curr());																// Set the size according to the animator
		} else switch (this->state) {																						// Else if the window is not animating
			case STATE::MINIMIZED:
			{
				this->set_position(float2(this->get_position().x, std::max(this->parent->get_height() - 20, 20.0f)));		// Handle if parent component changes size
				if (this->calc_minimized_pos().x != this->get_position().x) {												// Handle if parent component has less minized windows than before
					this->minimize();
				}
				break;
			}
			case STATE::NORMALIZED:
			{
				this->set_position(this->orig_pos);																			// Set the original position
				this->set_size(this->orig_size);																			// And size
				break;
			}
			case STATE::MAXIMIZED:
			{
				this->set_position(this->parent->get_base());																// Position is the top left corner of the parent component
				this->set_size(this->parent->get_size() - this->parent->get_base());										// Size is the size of the parent component
				break;
			}
		}
		
		Panel::draw(draw_background);

		if (this->resizable) {																								// Draw a triangle in the bottom right corner to indicate that the window is resizable
			float2 pos = this->get_absolute_position() +																	// Bottom right corner's position
				this->get_size() -
				float2(this->default_border_width, this->default_border_width);

			Color c = this->get_border_color();
			glColor4f(c.R, c.G, c.B, c.A);
			glBegin(GL_TRIANGLES);
			glVertex2f(pos.x, pos.y);
			glVertex2f(pos.x - 10, pos.y);
			glVertex2f(pos.x, pos.y - 10);
			glEnd();
		}

		// Bring to the front to draw last(hence it is in front of every object in the window)
		this->btn_close->bring_front();
		this->btn_maximize->bring_front();
		this->btn_minimize->bring_front();
		this->btn_title->bring_front();

		this->btn_close->set_visible(this->closable);			// Hide the controls if it cannot be used
		this->btn_maximize->set_visible(this->maximizable);
		this->btn_minimize->set_visible(this->minimizable);

		std::vector<Button*> controls;
		if (this->minimizable) {
			controls.push_back(this->btn_minimize);
		}
		if (this->maximizable) {
			controls.push_back(this->btn_maximize);
		}
		if (this->closable) {
			controls.push_back(this->btn_close);
		}

		this->btn_title->set_size(float2(std::max(this->get_width() - 25 * controls.size() + this->default_border_width * controls.size(),	// Calc width
												  this->calc_minimized_size().x),															// The width of the minimized window
										 20));

		for (int i = 0; i < controls.size(); ++i) {
			controls[i]->set_position(float2(this->btn_title->get_size().x + 25*i - this->default_border_width * (i+1), 0));				// Set the allowed controls' positions
		}
	}

	// To listen on inner component actions
	void Window::action_performed(void* sender, ActionEvent& e) {
		if (sender == this->pos_animator || sender == this->size_animator) {						// If one of the animators
			if (e.animator_reached) {																// Reached it's destination
				switch (this->state) {																// Finish the animation
					case STATE::MINIMIZING: this->state = STATE::MINIMIZED;
						break;
					case STATE::NORMALIZING: this->state = STATE::NORMALIZED;
						break;
					case STATE::MAXIMIZING: this->state = STATE::MAXIMIZED;
						break;
				}
			}
		}
		if (sender == this->btn_title) {															// If the title
			if (e.button_pressed) {																	// Was pressed
				if (this->state == STATE::NORMALIZED) {												// And the window is normalized
					if (this->movable) {															// And the window can be moved,
						this->move_offset = (float2(e.x, e.y) - this->get_absolute_position() +		// Calculate the mouse position relative the window's parent's absolute position
											 this->parent->get_absolute_position());
						this->grabbed_move = true;													// Because the user possibly wants to move the window with the mouse
					}
				}
			} else if (e.button_released) {
				this->grabbed_move = false;															// The user released the title

				if (this->state == STATE::MINIMIZED || this->state == STATE::MINIMIZING) {			// If the window was minimized, start enlarging it to it's original state
					this->parent->remove_minimized_window(this);
					if (this->orig_state == STATE::NORMALIZED) {
						this->normalize();
					} else if (this->orig_state == STATE::MAXIMIZED) {
						this->maximize();
					}
				}
			}
		}
		if (sender == this->btn_minimize) {															// If the minimize button
			if (e.button_released) {																// Was pressed,
				this->parent->add_minimized_window(this);											// Start
				this->minimize();																	// Minimizing the window
			}
		}
		if (sender == this->btn_maximize) {															// If the maximize button
			if (e.button_released) {																// Was pressed,
				if (this->state == STATE::NORMALIZED) {												// Start
					this->maximize();																// Maximizing or
				} else if (this->state == STATE::MAXIMIZED) {										// Normalizing the window
					this->normalize();																// According it's previous state
				}
			}
		}
		if (sender == this->btn_close) {															// If the close button
			if (e.button_released) {																// Was pressed,
				this->set_visible(false);															// Hide the window
			}
		}
	}

	// Gets the base position
	float2 Window::get_base() {
		return float2(0, 20 - this->default_border_width);
	}

	// Title, what is displayed in the top left corner, the coordinates, the size, and the border's width
	Window::Window(std::string title, float x, float y, float width, float height, float border_width) : Panel(true, x, y, width, height, border_width) {
		this->state = STATE::NORMALIZED;
		this->orig_state = this->state;

		this->orig_pos = float2(x, y);
		this->orig_size = float2(width, height);
		this->move_offset = float2(0, 0);
		this->resize_offset = float2(0, 0);
		this->grabbed_move = false;
		this->grabbed_resize = false;
		this->movable = true;
		this->resizable = true;
		this->minimizable = true;
		this->maximizable = true;
		this->closable = true;

		this->btn_title = new Button(" " + title + " ", 0, 0, width - 75 + border_width * 3, 20, border_width);
		this->btn_title->get_label()->set_h_align(Label::H_ALIGN::LEFT);
		this->btn_title->set_wait_time(Duration<Time::HOURS>(24));
		this->btn_title->add_action_listener(this);
		this->add_component(this->btn_title);

		this->btn_minimize = new Button("-", width - 75 + border_width * 2, 0, 25, 20, border_width);
		this->btn_minimize->set_wait_time(Duration<Time::HOURS>(24));
		this->btn_minimize->add_action_listener(this);
		this->add_component(this->btn_minimize);

		this->btn_maximize = new Button("[]", width - 50 + border_width, 0, 25, 20, border_width);
		this->btn_maximize->set_wait_time(Duration<Time::HOURS>(24));
		this->btn_maximize->add_action_listener(this);
		this->add_component(this->btn_maximize);

		this->btn_close = new Button("X", width - 25, 0, 25, 20, border_width);
		this->btn_close->set_wait_time(Duration<Time::HOURS>(24));
		this->btn_close->add_action_listener(this);
		this->add_component(this->btn_close);

		this->pos_animator = new Animator();
		this->pos_animator->set_anim_time(Duration<Time::MILLI_SECONDS>(500));
		this->pos_animator->add_action_listener(this);

		this->size_animator = new Animator();
		this->size_animator->set_anim_time(Duration<Time::MILLI_SECONDS>(500));
		this->size_animator->add_action_listener(this);
	}

	// Sets the movability
	void Window::set_movable(bool movable) {
		this->movable = movable;
	}

	// Sets the resizability
	void Window::set_resizable(bool resizable) {
		this->resizable = resizable;
	}

	// Sets the minimizability
	void Window::set_minimizable(bool minimizable) {
		this->minimizable = minimizable;
	}

	// Sets the maximizability
	void Window::set_maximizable(bool maximizable) {
		this->maximizable = maximizable;
	}

	// Sets the closability
	void Window::set_closable(bool closable) {
		this->closable = closable;
	}

	// Sets the title of the panel
	void Window::set_title(std::string title) {
		this->btn_title->get_label()->set_text(" " + title + " ");
	}

	// Gets the title of the panel
	std::string Window::get_title() {
		std::string title = this->btn_title->get_label()->get_text();
		return title.substr(1, title.size() - 2);
	}

	// Returns true if movable
	bool Window::is_movable() {
		return this->movable;
	}

	// Returns true resizable
	bool Window::is_resizable() {
		return this->resizable;
	}

	// Returns true if minimizable
	bool Window::is_minimizable() {
		return this->minimizable;
	}

	// Returns true if maximizable
	bool Window::is_maximizable() {
		return this->maximizable;
	}

	// Return true if closable
	bool Window::is_closable() {
		return this->closable;
	}

}