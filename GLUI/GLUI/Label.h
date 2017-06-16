#pragma once

#include "Component.h"

namespace GLUI {

	// Simple label, with the ability to align it
	class Label : public Component {
	protected:
		enum class H_ALIGN;		// Forward declaration
		enum class V_ALIGN;		// Forward declaration

		std::string text;		// The label itself
		H_ALIGN h_align;		// Determines the horizontal alignment
		V_ALIGN v_align;		// Determines the vertical alignment

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		enum class H_ALIGN {	// Horizontal alignment values
			LEFT,				// Horizontal left
			MID,				// Horizontal centre
			RIGHT				// Horizontal right
		};
		enum class V_ALIGN {	// Vertical alignment values
			TOP,				// Vertical top
			MID,				// Vertical centre
			BOT					// Vertical bottom
		};

		// Text to be displayed, it's position, and size
		Label(std::string text = "Label", float x = 0, float y = 0, float width = 0, float height = 0);
		// Sets the label
		void set_text(std::string text);
		// Sets the horizontal alignment
		void set_h_align(H_ALIGN h_align);
		// Sets the vertical alignment
		void set_v_align(V_ALIGN v_align);
		// Gets the label
		std::string get_text();
		// Gets the horizontal alignment
		H_ALIGN get_h_align();
		// Get the vertical alignment
		V_ALIGN get_v_align();
	};

	void Label::handle_event(Event& e) {

	}

	void Label::draw(bool draw_background) {
		if (!text.empty()) {
			float2 pos = this->get_absolute_position();

			int x = pos.x;
			int y = pos.y + char_height - 3;
			int w = text.size() * char_width;	// Width of the text in pixels

			x = (h_align == H_ALIGN::RIGHT) ? (x + width - w) : ((h_align == H_ALIGN::MID) ? (x + (width - w) / 2) : x);						// Align
			y = (v_align == V_ALIGN::BOT) ? (y + height - char_height) : ((v_align == V_ALIGN::MID) ? (y + (height - char_height) / 2) : y);	// Align

			glColor4f(1, 1, 1, 1);
			glRasterPos2f(x, y);
			glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)text.c_str());
		}
	}

	// The label itself, it's position, and size
	Label::Label(std::string text, float x, float y, float width, float height) : Component(x, y, width, height) {
		this->text = text;
		this->h_align = H_ALIGN::MID;										// Default mid
		this->v_align = V_ALIGN::MID;
		this->width = std::max(width, (float)(text.size() * char_width));	// Minimum width is the width of the text in pixels
		this->height = std::max(height, (float)char_height);				// Minimum height is the width of the text in pixels
	}

	// Sets the label
	void Label::set_text(std::string text) {
		this->text = text;
		this->width = std::max(width, (float)(text.size() * char_width));	// Minimum width is the width of the text in pixels
		this->height = std::max(height, (float)char_height);				// Minimum height is the width of the text in pixels
	}

	// Sets the horizontal alignment
	void Label::set_h_align(H_ALIGN h_align) {
		this->h_align = h_align;
	}

	// Sets the vertical alignment
	void Label::set_v_align(V_ALIGN v_align) {
		this->v_align = v_align;
	}

	// Gets the label
	std::string Label::get_text() {
		return this->text;
	}

	// Gets the horizontal alignment
	Label::H_ALIGN Label::get_h_align() {
		return this->h_align;
	}

	// Get the vertical alignment
	Label::V_ALIGN Label::get_v_align() {
		return this->v_align;
	}

}