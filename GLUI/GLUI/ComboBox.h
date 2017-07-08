#pragma once

#include <vector>
#include "Panel.h"
#include "Label.h"
#include "Button.h"
#include "ScrollPanel.h"
#include "..\Event\ActionEvent.h"
#include "..\Event\ActionListener.h"
#include "..\Event\ActionPerformer.h"
#include "..\Event\Event.h"
#include "..\Utility\float2.h"
#include "..\Utility\Color.h"
#include "..\Utility\Stopwatch.h"

namespace GLUI {

	// Combobox with selectable drop down list, that can perform actions when selected item changes
	class ComboBox : public ActionListener, public Panel {
	protected:
		enum class D_STATE {				// States of combobox list dropping down
			DROPPED_DOWN,
			ROLLING_UP,
			ROLLED_UP,
			DROPPING_DOWN
		};

		D_STATE d_state;					// Stores that the drop down list is dropped down, dropping down, rolled up or rolling up

		Label* lbl_selected;				// Displays the selected item's value
		Button* btn_drop_down;				// The drop down list drops when pressed, and rolls up when pressed again
		ScrollPanel* scp_list;				// The scrollable drop down panel with the selectable items
		std::vector<Button*> btn_items;		// The selectable items from the drop down list

		float item_offset;					// For item addition alignment

		float2 size_offset;					// Size offset for animation
		float2 orig_size;					// Original size for animation

		Stopwatch watch;					// Timer for animations
		float2 acc_size;					// Stores accumulated size value for animation
		float anim_time;					// Determines how long the animation last (not accurate because of non linearity)

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		// To listen on the children component's actions
		virtual void action_performed(void* sender, ActionEvent& e) override;

		// The coordinates of the combo box, the size of the combo box, and the border's width of the combo box
		ComboBox(float x = 0, float y = 0, float width = 100, float height = 20, float border_width = 1);

		// Adds an item to the combo box
		void add_item(std::string text);
		// Removes an item from the combo box (or multiple, if there are multiplte items with the same value)
		void remove_item(std::string text);
		// Selects item by index (DOES NOT performs any action)
		void select_item(int index);
		// Selects item by value (DOES NOT performs any action)
		void select_item(std::string text);
		// Returns the selected item
		std::string get_selected_item();
		// Returns the index-th item
		std::string get_item(int index);
		// Returns the selected item's index
		int get_selected_item_index();

		// Performs a selected item changed action
		void change_selected_item(int index);
		// Performs a selected item changed action
		void change_selected_item(std::string text);
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

	// To listen on the children component's actions
	void ComboBox::action_performed(void* sender, ActionEvent& e) {
		if (sender == this->btn_drop_down && e.button_released) {											// If the drop down button was pressed
			if (this->d_state == D_STATE::ROLLED_UP || this->d_state == D_STATE::ROLLING_UP) {				// Start dropping down the list if it was rolling up or rolled up
				this->size_offset = float2(0, 0);
				this->d_state = D_STATE::DROPPING_DOWN;
				this->btn_drop_down->get_label()->set_text(R"(/\)");
			} else {																						// Else start rolling up the list
				this->size_offset = float2(this->width, 20) - this->orig_size;
				this->d_state = D_STATE::ROLLING_UP;
				this->btn_drop_down->get_label()->set_text(R"(\/)");
			}

			if (this->watch.is_running()) {																	// If the watch is running
				this->watch.reset();																		// Reset it
			} else {																						// Else
				this->watch.start();																		// Start it
			}
		} else {																							// Else
			for (auto item : this->btn_items) {																// Find
				if (item == sender && e.button_released) {													// Wich button was pressed
					this->change_selected_item(item->get_label()->get_text());								// And then change the selected item

					this->size_offset = float2(this->width, 20) - this->orig_size;							// Start rolling up the list
					this->d_state = D_STATE::ROLLING_UP;
					this->btn_drop_down->get_label()->set_text(R"(\/)");

					if (this->watch.is_running()) {															// If the watch is running
						this->watch.reset();																// Reset it
					} else {																				// Else
						this->watch.start();																// Start it
					}
					break;
				}
			}
		}
	}

