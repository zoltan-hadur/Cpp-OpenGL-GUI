#pragma once

#include "Component.h"

namespace GLUI {

	// A checkbox that can raise events when checked/unchecked
	class CheckBox : public Button {
	protected:
		bool checked;	// Becomes true when the user clicks on a checkbox, and becomes false when the user clicks on the same checkbox again

		virtual void handle_event(Event& e) override;
		virtual void draw() override;
	public:
		// The coordinates of the checkbox, the size of the checkbox, and the border's width
		CheckBox(float x = 0, float y = 0, float width = 20, float height = 20, float border_width = 1);
		// Returns true when checked, and false when not
		bool get_checked();
	};

	void CheckBox::handle_event(Event& e) {
		float2 pos = this->get_absolute_position();											// The absolute position relative to the top-level window
		if (pos.x < e.x && e.x < pos.x + width && pos.y < e.y && e.y < pos.y + height) {	// Check if the mouse was was above the button when the event happened
			if (visible) {																	// Check if visible, because one does not simply press a button if it's invisible
				this->highlighted = true;													// Highlight the button if the mouse is above the button
				if (e.mouse_left && e.mouse_pressed) {										// Check if the user clicked on the button with the left mouse button
					border_width = active_border_width;										// The checkbox press 'animation', simply increases the border until the event last
					checked = !checked;														// Check if it was unchecked, uncheck if it was checked
					e.checkbox_checked = checked;
					e.checkbox_unchecked = !checked;
					this->raise_event(this, e);												// Raise an event that the checkbox was checked/unchecked
					e.checkbox_checked = false;
					e.checkbox_unchecked = false;
				}
			}
		} else {
			this->highlighted = false;														// Disable the highligh when the mouse is not above the checkbox
		}
		if (e.mouse_left && e.mouse_released) {												// Check if the user released the left mouse button
			border_width = default_border_width;											// The checkbox checked/unchecked 'animation' is over, decrease the border's width
		}
	}

	void CheckBox::draw() {
		Component::draw();																	// Draw the base component

		float2 pos = this->get_absolute_position();											// Get the absolute position relative to the top-level window
		if (checked) {																		// Draw a check mark if the checkbox is checked
			glColor4f(1, 1, 1, 1);
			glBegin(GL_LINES);
			glVertex2f(pos.x + 2, pos.y + height / 2);
			glVertex2f(pos.x + width / 2, pos.y - 2 + height);

			glVertex2f(pos.x + width / 2, pos.y - 2 + height);
			glVertex2f(pos.x - 2 + width, pos.y + 2);
			glEnd();
		}
	}

	// The coordinates of the check box, the size of the check box, and the border's width
	CheckBox::CheckBox(float x, float y, float width, float height, float border_width) : Button("", x, y, width, height, border_width) {
		this->background_color = Color(150, 150, 150, 255);
		checked = false;
	}

	// Return true when checked, and false when not
	bool CheckBox::get_checked() {
		return checked;
	}

}