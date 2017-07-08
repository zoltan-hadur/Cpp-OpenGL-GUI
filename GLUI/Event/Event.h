#pragma once

namespace GLUI {

	struct Event {
		// Keyboard
		unsigned char key_code;
		int special_key_code;
		bool keys_down[256];
		bool key_pressed;
		bool key_released;
		bool special_pressed;
		bool special_released;
		bool active_shift;
		bool active_ctrl;
		bool active_alt;

		// Mouse
		int x;
		int y;
		int dx;
		int dy;
		bool mouse_left_down;
		bool mouse_middle_down;
		bool mouse_right_down;
		bool mouse_left;
		bool mouse_middle;
		bool mouse_right;
		bool mouse_scroll_up;
		bool mouse_scroll_down;
		bool mouse_pressed;
		bool mouse_released;
		bool mouse_moved;
		bool mouse_is_inside;
		bool mouse_is_covered;
	};

}