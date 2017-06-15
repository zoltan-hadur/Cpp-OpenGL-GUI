#pragma once

#include "Component.h"

namespace GLUI {

	// A checkbox that can raise events when checked/unchecked
	class CheckBox : public EventListener, public Component {
	protected:
		Button* btn_check;								// The checkbox
		bool checked;									// Becomes true when the user clicks on a checkbox, and becomes false when the user clicks on the same checkbox again

		virtual void check_checkbox(Event& e);			// Raises a checkbox checked event
		virtual void uncheck_checkbox(Event& e);		// Raises a checkbox unchecked event
		virtual void handle_event(Event& e) override;
		virtual void draw() override;
	public:
		// To listen on inner component events
		virtual void action_performed(void* sender, Event& e) override;
		// The coordinates of the checkbox, the size of the checkbox, and the border's width
		CheckBox(float x = 0, float y = 0, float width = 20, float height = 20, float border_width = 1);
		// Sets the checked state
		void set_checked(bool checked);
		// Returns true when checked, and false when not
		bool is_checked();
	};

	// Raises a checkbox checked event
	void CheckBox::check_checkbox(Event& e) {
		this->checked = true;			// Check if it was unchecked, uncheck if it was checked
		e.checkbox_checked = true;
		e.checkbox_unchecked = false;
		this->raise_event(this, e);		// Raise an event that the checkbox was checked/unchecked
		e.checkbox_checked = false;
		e.checkbox_unchecked = false;
	}

	// Raises a checkbox unchecked event
	void CheckBox::uncheck_checkbox(Event& e) {
		this->checked = false;			// Check if it was unchecked, uncheck if it was checked
		e.checkbox_checked = false;
		e.checkbox_unchecked = true;
		this->raise_event(this, e);		// Raise an event that the checkbox was checked/unchecked
		e.checkbox_checked = false;
		e.checkbox_unchecked = false;
	}

	void CheckBox::handle_event(Event& e) {

	}

	void CheckBox::draw() {
		if (checked) {
			this->btn_check->get_label()->set_text("X");
		} else {
			this->btn_check->get_label()->set_text("");
		}
	}

	// To listen on inner component events
	void CheckBox::action_performed(void* sender, Event& e) {
		if (sender == btn_check) {
			if (e.button_pressed) {
				if (!checked) {
					this->check_checkbox(e);
				} else {
					this->uncheck_checkbox(e);
				}
			}
		}
	}

	// The coordinates of the check box, the size of the check box, and the border's width
	CheckBox::CheckBox(float x, float y, float width, float height, float border_width) : Component(x, y, width, height, border_width) {
		this->checked = false;
		this->btn_check = new Button("", 0, 0, width, height, border_width);
		this->btn_check->add_event_listener(this);
		this->add_component(this->btn_check);
	}

	// Sets the checked state
	void CheckBox::set_checked(bool checked) {
		this->checked = checked;
	}

	// Returns true when checked, and false when not
	bool CheckBox::is_checked() {
		return checked;
	}

}