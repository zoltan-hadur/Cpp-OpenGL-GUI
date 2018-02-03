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

	// ComboBox with selectable drop down list, that can perform actions when selected item changes
	class ComboBox : public ActionListener, public Panel {
	protected:
		enum class STATE {				// States of combobox list dropping down
			DROPPED_DOWN,
			ROLLED_UP,
			DROPPING_DOWN,
			ROLLING_UP
		};

		STATE state;						// States of the ComboBox

		Label* lbl_selected;				// Displays the selected item's value
		Button* btn_drop_down;				// The drop down list drops when pressed, and rolls up when pressed again
		ScrollPanel* scp_list;				// The scrollable drop down panel with the selectable items
		std::vector<Button*> btn_items;		// The selectable items from the drop down list

		Animator* pos_animator;
		Animator* size_animator;

		float2 orig_pos;					// Original position for animation
		float2 orig_size;					// Original size for animation

		Duration<Time::SECONDS> anim_time;	// Length of the animation

		float item_offset;					// For item addition alignment

		void drop_down();
		void roll_up();

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

	void ComboBox::drop_down() {
		//this->pos_animator->stop();
		this->size_animator->stop();

		this->state = STATE::DROPPING_DOWN;
		this->btn_drop_down->get_label()->set_text(R"(/\)");

		//this->pos_animator->set_curr(this->get_position());
		//this->pos_animator->set_src(this->pos_animator->get_curr());
		//this->pos_animator->set_dst(this->orig_pos);

		this->size_animator->set_curr(this->get_size());
		this->size_animator->set_src(this->size_animator->get_curr());
		this->size_animator->set_dst(this->orig_size + float2(0, 200));

		//this->pos_animator->start();
		this->size_animator->start();
	}

	void ComboBox::roll_up() {
		//this->pos_animator->stop();
		this->size_animator->stop();

		this->state = STATE::ROLLING_UP;
		this->btn_drop_down->get_label()->set_text(R"(\/)");

		//this->pos_animator->set_curr(this->get_position());
		//this->pos_animator->set_src(this->pos_animator->get_curr());
		//this->pos_animator->set_dst(this->orig_pos);

		this->size_animator->set_curr(this->get_size());
		this->size_animator->set_src(this->size_animator->get_curr());
		this->size_animator->set_dst(this->orig_size);

		//this->pos_animator->start();
		this->size_animator->start();
	}

	void ComboBox::handle_event(Event& e) {

	}

	void ComboBox::draw(bool draw_background) {
		//this->pos_animator->update();																						// Update the animator
		this->size_animator->update();																						// Update the animator

		if (this->state == STATE::ROLLING_UP || this->state == STATE::DROPPING_DOWN) {										// If the ComboBox is animating
			//this->set_position(this->pos_animator->get_curr());																// Set the position according to the animator
			this->set_size(this->size_animator->get_curr());																// Set the size according to the animator
		} else switch (this->state) {																						// Else if the ComboBox is not animating
			case STATE::ROLLED_UP:
			{
				this->set_position(this->orig_pos);																			// Set the original position
				this->set_size(this->orig_size);																			// And size
				break;
			}
			case STATE::DROPPED_DOWN:
			{
				this->set_size(this->orig_size + float2(0, 200));
				break;
			}
		}

		this->scp_list->set_visible(state != STATE::ROLLED_UP);											// Hide the scroll panel when the combobox is rolled up


		Panel::draw(draw_background);
	}

	// To listen on the children component's actions
	void ComboBox::action_performed(void* sender, ActionEvent& e) {
		if (sender == this->pos_animator || sender == this->size_animator) {						// If one of the animators
			if (e.animator_reached) {																// Reached it's destination
				switch (this->state) {																// Finish the animation
					case STATE::DROPPING_DOWN: this->state = STATE::DROPPED_DOWN;
						break;
					case STATE::ROLLING_UP: this->state = STATE::ROLLED_UP;
						break;
				}
			}
		}
		if (sender == this->btn_drop_down) {
			if (e.button_released) {
				if (this->state == STATE::ROLLED_UP || this->state == STATE::ROLLING_UP) {
					this->drop_down();
				} else {
					this->roll_up();
				}
			}
		}
		for (auto item : this->btn_items) {
			if (sender == item) {
				if (e.button_released) {
					this->change_selected_item(item->get_label()->get_text());
					this->roll_up();
				}
			}
		}
	}

	// The coordinates of the combo box, the size of the combo box, and the border's width the of the combo box
	ComboBox::ComboBox(float x, float y, float width, float height, float border_width) : Panel(true, x, y, width, 20, border_width) {
		this->state = STATE::ROLLED_UP;
		this->item_offset = 0;
		this->orig_pos = float2(x, y);
		this->orig_size = float2(width, height);

		this->background_color = Color(120, 120, 120, 255) / 255;	// Default grey color

		this->lbl_selected = new Label("", 0, 0, width - 20, height);
		this->add_component(this->lbl_selected);

		this->btn_drop_down = new Button(R"(\/)", width - 20, 0, 20, height);
		this->btn_drop_down->set_wait_time(Duration<Time::HOURS>(24));
		this->btn_drop_down->add_action_listener(this);
		this->add_component(this->btn_drop_down);

		this->scp_list = new ScrollPanel(ScrollPanel::ALIGN::VERTICAL, 0, 19, width, 220 - 19, border_width);
		this->scp_list->get_scroll_bar()->set_increment(20 + border_width * 2);
		this->scp_list->set_visible(false);
		this->add_component(this->scp_list);

		this->pos_animator = new Animator();
		this->pos_animator->set_anim_time(Duration<Time::MILLI_SECONDS>(500));
		this->pos_animator->add_action_listener(this);

		this->size_animator = new Animator();
		this->size_animator->set_anim_time(Duration<Time::MILLI_SECONDS>(500));
		this->size_animator->add_action_listener(this);
	}

	// Adds an item to the combo box
	void ComboBox::add_item(std::string text) {
		//Button* btn = new Button(text,
		//						 this->default_border_width + 2, this->default_border_width + this->item_offset + 2,	// X, y
		//						 this->get_width() - this->default_border_width * 2 - 3 - 20, 20);						// Width, height
		//btn->set_default_border_width(0);
		//btn->set_background_color(this->scp_list->get_background_color());
		//btn->set_wait_time(Duration<Time::HOURS>(24));
		//btn->add_action_listener(this);
		//this->item_offset = this->item_offset + this->scp_list->get_scroll_bar()->get_increment();
		//this->btn_items.push_back(btn);
		//this->scp_list->add_component(btn);

		Button* btn = new Button(text,
								 this->default_border_width + 2, this->default_border_width + this->item_offset + 2,	// X, y
								 this->get_width() - this->default_border_width * 2 - 3 - 20, 20);						// Width, height
		btn->set_default_border_width(0);
		btn->set_background_color(this->scp_list->get_background_color());
		btn->set_wait_time(Duration<Time::HOURS>(24));
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