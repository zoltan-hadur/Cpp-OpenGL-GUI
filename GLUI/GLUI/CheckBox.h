#pragma once

#include "Component.h"
#include "Button.h"
#include "..\Event\ActionEvent.h"
#include "..\Event\ActionListener.h"
#include "..\Event\ActionPerformer.h"
#include "..\Event\Event.h"

namespace GLUI {

	// A checkbox that can perform actions when checked/unchecked
	class CheckBox : public ActionListener, public Component {
	protected:
		Button* btn_check;	// The checkbox

		bool checked;		// Indicates that the checkbox is checked or not

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		// To listen on inner component's actions
		virtual void action_performed(void* sender, ActionEvent& e) override;

		// The coordinates of the checkbox, the size of the checkbox, and the border's width
		CheckBox(float x = 0, float y = 0, float width = 20, float height = 20, float border_width = 1);

		// Sets the checked state (DOES NOT performs any action)
		void set_checked(bool checked);
		// Returns true when checked, and false when not
		bool is_checked();

		// Performs a checkbox checked action
		void check();
		// Performs a checkbox unchecked action
		void uncheck();
	};

	void CheckBox::handle_event(Event& e) {

	}

	void CheckBox::draw(bool draw_background) {
		this->btn_check->get_label()->set_text(checked ? "X" : "");
	}

	// To listen on inner component's actions
	void CheckBox::action_performed(void* sender, ActionEvent& e) {
		if (sender == btn_check && e.button_released) {		// If the checkbox was pressed
			if (!checked) {									// Check it if it's unchecked
				this->check();
			} else {										// Else uncheck it
				this->uncheck();
			}
		}
	}

	// The coordinates of the check box, the size of the check box, and the border's width
	CheckBox::CheckBox(float x, float y, float width, float height, float border_width) : Component(x, y, width, height, border_width) {
		this->checked = false;
		this->btn_check = new Button("", 0, 0, width, height, border_width);
		this->btn_check->set_background_color(Color(120, 120, 120, 255) / 255);		// Default grey color
		this->btn_check->add_action_listener(this);
		this->add_component(this->btn_check);
	}

	// Sets the checked state (DOES NOT performs any action)
	void CheckBox::set_checked(bool checked) {
		this->checked = checked;
	}

	// Returns true when checked, and false when not
	bool CheckBox::is_checked() {
		return checked;
	}

	// Performs a checkbox checked action
	void CheckBox::check() {
		this->set_checked(true);
		ActionEvent e;
		e.checkbox_checked = true;
		this->perform_action(this, e);
	}

	// Performs a checkbox unchecked action
	void CheckBox::uncheck() {
		this->set_checked(false);
		ActionEvent e;
		e.checkbox_unchecked = true;
		this->perform_action(this, e);
	}

}