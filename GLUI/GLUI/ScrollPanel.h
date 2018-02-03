#pragma once

#include "Panel.h"
#include "Slider.h"
#include "..\Event\ActionEvent.h"
#include "..\Event\ActionListener.h"
#include "..\Event\Event.h"
#include "..\Utility\float2.h"

namespace GLUI {

	// Scrollable panel, with either horizontal or vertical alignment
	// The scrollable area is dynamic, but only in either horizontal or vertical space
	class ScrollPanel : public ActionListener, public Panel {
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

		// To listen on the children component's actions
		virtual void action_performed(void* sender, ActionEvent& e) override;

		// Alignment, coordinates, size, border's width
		ScrollPanel(ALIGN align, float x = 0, float y = 0, float width = 100, float height = 100, float border_width = 2);
		// Gets the scroll bar
		Slider* get_scroll_bar();
	};

	void ScrollPanel::handle_event(Event& e) {
		if (e.mouse_is_inside && !e.mouse_is_covered) {		// If the mouse is inside the scroll panel and it's not behind an other component
			if (e.mouse_pressed && e.mouse_scroll_up) {		// If the user moved the mouse wheel to scroll
				this->sld_scroll_bar->dec_value();			// Then scroll
			}
			if (e.mouse_pressed && e.mouse_scroll_down) {	// If the user moved the mouse wheel to scroll
				this->sld_scroll_bar->inc_value();			// Then scroll
			}
		}
	}

	void ScrollPanel::draw(bool draw_background) {
		switch (this->align) {																			// Position the slider in the panel according to it's alignment and it's size
			case ALIGN::HORIZONTAL:
				this->sld_scroll_bar->set_position(float2(0,											// X
														  this->get_height() - 20));					// Y
				this->sld_scroll_bar->set_size(float2(this->get_width(),								// Width
													  20));												// Height
				break;
			case ALIGN::VERTICAL:
				this->sld_scroll_bar->set_position(float2(this->get_width() - 20,						// X
														  0));											// Y
				this->sld_scroll_bar->set_size(float2(20,												// Width
													  this->get_height()));								// Height
				break;
		}

		for (auto c : this->children) {																	// Get the farthest components bottom right corner's absolute coordinates relative to the scroll panel
			float2 pos = c->get_absolute_position() - this->get_absolute_position();					// Get absolute position relative to the scroll panel
			pos = pos + float2(c->get_width(), c->get_height());										// Bottom right corner
			//pos = pos + float2(this->default_border_width, this->default_border_width) + float2(c->get_default_border_width(), c->get_default_border_width());
			if (pos.x > this->max_pos.x) {
				this->max_pos.x = pos.x;
			}
			if (pos.y > this->max_pos.y) {
				this->max_pos.y = pos.y;
			}
		}

		switch (this->align) {																			// Set the scrollable area according to the farthest component and to the alignment
			case ALIGN::HORIZONTAL:
				this->sld_scroll_bar->set_max(std::max(this->max_pos.x - this->get_width(), 0.0f));		// If the farthest object can fit into the panel, no need for scrolling, thus the 0
				break;
			case ALIGN::VERTICAL:
				this->sld_scroll_bar->set_max(std::max(this->max_pos.y - this->get_height(), 0.0f));
				break;
		}
		//printf("%f %f \n\r", this->max_pos.x, this->max_pos.y);

		for (auto c : this->children) {																	// Hide every component that is outside of the panel's area
			if (c != this->sld_scroll_bar) {															// Except the scroll bar
				float2 pos = c->get_position();
				switch (this->align) {
					case ALIGN::HORIZONTAL:
						if (pos.x + c->get_width() < 0 + this->default_border_width || pos.x > this->get_width() - this->default_border_width) {
							c->set_visible(false);
						} else {
							c->set_visible(true);
						}
						break;
					case ALIGN::VERTICAL:
						if (pos.y + c->get_height() < 0 + this->default_border_width || pos.y > this->get_height() - this->default_border_width) {
							c->set_visible(false);
						} else {
							c->set_visible(true);
						}
						break;
				}
			}
		}

		Panel::draw(draw_background);
	}

	// To listen on the children component's actions
	void ScrollPanel::action_performed(void* sender, ActionEvent& e) {
		if (sender == this->sld_scroll_bar && e.slider_changed) {										// If the slider's value changed
			float2 dpos;
			switch (this->align) {
				case ALIGN::HORIZONTAL:
					dpos = float2(-e.slider_dvalue, 0);
					break;
				case ALIGN::VERTICAL:
					dpos = float2(0, -e.slider_dvalue);
					break;
			}
			for (auto c : this->children) {																// Translate every children component
				if (c != this->sld_scroll_bar) {														// Except the scroll bar itself
					c->set_position(c->get_position() + dpos);											// By the according dx dy value of the slider
				}
			}
		}
	}

	// Alignment, coordinates, size, border's width
	ScrollPanel::ScrollPanel(ALIGN align, float x, float y, float width, float height, float border_width) : Panel(true, x, y, width, height, border_width) {
		this->max_pos = float2(0, 0);
		this->align = align;
		switch (this->align) {																			// Position the slider in the panel according to it's alignment
			case ALIGN::HORIZONTAL:
				this->sld_scroll_bar = new Slider(0, 0, 0, height - 20, width, 20);
				break;
			case ALIGN::VERTICAL:
				this->sld_scroll_bar = new Slider(0, 0, width - 20, 0, 20, height);
				break;
		}
		sld_scroll_bar->set_increment(20);																// 20 pixel is not too small, not too big
		sld_scroll_bar->add_action_listener(this);														// To listen the slider's events
		this->add_component(sld_scroll_bar);
	}

	// Gets the scroll bar
	Slider* ScrollPanel::get_scroll_bar() {
		return this->sld_scroll_bar;
	}

}