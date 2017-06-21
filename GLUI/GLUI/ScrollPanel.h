#pragma once

#include "Panel.h"
#include "Slider.h"

namespace GLUI {

	// Scrollable panel, with either horizontal or vertical alignment
	// The scrollable area is dynamic, but only in eaither horizontal or vertical space
	class ScrollPanel : public EventListener, public Panel {
	protected:
		enum class ALIGN;		// Forward declaration

		ALIGN align;			// Determines the alignment, either horizontal or vertical
		Slider* sld_scroll_bar;	// The scroll bar
		float2 max_pos;			// Dynamically gets calculated to determine how far the user can scroll

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		enum class ALIGN {		// Alignment values
			HORIZONTAL,			// Horizontal alignment
			VERTICAL			// Vertical alignment
		};

		// To listen on the children component's events
		virtual void action_performed(void* sender, Event& e) override;

		// Alignment, coordinates, size, border's width
		ScrollPanel(ALIGN align, float x = 0, float y = 0, float width = 100, float height = 100, float border_width = 2);
		// Gets the scroll bar
		Slider* get_scroll_bar();
	};

	void ScrollPanel::handle_event(Event& e) {
		float2 pos = this->get_absolute_position();														// Get the absolute position relative to the top-level window
		if (pos.x < e.x && e.x < pos.x + this->width && pos.y < e.y && e.y < pos.y + this->height) {	// Check if the mouse is above the panel
			if (!e.mouse_covered) {																		// Check if the panel is behind a component
				if (e.mouse_scroll_up && e.mouse_pressed) {												// Check if the user moved the mouse wheel to scroll
					this->sld_scroll_bar->dec_value();													// Then scroll
				}
				if (e.mouse_scroll_down && e.mouse_pressed) {											// Check if the user moved the mouse wheel to scroll
					this->sld_scroll_bar->inc_value();													// Then scroll
				}
			}
		}
	}

	void ScrollPanel::draw(bool draw_background) {
		this->set_use_scissor(true);
		this->set_draw_background(true);

		switch (this->align) {	// Position the slider in the panel according to it's alignment and it's size
			case ALIGN::HORIZONTAL:
				this->sld_scroll_bar->set_position(this->default_border_width / 2,						// X
											   this->height - 20 - this->default_border_width / 2);		// Y
				this->sld_scroll_bar->set_size(this->width - this->default_border_width,				// Width
										   20);															// Height
				break;
			case ALIGN::VERTICAL:
				this->sld_scroll_bar->set_position(this->width - 20 - this->default_border_width / 2,	// X
											   this->default_border_width / 2);							// Y
				this->sld_scroll_bar->set_size(20,														// Width
										   this->height - this->default_border_width);					// Height
				break;
		}

		for (auto c : this->children) {																	// Get the farthest components bottom right corner's absolute coordinates relative to the scroll panel
			float2 pos = c->get_absolute_position() - this->get_absolute_position();					// Get absolute position relative to the scroll panel
			pos = pos + float2(c->get_width(), c->get_height());										// Bottom right corner
			pos = pos + float2(this->default_border_width, this->default_border_width) + float2(c->get_default_border_width(), c->get_default_border_width());
			if (pos.x > this->max_pos.x) {
				this->max_pos.x = pos.x;
			}
			if (pos.y > this->max_pos.y) {
				this->max_pos.y = pos.y;
			}
		}

		switch (this->align) {																			// Set the scrollable area according to the farthest component and to the alignment
			case ALIGN::HORIZONTAL:
				this->sld_scroll_bar->set_max(std::max(this->max_pos.x - this->width, 0.0f));			// If the farthest object can fit into the panel, no need for scrolling, thus the 0
				break;
			case ALIGN::VERTICAL:
				this->sld_scroll_bar->set_max(std::max(this->max_pos.y - this->height, 0.0f));
				break;
		}

		for (auto c : this->children) {																	// Hide every component that is outside of the panel's area
			if (c != this->sld_scroll_bar) {															// Except the scroll bar
				float2 pos = c->get_position();
				switch (this->align) {
					case ALIGN::HORIZONTAL:
						if (pos.x + c->get_width() < 0 + this->default_border_width || pos.x > this->width - this->default_border_width) {
							c->set_visible(false);
						} else {
							c->set_visible(true);
						}
						break;
					case ALIGN::VERTICAL:
						if (pos.y + c->get_height() < 0 + this->default_border_width || pos.y > this->height - this->default_border_width) {
							c->set_visible(false);
						} else {
							c->set_visible(true);
						}
						break;
				}
			}
		}

		Panel::draw();
	}

	// To listen on the children component's events
	void ScrollPanel::action_performed(void* sender, Event& e) {
		for (auto c : this->children) {										// Translate every component
			if (c != this->sld_scroll_bar) {								// Except the scroll bar
				float2 pos = c->get_position();								// According to the delta x/y of the slider
				switch (this->align) {
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

	// Alignment, coordinates, size, border's width
	ScrollPanel::ScrollPanel(ALIGN align, float x, float y, float width, float height, float border_width) : Panel(true, x, y, width, height, border_width) {
		this->max_pos = float2(0, 0);
		this->align = align;
		switch (this->align) {						// Position the slider in the panel according to it's alignment
			case ALIGN::HORIZONTAL:
				this->sld_scroll_bar = new Slider(0, 0, border_width / 2, height - 20 - border_width / 2, width - border_width, 20);
				break;
			case ALIGN::VERTICAL:
				this->sld_scroll_bar = new Slider(0, 0, width - 20 - border_width / 2, border_width / 2, 20, height - border_width);
				break;
		}
		sld_scroll_bar->set_increment(20);			// 20 pixel is not too small, not too big
		sld_scroll_bar->add_event_listener(this);	// To listen the slider's events
		this->add_component(sld_scroll_bar);
	}

	// Gets the scroll bar
	Slider* ScrollPanel::get_scroll_bar() {
		return this->sld_scroll_bar;
	}

}