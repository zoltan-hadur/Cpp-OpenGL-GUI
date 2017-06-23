#pragma once

#include "Component.h"
#include "Button.h"
#include "Label.h"
#include "ScrollPanel.h"

namespace GLUI {

	// Combobox with selectable drop down list, that can raise events when selected changes
	class ComboBox : public EventListener, public Panel {
	protected:
		enum class D_STATE {				// States of combobox list dropping down
			DROPPED_DOWN,
			ROLLING_UP,
			ROLLED_UP,
			DROPPING_DOWN
		};

		D_STATE d_state;					// Stores that the drop down list is dropped down, dropping down, rolled up or rolling up

		Label* lbl_selected;				// Displays the selected value
		Button* btn_drop_down;				// The drop down list drops when pressed, and rolls up when pressed again
		ScrollPanel* scp_list;				// The scrollable drop down panel with the selectable elements
		std::list<Button*> btn_elements;	// The selectable elements from the drop down list

		float element_offset;				// For element addition alignment

		float2 size_offset;					// Size offset for animation
		float2 orig_size;					// Original size for animation

		Stopwatch watch;					// Timer for animations
		float2 acc_size;					// Stores accumulated size value for animation
		float anim_time;					// Determines how long the animation last (not accurate because of non linearity)

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		// To listen on the children component's events
		virtual void action_performed(void* sender, Event& e) override;
		// The coordinates of the combo box, the size of the combo box, and the border's width of the combo box
		ComboBox(float x = 0, float y = 0, float width = 100, float height = 20, float border_width = 1);
		// Adds an element to the combo box
		void add_element(std::string text);
		// Removes an element from the combo box (or multiple, if there are multiplte elements with the same name)
		void remove_element(std::string text);
		// Returns the selected element's index
		int get_selected_index();
		// Return the selected element
		std::string get_selected_element();
		// Return the index-th element
		std::string get_selected_element(int index);
	};

	void ComboBox::handle_event(Event& e) {

	}

	void ComboBox::draw(bool draw_background) {
		this->scp_list->set_visible(d_state != D_STATE::ROLLED_UP);											// Hide the scroll panel when the combobox is rolled up

		if (this->watch.is_running()) {																		// Animate if the watch is running
			float dt = this->watch.get_delta_time();														// Get delta time
			float et = this->watch.get_elapsed_time();														// Get elapsed time

			float2 speed_size = (this->size_offset - this->acc_size) * 20 * et / this->anim_time;			// Calculate the current speed to modify the combobox's size

			this->acc_size = this->acc_size + speed_size * dt;												// Accumulate the combobox's size

			bool x_in_size = false;
			bool y_in_size = false;
			if (std::fabs(this->size_offset.x - this->acc_size.x) < 1.0f) {									// Check if combobox's width has reached it's target value
				this->acc_size.x = this->size_offset.x;
				x_in_size = true;
			}
			if (std::fabs(this->size_offset.y - this->acc_size.y) < 1.0f) {									// Check if combobox's height has reached it's target value
				this->acc_size.y = this->size_offset.y;
				y_in_size = true;
			}

			this->set_size(this->orig_size.x + this->acc_size.x, this->orig_size.y + this->acc_size.y);		// Modifiy the combobox's size according to the animation

			if (x_in_size && y_in_size) {																	// If the combobox's size reached it's target value
				this->watch.stop();																			// The animation is over

				if (this->d_state == D_STATE::ROLLING_UP) {													// Set the states
					this->d_state = D_STATE::ROLLED_UP;
				}
				if (this->d_state == D_STATE::DROPPING_DOWN) {
					this->d_state = D_STATE::DROPPED_DOWN;
				}
			}
		}

		Panel::draw(draw_background);
	}

