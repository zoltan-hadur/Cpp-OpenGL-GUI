#pragma once

#include <GL\freeglut.h>
#include <deque>
#include <algorithm>
#include "Component.h"
#include "..\Utility\Stopwatch.h"

namespace GLUI {

	// Handles the massive bullshit:
	// inner buffer, cursor position, text selection, character insertion/deletion
	// copy paste, home/end buttons, etc
	class TextEditor : public Component {
	private:
		Stopwatch watch;
		float acc;

		int window_start;
		int window_end;
		int window_width;

		int pos_cursor;								// Position of the cursor to manipulate the input buffer
		int pos_selection;							// Position of where the cursor was at when the selection started
		bool selecting_buffer_input;				// True if user currently selecting characters from the input buffer
		bool active_shift;							// True if shift is down
		std::deque<unsigned char> buffer;			// Stores the characters that the user writes to the console. Also can be manipulated like in nearly every text editor

		void insert_char(unsigned char key);		// Inserts a character at the cursor's position
		void insert_paste(std::string paste);		// Inserts a whole string at the cursor's position
		std::string get_clipboard_text();			// Returns with the string stored in the global clipboard (ctrl+v)
		void set_clipboard_text(std::string copy);	// Sets the global clipboard with the string selected in the input buffer (ctrl+c, ctrl+x)
		void delete_char_before();					// Deletes the char before the cursor from the input buffer (like pressing backspace in a text editor)
		void delete_char_after();					// Deletes the char after the cursor from the input buffer (like pressing delete in a text editor)
		void cursor_move_left();					// Moves the cursor to the left by one (like pressing left arrow in a text editor)
		void cursor_move_right();					// Moves the cursor to the right by one (like pressing the right arrow in a text editor)
		void cursor_move_up();						// Moves the cursor by one line up
		void cursor_move_down();					// Moves the cursor by one line down
		void cursor_jump_left();					// Moves the cursor to the top left (like pressing home in a text editor)
		void cursor_jump_right();					// Moves the cursor to the top right (like pressing end in a text editor)
		void draw_selection(float x, float y);		// Draws quads upon selected text in the buffer
	protected:
		enum class TYPE;

		TYPE type;
		bool focused;

		TextEditor(TYPE type, float x = 0, float y = 0, float width = 100, float height = 20);

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		enum class TYPE {	// Determines that the text editor can edit single or multiple lines
			SINGLE_LINE,	// Text editor has only one editable line
			MULTI_LINE		// Text editor has multiple editable line
		};

		void set_text(std::string text);
		std::string get_text();
	};

	void TextEditor::insert_char(unsigned char key) {
		if (selecting_buffer_input) {												// WHen selecting_buffer_input
			this->delete_char_before();												// Delete the whole selection
		}
		buffer.insert(buffer.begin() + (pos_cursor++), key);						// Then insert the char into the input buffer
		selecting_buffer_input = false;
	}

	void TextEditor::insert_paste(std::string paste) {
		if (selecting_buffer_input) {												// When selecting_buffer_input
			this->delete_char_before();												// Delete the whole selection
		}
		for (unsigned char c : this->get_clipboard_text()) {						// Then insert the string into the input line
			this->insert_char(c);
		}
		selecting_buffer_input = false;
	}

	std::string TextEditor::get_clipboard_text() {
#ifdef _WIN32
		std::string str = "";
		if (IsClipboardFormatAvailable(CF_TEXT)) {									// Must check the format, otherwise can get a nice exception
			if (OpenClipboard(nullptr)) {
				str = std::string((char*)GetClipboardData(CF_TEXT));				// Get and convert the clipboard text to string
			}
			CloseClipboard();
		}
		return str;
#endif
#ifdef linux
		throw "Getting clipboard text on linux with ctrl+c or ctrl+x is not supported!";
#endif
	}