	// The coordinates of the combo box, the size of the combo box, and the border's width the of the combo box
	ComboBox::ComboBox(float x, float y, float width, float height, float border_width) : Panel(true, x, y, width, 20, border_width) {
		this->d_state = D_STATE::ROLLED_UP;
		this->item_offset = 0;
		this->orig_size = float2(width, height);
		this->size_offset = float2(this->width, 20) - this->orig_size;
		this->watch = Stopwatch();
		this->acc_size = this->size_offset;
		this->anim_time = 0.5;

		this->background_color = Color(120, 120, 120, 255) / 255;	// Default grey color
		this->lbl_selected = new Label("", 0, 0, width - 20, 20);
		this->btn_drop_down = new Button(R"(\/)", width - 20, 0, 20, 20);
		this->btn_drop_down->set_wait_time(std::numeric_limits<float>::max());
		this->btn_drop_down->add_action_listener(this);
		this->scp_list = new ScrollPanel(ScrollPanel::ALIGN::VERTICAL, 0, 19, width, height - 19, border_width);
		this->scp_list->get_scroll_bar()->set_increment(20 + border_width * 2);
		this->scp_list->set_visible(false);
		this->add_component(this->lbl_selected);
		this->add_component(this->btn_drop_down);
		this->add_component(this->scp_list);
	}

	// Adds an item to the combo box
	void ComboBox::add_item(std::string text) {
		Button* btn = new Button(text,
								 this->default_border_width + 2, this->default_border_width + this->item_offset + 2,	// X, y
								 this->width - this->default_border_width * 2 - 3 - 20, 20);							// Width, height
		btn->set_wait_time(std::numeric_limits<float>::max());
		btn->add_action_listener(this);
		this->item_offset = this->item_offset + this->scp_list->get_scroll_bar()->get_increment();
		this->btn_items.push_back(btn);
		this->scp_list->add_component(btn);
	}

	// Removes an item from the combo box (or multiple, if there are multiplte item with the same name)
	void ComboBox::remove_item(std::string text) {
		auto items = this->btn_items;
		for (auto item : items) {							// Iterate through all item
			if (item->get_label()->get_text() == text) {	// If the values are equals
				item->remove_action_listener(this);			// Remove the action listener from the combobox
				this->remove_component(item);				// Remove the item from the combobox
															// And remove the item from the items
				this->btn_items.erase(std::remove(this->btn_items.begin(), this->btn_items.end(), item), this->btn_items.end());
			}
		}
		if (items.size() == this->btn_items.size()) {
			throw "Item could not be removed, because the combobox does not contained the item!";
		}
	}

	// Selects item by index (DOES NOT performs any action)
	void ComboBox::select_item(int index) {
		this->lbl_selected->set_text(this->btn_items[index]->get_label()->get_text());
	}

	// Selects item by value (DOES NOT performs any action)
	void ComboBox::select_item(std::string text) {
		for (int i = 0; i < this->btn_items.size(); ++i) {
			if (this->btn_items[i]->get_label()->get_text() == text) {
				this->select_item(i);
				return;
			}
		}
		throw "Item could not be removed, because the combobox does not contained the item!";
	}

	// Returns the selected item
	std::string ComboBox::get_selected_item() {
		return this->lbl_selected->get_text();
	}

	// Returns the index-th item
	std::string ComboBox::get_item(int index) {
		return this->btn_items[index]->get_label()->get_text();
	}

	// Returns the selected item's index
	int ComboBox::get_selected_item_index() {
		for (int i = 0; i < this->btn_items.size(); ++i) {
			if (this->btn_items[i]->get_label()->get_text() == this->lbl_selected->get_text()) {
				return i;
			}
		}
		throw "Selected item's index not found, probably not initialized?";
	}

	// Performs a selected item changed action
	void ComboBox::change_selected_item(int index) {
		ActionEvent e;
		e.combobox_changed = this->get_item(index) != this->lbl_selected->get_text();
		this->select_item(index);
		e.combobox_selected_item = this->lbl_selected->get_text();
		e.combobox_selected_index = index;
		this->perform_action(this, e);
	}

	// Performs a selected item changed action
	void ComboBox::change_selected_item(std::string text) {
		ActionEvent e;
		e.combobox_changed = text != this->lbl_selected->get_text();
		this->select_item(text);
		e.combobox_selected_item = text;
		e.combobox_selected_index = this->get_selected_item_index();
		this->perform_action(this, e);
	}

}