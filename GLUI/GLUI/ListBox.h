#pragma once

#include <deque>
#include <tuple>
#include <initializer_list>
#include "..\Event\ActionListener.h"
#include "..\Event\ActionPerformer.h"
#include "Button.h"
#include "ListElement.h"
#include "Panel.h"
#include "ScrollPanel.h"

namespace GLUI {

	class ColumnTitle {
	protected:
		Button* btn_title;
		std::string preferred_width;
	public:
		ColumnTitle(Button* btn_title, std::string preferred_width) {
			this->btn_title = btn_title;
			this->preferred_width = preferred_width;
		}
		bool is_automatic() {
			return this->preferred_width.find('*') != std::string::npos;
		}
		float get_part() {
			return std::stof(this->preferred_width.substr(0, this->preferred_width.size() - 1));
		}
		Button* get_button() {
			return this->btn_title;
		}
		float get_width() {
			return std::stof(this->preferred_width);
		}
		float get_ratio(float sum_of_parts) {
			return this->get_part() / sum_of_parts;
		}
	};

	class ListBox : public Panel, public ActionListener, public ActionPerformer {
	protected:
		std::deque<ColumnTitle> column_titles;

		std::deque<ListElement*> list_elements;
		ScrollPanel* scp_list;

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		// To listen on inner component actions
		virtual void action_performed(void* sender, ActionEvent& e) override;

		ListBox(std::initializer_list<std::tuple<std::string, std::string>> columns, float x = 0, float y = 0, float width = 100, float height = 600, float border_width = 1);

		void add_element(ListElement* list_element);
	};

	void ListBox::handle_event(Event& e) {

	}

	void ListBox::draw(bool draw_background) {
		Panel::draw(draw_background);

		if (this->column_titles.size() == 1) {
			this->column_titles[0].get_button()->set_visible(this->column_titles[0].get_button()->get_label()->get_text() != "  ");
		}

		float remained_width = this->get_width() - this->scp_list->get_scroll_bar()->get_width();
		float sum_of_parts = 0;
		std::deque<float2> sizes;
		std::deque<float2> positions;

		for (auto column_title : this->column_titles) {
			if (column_title.is_automatic()) {
				sum_of_parts = sum_of_parts + column_title.get_part();
				remained_width = remained_width + 1;
			} else {
				remained_width = remained_width - column_title.get_width() + 1;
			}
		}

		for (auto column_title : this->column_titles) {
			Button* btn_title = column_title.get_button();
			if (column_title.is_automatic()) {
				btn_title->set_size(float2(remained_width * column_title.get_ratio(sum_of_parts), btn_title->get_height()));
			} else {
				btn_title->set_size(float2(column_title.get_width(), btn_title->get_height()));
			}
			sizes.push_back(btn_title->get_size());
		}

		float offset = 0;
		for (auto column_title : this->column_titles) {
			Button* btn_title = column_title.get_button();
			btn_title->set_position(float2(offset, 0));
			positions.push_back(btn_title->get_position());
			offset = offset + btn_title->get_width() - 1;

			btn_title->bring_front();
		}

		if (!this->list_elements.empty()) {
			float2 offset = this->list_elements[0]->get_position();
			for (auto list_element : this->list_elements) {
				list_element->set_sizes(sizes);
				list_element->set_positions(positions);

				Button* last = this->column_titles.back().get_button();
				list_element->set_size(float2(last->get_position().x + last->get_width(), 20));
				list_element->set_position(offset);
				offset = offset + float2(0, 19);
			}
		}
	}

	// To listen on inner component actions
	void ListBox::action_performed(void* sender, ActionEvent& e) {

	}

	ListBox::ListBox(std::initializer_list<std::tuple<std::string, std::string>> columns, float x, float y, float width, float height, float border_width) : Panel(true, x, y, width, height, border_width) {
		for (auto column : columns) {
			Button* btn_title = new Button(" " + std::get<0>(column) + " ");			// Create the button for the column header
			btn_title->get_label()->set_h_align(Label::H_ALIGN::LEFT);					// With text aligned to the left

			this->column_titles.push_back(ColumnTitle(btn_title, std::get<1>(column)));
		}

		for (auto column_title : this->column_titles) {
			this->add_component(column_title.get_button());
		}

		this->scp_list = new ScrollPanel(ScrollPanel::ALIGN::VERTICAL);
		this->scp_list->set_default_border_width(1);
		this->scp_list->set_position(float2(0, 0));
		this->scp_list->set_size(this->get_size());

		this->add_component(this->scp_list);
	}

	void ListBox::add_element(ListElement* list_element) {
		this->list_elements.push_back(list_element);
		this->scp_list->add_component(list_element);
		list_element->add_action_listener(this);

		if (this->column_titles[0].get_button()->get_label()->get_text() != "  " || this->column_titles.size() >= 2) {
			list_element->set_position(float2(0, 19));
		}
	}

}