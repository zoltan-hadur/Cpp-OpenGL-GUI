#pragma once

#include "Component.h"

namespace GLUI {

	// Panel, that can store and display various components, also can be moved with the mouse if the panel has a title
	class Window : public EventListener, public Panel {
	protected:
		Button* btn_title;	// Title of the panel
		Button* btn_collapse;
		Button* btn_minimize;
		float2 pos_offset;	// For moving the window
		bool dragged;		// Becomes true when the user clicks on the title button, and false when releases the left mouse button

		Stopwatch watch;
		float acc;
		float acc2;
		float offset;
		float orig_height;
		float anim_time;
		bool collapsing;
		bool collapsed;
		bool animating;

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
			}
		}
	}

	void Window::draw(bool draw_background) {
		//std::string title = this->btn_title->get_label()->get_text();				// Get the title
		//if (title == "" || title == " ") {											// Don't draw it if it's empty
		//	this->btn_title->set_visible(false);
		//}

		if (this->animating && this->watch.is_running()) {
			float dt = this->collapsing ? -this->watch.get_delta_time() : this->watch.get_delta_time();
			this->acc2 = this->acc2 + std::fabs(dt);
			//float speed = this->main_tab->get_width() / this->anim_time;
			//float speed = 20*(std::fabs(this->offset - this->acc) * this->watch.get_elapsed_time()) / this->anim_time + 10;
			float speed = 20 * (std::fabs(this->offset - this->acc) * this->acc2) / this->anim_time + 20;
			//printf("%f %f\n", speed, acc2);

			this->acc = this->acc + speed*dt;

			if ((this->collapsing && this->acc < this->offset) || (!this->collapsing && this->acc > this->offset)) {
				this->acc = this->offset;
				this->acc2 = 0;
				this->watch.stop();
				this->animating = false;
				this->collapsed = collapsing;
			}

			this->height = this->orig_height + acc;
		}

		this->set_use_scissor(true);

		this->set_visible(!this->collapsed);
		if (this->collapsed) {
			this->visible = true;
			this->btn_title->set_visible(true);
			this->btn_collapse->set_visible(true);
			this->btn_minimize->set_visible(true);
		}

		if (this->collapsing) {
			this->btn_collapse->get_label()->set_text(R"(\/)");
		} else {
			this->btn_collapse->get_label()->set_text(R"(/\)");
		}

		Panel::draw(draw_background);
	}

	// To listen on inner component events
	void Window::action_performed(void* sender, Event& e) {
		if (sender == this->btn_title) {		// If the title
			if (e.button_pressed) {				// Was pressed
				this->dragged = true;			// Then the user probably going to drag the panel to somewhere else
				this->pos_offset = float2(e.x, e.y) - this->get_absolute_position();
			} else if (e.button_released) {		// The user finished
				this->dragged = false;			// Dragging the window
			}
		}
		if (sender == this->btn_collapse) {
			if (e.button_pressed) {
				this->collapsing = !this->collapsing;
				this->animating = true;
				this->acc2 = 0;
				this->watch.start();
				this->offset = this->collapsing ? 20 - this->orig_height : this->offset + this->orig_height;
				if (this->collapsed && !this->collapsing) {
					this->collapsed = false;
				}
				//this->collapsed = this->collapsed == this->collapsing;
			}
		}
		if (sender == this->btn_minimize) {

		}
	}

	// Title, what is displayed in the top left corner, the coordinates, the size, and the border's width
	Window::Window(std::string title, float x, float y, float width, float height, float border_width) : Panel(true, x, y, width, height, border_width) {
		this->watch = Stopwatch();
		this->acc = 0;
		this->acc2 = 0;
		this->orig_height = height;
		this->anim_time = 1;
		this->collapsing = false;
		this->collapsed = false;
		this->animating = false;

		this->dragged = false;
		this->btn_title = new Button(" " + title, 0, 0, width - 50 + border_width * 2, 20, border_width);
		this->btn_title->get_label()->set_h_align(Label::H_ALIGN::LEFT);
		this->btn_title->set_wait_time(std::numeric_limits<float>::max());
		this->btn_title->add_event_listener(this);
		this->add_component(this->btn_title);

		this->btn_collapse = new Button(R"(\/)", width - 25, 0, 25, 20, border_width);
		this->btn_collapse->add_event_listener(this);
		this->add_component(this->btn_collapse);

		this->btn_minimize = new Button("_", width - 50 + border_width, 0, 25, 20, border_width);
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