#pragma once

#include <string>
#include "..\Utility\Color.h"


namespace GLUI {

	struct ActionEvent {
		int x=0;
		int y=0;

		// Button
		bool button_pressed=false;
		bool button_released = false;

		// Checkbox
		bool checkbox_checked = false;
		bool checkbox_unchecked = false;

		// ColorPicker
		Color colorpicker_color= Color();

		// ComboBox
		bool combobox_changed = false;
		int combobox_selected_index = 0;
		std::string combobox_selected_item = "";

		// ProgressBar
		bool progressbar_started = false;
		bool progressbar_stopped = false;
		bool progressbar_changed = false;
		float progressbar_min = 0;
		float progressbar_max = 0;
		float progressbar_value = 0;
		float progressbar_dvalue = 0;
		float progressbar_percent = 0;
		float progressbar_dpercent = 0;

		// Slider
		bool slider_changed = false;
		float slider_value = 0;
		float slider_dvalue = 0;

		// SpinnerBox
		bool spinnerbox_changed = false;
		long double spinnerbox_value = 0;
		long double spinnerbox_dvalue = 0;
		template<typename T> T get_spinnerbox_value() {
			return (T)spinnerbox_value;
		}
		template<typename T> T get_spinnerbox_dvalue() {
			return (T)spinnerbox_dvalue;
		}

		// TextBox
		bool textbox_focus_gained = false;
		bool textbox_focus_lost = false;
		bool textbox_enter_pressed = false;
		std::string textbox_text = "";

		// Animator
		bool animator_started = false;
		bool animator_stopped = false;
		bool animator_reached = false;
	};

}