	void TextEditor::set_clipboard_text(std::string copy) {
#ifdef _WIN32
		if (OpenClipboard(nullptr)) {												// If can open clipboard
			EmptyClipboard();														// Clear it
			HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, copy.size() + 1);				// Allocate mem for copied string
			if (!hg) {																// Return if it was unsuccesful
				CloseClipboard();
				return;
			}
			memcpy(GlobalLock(hg), copy.c_str(), copy.size() + 1);					// Copy string to clipboard
			GlobalUnlock(hg);
			SetClipboardData(CF_TEXT, hg);
			GlobalFree(hg);
		}
		CloseClipboard();
#endif
#ifdef linux
		throw "Setting clipboard text on linux with ctrl+v is not supported!";
#endif
	}

	void TextEditor::delete_char_before() {
		if (selecting_buffer_input) {												// Delete the whole selection when selecting_buffer_input
			buffer.erase(buffer.begin() + std::min(pos_selection, pos_cursor), buffer.begin() + std::max(pos_selection, pos_cursor));
			this->cursor_move_left();												// Then move the cursor to the top right of the selection
		} else if (buffer.size() > 0 && pos_cursor > 0) {							// Else delete the character right before the cursor
			buffer.erase(buffer.begin() + (--pos_cursor));
		}
	}

	void TextEditor::delete_char_after() {
		if (selecting_buffer_input) {												// Delete the whole selection when selecting_buffer_input
			buffer.erase(buffer.begin() + std::min(pos_selection, pos_cursor), buffer.begin() + std::max(pos_selection, pos_cursor));
			this->cursor_move_left();												// Then move the cursor to the top left of the selection
		} else if (buffer.size() > 0 && pos_cursor < buffer.size()) {				// Else delete the character right after the cursor
			buffer.erase(buffer.begin() + pos_cursor);
		}
	}

	void TextEditor::cursor_move_left() {
		if (!selecting_buffer_input && active_shift) {								// The user just started the selecting_buffer_input
			pos_selection = pos_cursor;												// The position where the selection started
			selecting_buffer_input = true;
		}
		if (selecting_buffer_input && !active_shift) {								// When shift is not pressed, pressing the left arrow ends the selection
			pos_cursor = std::min(pos_selection, pos_cursor);						// By definiton, pressing the left arrow while selecting_buffer_input, places the cursor at the start of the selection
			selecting_buffer_input = false;
		} else if (pos_cursor > 0) {												// Else just moves the cursor to the left by one
			pos_cursor--;
		}
	}

	void TextEditor::cursor_move_right() {
		if (!selecting_buffer_input && active_shift) {								// The user just started selecting_buffer_input
			pos_selection = pos_cursor;												// The position where the selection started
			selecting_buffer_input = true;
		}
		if (selecting_buffer_input && !active_shift) {								// When shift is not pressed, pressing the right arrow ends the selection
			pos_cursor = std::max(pos_selection, pos_cursor);						// By definiton, pressing the right arrow while selecting_buffer_input, places the cursor at the end of the selection
			selecting_buffer_input = false;
		} else if (pos_cursor < buffer.size()) {									// Else just moves the cursor to the right by one
			pos_cursor++;
		}
	}

	void TextEditor::cursor_move_up() {
		if (this->type == TYPE::SINGLE_LINE) {										// Does nothing when TextBox is a single line TextBox
			return;
		}

		if (!selecting_buffer_input && active_shift) {								// The user just started selecting_buffer_input
			pos_selection = pos_cursor;												// The position where the selection started
			selecting_buffer_input = true;
		}
		if (selecting_buffer_input && !active_shift) {								// When shift is not pressed, pressing home key ends the selection
			selecting_buffer_input = false;
		}

		float2 start_pos = this->get_absolute_position() + float2(default_border_width + char_width / 2, default_border_width + char_width * 3 / 2);
		float2 pos = start_pos;
		float max_pos_x = start_pos.x + this->get_width() - char_width * 2;

		for (int i = 0; i <= pos_cursor; ++i) {
			if (pos.x > max_pos_x) {
				pos = float2(start_pos.x, pos.y + char_height);
			}
			if (i < pos_cursor) {
				unsigned char c = buffer[i];
				if (c == '\n' || c == '\r') {
					pos = float2(start_pos.x, pos.y + char_height);
				}
				if (c != '\n' && c != '\r') {
					pos = pos + float2(char_width, 0);
				}
			}
		}

		if (pos.y > start_pos.y) {
			int from = 0;
			int to = 0;
			int actual_line_length = 0;
			int upper_line_length = 0;
			int orig_pos_cursor = pos_cursor;

			this->cursor_jump_left();
			actual_line_length = orig_pos_cursor - pos_cursor + 1;
			to = pos_cursor--;
			this->cursor_jump_left();
			from = pos_cursor;
			upper_line_length = to - from;
			pos_cursor = orig_pos_cursor;

			if (upper_line_length > actual_line_length) {
				pos_cursor = pos_cursor - upper_line_length;
			} else {
				pos_cursor = pos_cursor - actual_line_length;
			}
		}
	}

	void TextEditor::cursor_move_down() {
		if (this->type == TYPE::SINGLE_LINE) {										// Does nothing when TextBox is a single line TextBox
			return;
		}

		if (!selecting_buffer_input && active_shift) {								// The user just started selecting_buffer_input
			pos_selection = pos_cursor;												// The position where the selection started
			selecting_buffer_input = true;
		}
		if (selecting_buffer_input && !active_shift) {								// When shift is not pressed, pressing home key ends the selection
			selecting_buffer_input = false;
		}

		float2 start_pos = this->get_absolute_position() + float2(default_border_width + char_width / 2, default_border_width + char_width * 3 / 2);
		float2 pos = start_pos;
		float max_pos_x = start_pos.x + this->get_width() - char_width * 2;

		for (int i = 0; i <= pos_cursor; ++i) {
			if (pos.x > max_pos_x) {
				pos = float2(start_pos.x, pos.y + char_height);
			}
			if (i < pos_cursor) {
				unsigned char c = buffer[i];
				if (c == '\n' || c == '\r') {
					pos = float2(start_pos.x, pos.y + char_height);
				}
				if (c != '\n' && c != '\r') {
					pos = pos + float2(char_width, 0);
				}
			}
		}

		int max_chars = (this->get_width() - char_width) / char_width;
		int from = 0;
		int to = 0;
		int actual_line_length = 0;
		int actual_line_length_to_end = 0;
		int lower_line_length = 0;
		int orig_pos_cursor = pos_cursor;

		this->cursor_jump_left();
		from = pos_cursor;
		this->cursor_jump_right();
		if (pos_cursor - from == max_chars) {
			pos_cursor--;
		}
		to = pos_cursor;
		actual_line_length = to - from + 1;
		actual_line_length_to_end = to - orig_pos_cursor + 1;


		bool has_more_line = false;
		for (int i = from; i < buffer.size() && !has_more_line; ++i) {
			if (buffer[i] == '\n' || buffer[i] == '\r' || i - from >= max_chars) {
				has_more_line = true;
			}
		}
		if (has_more_line) {
			pos_cursor++;
			from = pos_cursor;
			this->cursor_jump_right();
			to = pos_cursor;
			if (to - from == max_chars) {
				lower_line_length = to - from;
			} else {
				lower_line_length = to - from;
			}
			pos_cursor = orig_pos_cursor + std::min(actual_line_length, actual_line_length_to_end + lower_line_length);
		} else {
			pos_cursor = orig_pos_cursor;
		}
	}

	void TextEditor::cursor_jump_left() {
		if (!selecting_buffer_input && active_shift) {								// The user just started selecting_buffer_input
			pos_selection = pos_cursor;												// The position where the selection started
			selecting_buffer_input = true;
		}
		if (selecting_buffer_input && !active_shift) {								// When shift is not pressed, pressing home key ends the selection
			selecting_buffer_input = false;
		}

		if (this->type == TYPE::SINGLE_LINE) {										// Jumps to the top left when TextBox is a single line TextBox
			pos_cursor = 0;
			return;
		}

		float2 start_pos = this->get_absolute_position() + float2(default_border_width + char_width / 2, default_border_width + char_width * 3 / 2);
		float2 pos = start_pos;
		float max_pos_x = start_pos.x + this->get_width() - char_width * 2;

		for (int i = 0; i < pos_cursor; ++i) {
			unsigned char c = buffer[i];
			if (c == '\n' || c == '\r' || pos.x > max_pos_x) {
				pos = float2(start_pos.x, pos.y + char_height);
			}
			if (c != '\n' && c != '\r') {
				pos = pos + float2(char_width, 0);
			}
		}


		while (pos_cursor > 0 && buffer[pos_cursor - 1] != '\n' && buffer[pos_cursor - 1] != '\r' && pos.x > start_pos.x) {// Jump to the start of the line
			pos_cursor--;
			pos = pos - float2(char_width, 0);
		}
	}

	void TextEditor::cursor_jump_right() {
		if (!selecting_buffer_input && active_shift) {								// The user just started the selecting_buffer_input
			pos_selection = pos_cursor;												// The position where selection started
			selecting_buffer_input = true;
		}
		if (selecting_buffer_input && !active_shift) {								// When shift is not pressed, pressing end keys ends the selection
			selecting_buffer_input = false;
		}

		if (this->type == TYPE::SINGLE_LINE) {										// Jumps to the top right when TextBox is a single line TextBox
			pos_cursor = buffer.size();
			return;
		}

		float2 start_pos = this->get_absolute_position() + float2(default_border_width + char_width / 2, default_border_width + char_width * 3 / 2);
		float2 pos = start_pos;
		float max_pos_x = start_pos.x + this->get_width() - char_width * 2;

		for (int i = 0; i < pos_cursor; ++i) {
			unsigned char c = buffer[i];
			if (c == '\n' || c == '\r' || pos.x > max_pos_x) {
				pos = float2(start_pos.x, pos.y + char_height);
			}
			if (c != '\n' && c != '\r') {
				pos = pos + float2(char_width, 0);
			}
		}


		while (pos_cursor < buffer.size() && buffer[pos_cursor] != '\n' && buffer[pos_cursor] != '\r' && pos.x < max_pos_x) {// Jump to the end of the line
			pos_cursor++;
			pos = pos + float2(char_width, 0);
		}
	}

	void TextEditor::draw_selection(float x, float y) {
		y = y + 3;
		//180, 180, 180, 255
		glColor4f(180 / 255.0f, 180 / 255.0f, 180 / 255.0f, 255 / 255.0f);
		//glColor4f(1, 0, 0, 1);
		glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x, y - char_height);
		glVertex2f(x + char_width, y - char_height);
		glVertex2f(x + char_width, y);
		glEnd();
	}

	TextEditor::TextEditor(TYPE type, float x, float y, float width, float height) : Component(x, y, width, height) {
		this->type = type;
		this->acc = 0;
		this->focused = false;
		this->pos_cursor = 0;
		this->pos_selection = 0;
		this->active_shift = false;
		this->buffer = std::deque<unsigned char>();

		this->window_start = 0;
		this->window_width = (width - char_width) / char_width;
		this->window_end = this->window_start + this->window_width;
		this->watch = Stopwatch();
		watch.start();
	}

	void TextEditor::handle_event(Event& e) {
		if (focused) {
			this->active_shift = e.active_shift;
			if (e.key_pressed) {
				switch (e.key_code) {
					case 1:		// Ctrl + a
					{
						selecting_buffer_input = true;
						pos_selection = 0;																						// Select the whole text
						pos_cursor = buffer.size();
						break;
					}
					case 3:		// Ctrl + c
					{
						if (selecting_buffer_input) {																			// If selecting text
							int from = std::min(pos_selection, pos_cursor);														// Selection starts at this index
							int to = std::max(pos_selection, pos_cursor);														// Selection ends at this index
							std::string selected_text = std::string(buffer.begin() + from, buffer.begin() + to);				// The selected text in the input buffer
							this->set_clipboard_text(selected_text);															// Copy it to the clipboard
						}
						break;
					}
					case 22:	// Ctrl + v
					{
						this->insert_paste(this->get_clipboard_text());															// Pastes string from clipboard to the input buffer
						break;
					}
					case 24:	// Ctrl + x
					{
						if (selecting_buffer_input) {
							int from = std::min(pos_selection, pos_cursor);														// Selection starts at this index
							int to = std::max(pos_selection, pos_cursor);														// Selection ends at this index
							std::string selected_text = std::string(buffer.begin() + from, buffer.begin() + to);				// The selected text in the input buffer
							this->set_clipboard_text(selected_text);															// Copy it to the clipboard
							this->delete_char_before();																			// And remove it from the input buffer
						}
						break;
					}
					case 8:		// Backspace key
					{
						this->delete_char_before();																				// Acts like backspace in every text editor
						break;
					}
					case 127:	// Delete key
					{
						this->delete_char_after();																				// Acts like delete in every text editor
						break;
					}
					default:	// Insert char at cursor
					{
						this->active_shift = false;	// At insertion for example 'A' shift is pressed, but not currently selecting text, hence this function call
						this->insert_char(e.key_code);
						break;
					}
				}
			}
			if (e.special_pressed) {
				switch (e.special_key_code) {
					case GLUT_KEY_UP:
					{
						this->cursor_move_up();
						break;
					}
					case GLUT_KEY_DOWN:
					{
						this->cursor_move_down();
						break;
					}
					case GLUT_KEY_LEFT:
					{
						this->cursor_move_left();
						break;
					}
					case GLUT_KEY_RIGHT:
					{
						this->cursor_move_right();
						break;
					}
					case GLUT_KEY_HOME:
					{
						this->cursor_jump_left();
						break;
					}
					case GLUT_KEY_END:
					{
						this->cursor_jump_right();
						break;
					}
				}
			}

			if (e.mouse_left_down) {
				selecting_buffer_input = true;
				float2 start_pos = this->get_absolute_position() + float2(default_border_width + char_width / 2, default_border_width + char_width * 3 / 2);
				float2 pos = start_pos;
				float max_pos_x = start_pos.x + this->get_width() - char_width * 2;

				if (this->type == TYPE::SINGLE_LINE) {
					start_pos = start_pos - float2(window_start*char_width, 0);
					pos = start_pos;
				}

				for (int i = 0; i < buffer.size(); ++i) {
					if (pos.y - char_height < e.y && e.y < pos.y) {
						if (pos.x < e.x && e.x < pos.x + char_width / 2) {
							if (e.mouse_left && e.mouse_pressed) {
								pos_selection = i;
							}
							pos_cursor = i;
						} else if (pos.x + char_width / 2 < e.x) {
							if (e.mouse_left && e.mouse_pressed) {
								pos_selection = i + 1;
							}
							pos_cursor = i + 1;
						}
					}

					if (pos.x > max_pos_x) {
						pos = float2(start_pos.x, pos.y + char_height);
					}

					if (pos.y - char_height < e.y && e.y < pos.y) {
						if (pos.x < e.x && e.x < pos.x + char_width / 2) {
							if (e.mouse_left && e.mouse_pressed) {
								pos_selection = i;
							}
							pos_cursor = i;
						} else if (pos.x + char_width / 2 < e.x) {
							if (e.mouse_left && e.mouse_pressed) {
								pos_selection = i + 1;
							}
							pos_cursor = i + 1;
						}
					}

					if (i < buffer.size()) {
						unsigned char c = buffer[i];
						if (c == '\n' || c == '\r' || pos.x > max_pos_x) {
							pos = float2(start_pos.x, pos.y + char_height);
						}
						if (c != '\n' && c != '\r') {
							pos = pos + float2(char_width, 0);
						}
					}
				}
			}
		}
		if (e.mouse_left && e.mouse_pressed) {
			float2 pos = this->get_absolute_position();
			if (!(pos.x < e.x && e.x < pos.x + this->get_width() && pos.y < e.y && e.y < pos.y + this->get_height())) {
				this->selecting_buffer_input = false;
				this->pos_selection = this->pos_cursor;
			}
		}
		if (e.mouse_left && e.mouse_released) {
			if (this->selecting_buffer_input && this->pos_selection == this->pos_cursor) {
				this->selecting_buffer_input = false;
				this->pos_selection = this->pos_cursor;
			}
		}
	}

	void TextEditor::draw(bool draw_background) {
		Component::draw(draw_background);

		float dt = watch.get_delta<Time::SECONDS>();

		this->window_width = (this->get_width() - char_width) / char_width;
		int max_lines = this->get_height() / char_height;
		float2 start_pos = this->get_absolute_position() + float2(default_border_width + char_width / 2, default_border_width + char_width * 1.6);
		float2 pos = start_pos;
		float2 max_pos = float2(start_pos.x + this->get_width() - char_width * 2, start_pos.y + this->get_height() - char_height);

		//if (buffer.empty()) {
		//	glColor4f(0.7, 0.7, 0.7, 1);
		//	glRasterPos2f(pos.x, pos.y);
		//	glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char*)"TextBox");
		//}

		if (max_lines == 1 || this->type == TYPE::SINGLE_LINE) {
			window_start = std::min(window_start, pos_cursor);
			window_start = std::max(window_start, pos_cursor - window_width);
			window_end = std::max(window_end, pos_cursor);
			window_end = std::min(window_end, pos_cursor + window_width);
			for (int i = window_start; i < window_end && i < buffer.size(); ++i) {
				if (std::min(pos_selection, pos_cursor) <= i && i < std::max(pos_selection, pos_cursor) && selecting_buffer_input) {	// Check if char is in the selection
					if (buffer[i] != '\n' && buffer[i] != '\r') {
						this->draw_selection(pos.x, pos.y);
					}
				}
				glColor4f(1, 1, 1, 1);
				glRasterPos2f(pos.x, pos.y);
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, buffer[i]);
				pos = pos + float2(char_width, 0);
			}
			if (focused) {
				// Draw the cursor
				// The cursor is visible only in the first half
				glColor4f(1, 1, 1, 1);
				acc = fmod(acc + dt, 0.5);
				if (acc < 0.25) {
					pos = start_pos + float2(std::min(pos_cursor, pos_cursor - window_start) * char_width, 0);
					glLineWidth(2);
					glBegin(GL_LINES);
					glVertex2f(pos.x, pos.y + 3);
					glVertex2f(pos.x, pos.y + 3 - char_height);
					glEnd();
				}
			}
		} else {
			glColor4f(1, 1, 1, 1);		// Sets color
			glRasterPos2f(pos.x, pos.y);
			for (int i = 0; i < buffer.size(); ++i) {
				unsigned char c = buffer[i];
				if (c == '\n' || c == '\r' || pos.x > max_pos.x) {
					pos = float2(start_pos.x, pos.y + char_height);
				}
				if (std::min(pos_selection, pos_cursor) <= i && i < std::max(pos_selection, pos_cursor) && selecting_buffer_input) {	// Check if char is in the selection
					if (c != '\n' && c != '\r') {
						this->draw_selection(pos.x, pos.y);
					}
				}
				if (c != '\n' && c != '\r') {
					glColor4f(1, 1, 1, 1);
					glRasterPos2f(pos.x, pos.y);
					glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
					pos = pos + float2(char_width, 0);
				}
				if (pos.y > max_pos.y) {
					i--;
					this->delete_char_before();
				}
			}
			if (focused) {
				// Draw the cursor
				// The cursor is visible only in the first half
				glColor4f(1, 1, 1, 1);
				acc = fmod(acc + dt, 0.5);
				if (acc < 0.25) {
					pos = start_pos;
					for (int i = 0; i < pos_cursor; ++i) {
						unsigned char c = buffer[i];
						if (c == '\n' || c == '\r' || pos.x > max_pos.x) {
							pos = float2(start_pos.x, pos.y + char_height);
						}
						if (c != '\n' && c != '\r') {
							pos = pos + float2(char_width, 0);
						}
					}
					glLineWidth(2);
					glBegin(GL_LINES);
					glVertex2f(pos.x, pos.y + 3);
					glVertex2f(pos.x, pos.y + 3 - char_height);
					glEnd();
				}
			}
		}
	}

	void TextEditor::set_text(std::string text) {
		this->cursor_jump_right();
		while (!this->buffer.empty()) {
			this->delete_char_before();
		}
		for (unsigned char c : text) {
			this->insert_char(c);
		}
		window_start = 0;
		window_end =  window_width;
	}

	std::string TextEditor::get_text() {
		return std::string(this->buffer.begin(), this->buffer.end());
	}

}