	// To listen on the children component's events
	void ComboBox::action_performed(void* sender, Event& e) {
		if (sender == this->btn_drop_down) {																// If the drop down button
			if (e.button_released) {																		// Was pressed
				if (this->d_state == D_STATE::ROLLED_UP || this->d_state == D_STATE::ROLLING_UP) {			// Start dropping down the list if it was rolling up or rolled up
					this->size_offset = float2(0, 0);
					this->d_state = D_STATE::DROPPING_DOWN;
					this->btn_drop_down->get_label()->set_text(R"(/\)");
				} else {																					// Else start rolling up the list
					this->size_offset = float2(this->width, 20) - this->orig_size;
					this->d_state = D_STATE::ROLLING_UP;
					this->btn_drop_down->get_label()->set_text(R"(\/)");
				}

																											// Reset the watch at every button press
				if (this->watch.is_running()) {																// If the watch is running
					this->watch.reset();																	// Reset it
				} else {																					// Else
					this->watch.start();																	// Start it
				}
			}
		} else {																							// If some of the buttons in the list
			if (e.button_released) {																		// Was pressed
				std::string text =
					((Button*)sender)->get_label()->get_text();												// Get the selected item's text
				if (text != this->lbl_selected->get_text()) {												// If the previously selected item differs from the one selected now
					this->lbl_selected->set_text(text);														// Change the text
					e.combobox_changed = true;
					e.combobox_selected = text;
					this->raise_event(this, e);																// Raise an event that the selected item changed
					e.combobox_changed = false;
				}

				this->size_offset = float2(this->width, 20) - this->orig_size;								// Start rolling up the list
				this->d_state = D_STATE::ROLLING_UP;
				this->btn_drop_down->get_label()->set_text(R"(\/)");

																											// Reset the watch at every button press
				if (this->watch.is_running()) {																// If the watch is running
					this->watch.reset();																	// Reset it
				} else {																					// Else
					this->watch.start();																	// Start it
				}
			}
		}
	}

	// The coordinates of the combo box, the size of the combo box, and the border's width the of the combo box
	ComboBox::ComboBox(float x, float y, float width, float height, float border_width) : Panel(true, x, y, width, 20, border_width) {
		this->d_state = D_STATE::ROLLED_UP;
		this->element_offset = 0;
		this->orig_size = float2(width, height);
		this->size_offset = float2(this->width, 20)- this->orig_size;
		this->watch = Stopwatch();
		this->acc_size = this->size_offset;
		this->anim_time = 0.5;

		this->background_color = Color(120, 120, 120, 255);		// Default grey color
		this->lbl_selected = new Label("", 0, 0, width - 20, 20);
		this->btn_drop_down = new Button(R"(\/)", width - 20, 0, 20, 20);
		this->btn_drop_down->add_event_listener(this);
		this->scp_list = new ScrollPanel(ScrollPanel::ALIGN::VERTICAL, 0, 19, width, height - 19, border_width);
		this->scp_list->get_scroll_bar()->set_increment(20 + border_width * 2);
		this->scp_list->set_visible(false);
		this->add_component(this->lbl_selected);
		this->add_component(this->btn_drop_down);
		this->add_component(this->scp_list);
	}

	// Adds an element to the combo box
	void ComboBox::add_element(std::string text) {
		Button* btn = new Button(text,
								 this->default_border_width + 2, this->default_border_width + this->element_offset + 2,	// X, y
								 this->width - this->default_border_width * 2 - 3 - 20, 20);							// Width, height
		btn->set_wait_time(std::numeric_limits<float>::max());
		btn->add_event_listener(this);
		this->element_offset = this->element_offset + this->scp_list->get_scroll_bar()->get_increment();
		this->btn_elements.push_back(btn);
		this->scp_list->add_component(btn);
	}

	// Removes an element from the combo box (or multiple, if there are multiplte elements with the same name)
	void ComboBox::remove_element(std::string text) {
		for (auto c : this->btn_elements) {						// Iterate through all the elements
			if (c->get_label()->get_text() == text) {			// If the texts are matching
				c->remove_event_listener(this);					// Remove the event listener from the combobox
				this->remove_component(c);						// Remove the component from the combobox
				this->btn_elements.remove(c);					// Remove the element from the combobox
				break;
			}
		}
	}

	// Returns the selected element's index
	int ComboBox::get_selected_index() {
		int i = -1;
		for (auto btn : this->btn_elements) {
			++i;
			if (btn->get_label()->get_text() == this->lbl_selected->get_text()) {
				return i;
			}
		}
		return i;
	}

	// Return the selected element
	std::string ComboBox::get_selected_element() {
		return this->lbl_selected->get_text();
	}

	// Return the index-th element
	std::string ComboBox::get_selected_element(int index) {
		int i = -1;
		for (auto btn : this->btn_elements) {
			++i;
			if (i == index) {
				return btn->get_label()->get_text();
			}
		}
		throw "Index out of array";
	}

}