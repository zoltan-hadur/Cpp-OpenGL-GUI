#pragma once

#include "TextEditor.h"

namespace GLUI {

	// Classic textbox, can be either single or multi lined
	class TextBox : public TextEditor {
	protected:
		void gain_focus(Event& e);
		void lose_focus(Event& e);

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		// Type, position, size and border's width
		TextBox(TYPE type, float x = 0, float y = 0, float width = 100, float height = 20, float border_width = 1);

		bool has_focus();
	};

	void TextBox::gain_focus(Event& e) {
		this->focused = true;
		e.textbox_focus_gained = true;
		e.textbox_text = this->get_text();
		this->raise_event(this, e);
		e.textbox_focus_gained = false;
	}

	void TextBox::lose_focus(Event& e) {
		this->focused = false;
		e.textbox_focus_lost = true;
		e.textbox_text = this->get_text();
		this->raise_event(this, e);
		e.textbox_focus_lost = false;
	}

	void TextBox::handle_event(Event& e) {
		float2 pos = this->get_absolute_position();
		if (e.mouse_left && e.mouse_pressed) {																			// If the user clicked
			if (pos.x < e.x && e.x < pos.x + width && pos.y < e.y && e.y < pos.y + height) {							// Inside the text box
				if (!e.mouse_covered) {																					// And it's not covered by an other component
					this->gain_focus(e);																				// Set he focus to true
				}
			} else {
				this->lose_focus(e);																					// Else set the focus to false
			}
		}
		if ((e.key_code == '\n' || e.key_code == '\r') && e.key_pressed && !e.active_shift && this->has_focus()) {		// If the text box has focus and the user pressed the enter (shift + enter is the new line in a text box)
			e.textbox_enter_pressed = true;
			//this->raise_event(this, e);																					// Raise an event
			this->lose_focus(e);																						// And set the focus to false
			e.textbox_enter_pressed = false;
		} else {
			TextEditor::handle_event(e);																				// Else handle the input by the text editor
		}
	}

	void TextBox::draw(bool draw_background) {
		//Component::draw(draw_background);
		TextEditor::draw(draw_background);
	}

	// Type, position, size and border's width
	TextBox::TextBox(TYPE type, float x, float y, float width, float height, float border_width) : TextEditor(type, x, y, width, height) {
		this->background_color = Color(120, 120, 120, 255);
		this->set_default_border_width(border_width);
	}

	bool TextBox::has_focus() {
		return this->focused;
	}

}