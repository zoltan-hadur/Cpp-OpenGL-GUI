#pragma once

#include "Panel.h"
#include "Slider.h"

namespace GLUI {

	// Scrollable panel, with either horizontal or vertical alignment
	// The scrollable area is dynamic, but only in according to the alignment
	class ScrollPanel : public EventListener, public Panel {
	protected:
		enum class ALIGN;

		float2 max_pos;			// Dynamically gets calcualted to determine how far the user can scroll
		Slider* scroll_bar;		// The scroll bar
		ALIGN align;			// Determines the alignment, either horizontal of vertical

		virtual void handle_event(Event& e) override;
		virtual void draw() override;
		virtual void render() override;
	public:
		enum class ALIGN {
			HORIZONTAL,			// Horizontal alignment
			VERTICAL			// Vertical alignment
		};

		// Alignment, coordinates, size, border's width
		ScrollPanel(ALIGN align, float x = 0, float y = 0, float width = 100, float height = 100, float border_width = 2);
		// Gets the scroll bar
		Slider* get_scroll_bar();
		// To listen on the children component's events
		virtual void action_performed(void* sender, Event& e) override;
	};

	void ScrollPanel::handle_event(Event& e) {
		float2 pos = this->get_absolute_position();											// Get the absolute position relative to the top-level window
		if (pos.x < e.x && e.x < pos.x + width && pos.y < e.y && e.y < pos.y + height) {	// Check if the mouse is above the panel
			if (visible) {																	// Check if the panel is visible
				if (e.mouse_scroll_up && e.mouse_pressed) {									// Check if the user moved the mouse wheel to scroll
					scroll_bar->dec_value();												// Then scroll
				}
				if (e.mouse_scroll_down && e.mouse_pressed) {								// Check if the user moved the mouse wheel to scroll
					scroll_bar->inc_value();												// Then scroll
				}
			}
		}
	}

	void ScrollPanel::draw() {
		for (auto c : children) {															// Get the farthest components bottom right corner's absolute coordinates relative to the scroll panel
			float2 pos = c->get_absolute_position() - this->get_absolute_position();		// Get absolute position relative to the scroll panel
			pos = pos + float2(c->get_width(), c->get_height());							// Bottom right corner
			pos = pos + float2(default_border_width, default_border_width) + float2(c->get_default_border_width(), c->get_default_border_width());
			if (pos.x > max_pos.x) {
				max_pos.x = pos.x;
			}
			if (pos.y > max_pos.y) {
				max_pos.y = pos.y;
			}
		}

		switch (align) {																	// Set the scrollable area according to the farthest component and to the alignment
			case ALIGN::HORIZONTAL:
				scroll_bar->set_max(std::max(max_pos.x - width, 0.0f));						// If the farthest object can fit into the panel, no need for scrolling, thus the 0
				break;
			case ALIGN::VERTICAL:
				scroll_bar->set_max(std::max(max_pos.y - height, 0.0f));
				break;
		}

		for (auto c : children) {															// Hide every component that is outside of the panel's area
			if (c != scroll_bar) {															// Except the scroll bar
				float2 pos = c->get_position();
				switch (align) {
					case ALIGN::HORIZONTAL:
						if (pos.x + c->get_width() < 0 + default_border_width || pos.x > width - default_border_width) {
							c->set_visible(false);
						} else {
							c->set_visible(true);
						}
						break;
					case ALIGN::VERTICAL:
						if (pos.y + c->get_height() < 0 + default_border_width || pos.y > height - default_border_width) {
							c->set_visible(false);
						} else {
							c->set_visible(true);
						}
						break;
				}
			}
		}

		Component::draw();
	}

	void ScrollPanel::render() {
		if (visible) {
			glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();								// Save current projection matrix
			glOrtho(0.0f, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0.0f, -1.0f, 1.0f);	// Transform it to able to draw in pixel coordinates
			glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();								// Save current modelview matrix
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glEnable(GL_BLEND);						// Enable transparency

			this->draw();
			scroll_bar->render();

			float2 pos = this->get_absolute_position();
			pos.x = pos.x + default_border_width;
			pos.y = glutGet(GLUT_WINDOW_HEIGHT) - (pos.y + height - default_border_width);
			glScissor(pos.x, pos.y, width - default_border_width * 2, height - default_border_width * 2);
			glEnable(GL_SCISSOR_TEST);
			for (auto c : children) {
				if (c != scroll_bar) {
					c->render();
				}
			}
			glDisable(GL_SCISSOR_TEST);

			glDisable(GL_BLEND);
			glPopMatrix(); glMatrixMode(GL_PROJECTION);
			glPopMatrix(); glMatrixMode(GL_MODELVIEW);
		}
	}

	// Alignment, coordinates, size, border's width
	ScrollPanel::ScrollPanel(ALIGN align, float x, float y, float width, float height, float border_width) : Panel("", x, y, width, height, border_width) {
		max_pos = float2(0, 0);
		this->align = align;
		switch (align) {						// Position the slider in the panel according to it's alignment
			case ALIGN::HORIZONTAL:
				this->scroll_bar = new Slider(0, 0, border_width / 2, height - 20 - border_width / 2, width - border_width, 20);
				break;
			case ALIGN::VERTICAL:
				this->scroll_bar = new Slider(0, 0, width - 20 - border_width / 2, border_width / 2, 20, height - border_width);
				break;
		}
		scroll_bar->set_increment(20);			// 20 pixel is not too small, not too big
		scroll_bar->add_event_listener(this);	// To listen the slider's events
		this->add_component(scroll_bar);
	}

	// Gets the scroll bar
	Slider* ScrollPanel::get_scroll_bar() {
		return scroll_bar;
	}

	// To listen on the children component's events
	void ScrollPanel::action_performed(void* sender, Event& e) {
		for (auto c : children) {											// Translate every component
			if (c != scroll_bar) {											// Except the scroll bar
				float2 pos = c->get_position();								// According to the delta x/y of the slider
				switch (align) {
					case ALIGN::HORIZONTAL:
						c->set_position(pos.x + e.slider_dvalue, pos.y);
						break;
					case ALIGN::VERTICAL:
						c->set_position(pos.x, pos.y + e.slider_dvalue);
						break;
				}
			}
		}
	}

}