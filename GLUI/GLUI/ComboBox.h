#pragma once

#include "Component.h"
#include "Button.h"
#include "Label.h"
#include "ScrollPanel.h"

namespace GLUI {

	// Combobox with selectable drop down list, that can raise events when selected changes
	class ComboBox : public EventListener, public Component {
		float offset;					// For element addition alignment
	protected:
		bool dropped_down;				// Indicates if the drop down list is dropped down
		Label* selected;				// Displays the selected value
		Button* drop_down;				// The drop down list drops when pressed, and rolls up when pressed again
		ScrollPanel* panel;				// The scrollable drop down panel with the selectable elements
		std::list<Button*> elements;	// The selectable elements from the drop down list
	public:
		// The coordinates of the combo box, the size of the combo box, and the border's width the of the combo box
		ComboBox(float x = 0, float y = 0, float width = 100, float height = 20, float border_width = 2);
		// Adds an element to the combo box
		void add_element(std::string text);
		// Removes an element from the combo box (or multiple, if there are multiplte elements with the same name)
		void remove_element(std::string text);
		// To properly handle the drop down list
		virtual void set_visible(bool visible) override;
		// To listen on the children component's events
		virtual void action_performed(void* sender, Event& e) override;
	};

	// The coordinates of the combo box, the size of the combo box, and the border's width the of the combo box
	ComboBox::ComboBox(float x, float y, float width, float height, float border_width) : Component(x, y, width, height, border_width) {
		dropped_down = false;
		offset = 0;
		selected = new Label("", 0, 0, width - 20, 20);
		drop_down = new Button(R"(\/)", width - 20 - border_width / 2, border_width / 2, 20, 20 - border_width);
		drop_down->add_event_listener(this);
		panel = new ScrollPanel(ScrollPanel::ALIGN::VERTICAL, 0, height-border_width, width, 181, border_width);
		panel->get_scroll_bar()->set_increment(20 + border_width);
		panel->set_visible(false);
		this->add_component(selected);
		this->add_component(drop_down);
		this->add_component(panel);
	}

	// Adds an element to the combo box
	void ComboBox::add_element(std::string text) {
		Button* btn = new Button(text, default_border_width + 2, default_border_width + offset + 2, width - default_border_width*2 - 3 - 20, 20);
		btn->add_event_listener(this);
		offset = offset + 20 + default_border_width;
		elements.push_back(btn);
		panel->add_component(btn);
	}

	// Removes an element from the combo box (or multiple, if there are multiplte elements with the same name)
	void ComboBox::remove_element(std::string text) {
		for (auto c : elements) {
			if (c->get_label()->get_text() == text) {
				c->remove_event_listener(this);
				this->remove_component(c);
				elements.remove(c);
				break;
			}
		}
	}

	// To properly handle the drop down list
	void ComboBox::set_visible(bool visible) {
		if (visible) {								// If visible
			this->visible = true;					// The base
			this->selected->set_visible(true);		// The label
			this->drop_down->set_visible(true);		// And the button are 100% visible
			if (dropped_down) {						// The drop down list is only visible
				this->panel->set_visible(true);		// If the list is dropped down
			} else {								// Else
				this->panel->set_visible(false);	// It's the only not visible component
			}
		} else {
			Component::set_visible(false);			// If not visible, nothing is visible
		}
	}

	// To listen on the children component's events
	void ComboBox::action_performed(void* sender, Event& e) {
		if (sender == drop_down) {					// If the drop down button
			if (e.button_pressed) {					// Was pressed
				dropped_down = !dropped_down;		// The list opened if it was closed, and closed of it was opened
				this->set_visible(true);			// So need to refresh the visibility tree
			}
		} else {
			if (e.button_pressed) {					// A button in the drop down list was pressed
				selected->set_text(((Button*)(sender))->get_label()->get_text());	// So set selected item
				dropped_down = false;				// The drop down list automatically closes when an item is selected from it
				this->set_visible(true);			// Thus the refresh
				e.combobox_selected = selected->get_text();
				this->raise_event(this, e);			// Raise an event with the selected item
			}
		}
	}

}