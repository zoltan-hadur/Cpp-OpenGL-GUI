#pragma once

#include "TextEditor.h"

namespace GLUI {

	class TextBox : public TextEditor {
	protected:

	public:
		TextBox(TYPE type, float x = 0, float y = 0, float width = 100, float height = 20, float border_width = 1);
		virtual void handle_event(Event& e) override;
	};

	TextBox::TextBox(TYPE type, float x, float y, float width, float height, float border_width) : TextEditor(type, x, y, width, height) {
		this->background_color = Color(120, 120, 120, 255);
		this->set_default_border_width(border_width);
	}

	void TextBox::handle_event(Event& e) {
		float2 pos = this->get_absolute_position();
		if (e.mouse_left && e.mouse_pressed) {
			if (pos.x < e.x && e.x < pos.x + width && pos.y < e.y && e.y < pos.y + height) {
				if (visible) {
					this->set_focus(true);
				}
			} else {
				this->set_focus(false);
			}
		}
		if ((e.key_code == '\n' || e.key_code == '\r') && e.key_pressed && !e.active_shift && this->has_focus()) {
			this->raise_event(this, e);
		} else {
			TextEditor::handle_event(e);
		}
	}

}