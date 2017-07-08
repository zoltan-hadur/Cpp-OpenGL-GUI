#pragma once

#include "TextEditor.h"
#include "..\Event\ActionEvent.h"
#include "..\Event\ActionPerformer.h"
#include "..\Event\Event.h"
#include "..\Utility\Color.h"

namespace GLUI {

	// Classic textbox, can be either single or multi lined
	class TextBox : public TextEditor {
	protected:
		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		// Type, position, size and border's width
		TextBox(TYPE type, float x = 0, float y = 0, float width = 100, float height = 20, float border_width = 1);

		// Sets the focus (NOT performs any action)
		void set_focus(bool focused);
		// Returns true if has focus, else false
		bool has_focus();

		// Gains focus and performs text box focus gained action
		void gain_focus();
		// Loses focus and performs text box focus losed action
		void lose_focus();
		// Simulates enter press and performs text box enter pressed action
		void press_enter();
	};

	void TextBox::handle_event(Event& e) {
		if (e.mouse_pressed && e.mouse_left) {																		// If the user pressed the left mouse button
			if (e.mouse_is_inside && !e.mouse_is_covered) {															// Inside the text box
				this->gain_focus();																					// Then gain focus
			} else {																								// Else
				this->lose_focus();																					// Lose focus
			}
		}

		if (e.key_pressed && (e.key_code == '\n' || e.key_code == '\r') && !e.active_shift && this->has_focus()) {	// If the user pressed enter while the text box had focus (shift+enter is the new line)
			this->press_enter();
			this->lose_focus();
		} else {																									// Else
			TextEditor::handle_event(e);																			// Handle the input by the text editor
		}
	}

	void TextBox::draw(bool draw_background) {
		TextEditor::draw(draw_background);
	}

	// Type, position, size and border's width
	TextBox::TextBox(TYPE type, float x, float y, float width, float height, float border_width) : TextEditor(type, x, y, width, height) {
		this->background_color = Color(120, 120, 120, 255) / 255;
		this->set_default_border_width(border_width);
	}

	// Sets the focus (NOT performs any action)
	void TextBox::set_focus(bool focused) {
		this->focused = focused;
	}

	// Returns true if has focus, else false
	bool TextBox::has_focus() {
		return this->focused;
	}

	// Gains focus and performs text box focus gained action
	void TextBox::gain_focus() {
		ActionEvent e;
		this->focused = true;
		e.textbox_focus_gained = true;
		e.textbox_text = this->get_text();
		this->perform_action(this, e);
	}

	// Loses focus and performs text box focus losed action
	void TextBox::lose_focus() {
		ActionEvent e;
		this->set_focus(false);
		e.textbox_focus_lost = true;
		e.textbox_text = this->get_text();
		this->perform_action(this, e);
	}

	// Simulates enter press and performs text box enter pressed action
	void TextBox::press_enter() {
		ActionEvent e;
		e.textbox_enter_pressed = true;
		e.textbox_text = this->get_text();
		this->perform_action(this, e);
	}

}