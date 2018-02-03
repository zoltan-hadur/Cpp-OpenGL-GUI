#pragma once

#include <deque>
#include <initializer_list>
#include "Button.h"

namespace GLUI {

	class ListElement : public Button {
	protected:
		std::deque<Button*> data;

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		ListElement(std::initializer_list<std::string> data);

		void set_sizes(std::deque<float2>& sizes);
		void set_positions(std::deque<float2>& positions);
	};

	void ListElement::handle_event(Event& e) {
		Button::handle_event(e);
	}

	void ListElement::draw(bool draw_background) {
		Button::draw(draw_background);
	}

	ListElement::ListElement(std::initializer_list<std::string> data) : Button("") {
		for (auto column : data) {
			Button* btn_column = new Button(" " + column + " ");
			btn_column->get_label()->set_h_align(Label::H_ALIGN::LEFT);
			btn_column->set_background_color(Color(0, 0, 0, 0));
			btn_column->disable();
			btn_column->get_label()->enable();
			this->data.push_back(btn_column);
			this->add_component(btn_column);
		}
	}

	void ListElement::set_sizes(std::deque<float2>& sizes) {
		for (int i = 0; i < sizes.size(); ++i) {
			this->data[i]->set_size(sizes[i]);
		}
	}

	void ListElement::set_positions(std::deque<float2>& positions) {
		for (int i = 0; i < positions.size(); ++i) {
			this->data[i]->set_position(positions[i]);
		}
	}

}