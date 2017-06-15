#pragma once

#include <vector>
#include "Panel.h"
#include "Button.h"

namespace GLUI {

	class TabbedPanel : public EventListener, public Panel {
	protected:
		std::vector<Button*> tab_selectors;
		std::vector<Panel*> tabs;
	public:
		TabbedPanel(float x = 0, float y = 0, float width = 100, float height = 100, float border_width = 2);
		void add_tab(std::string tab_title = "Tab", Panel* tab = nullptr);
		void remove_tab(std::string tab_title);
		void remove_tab(Panel* tab);
		void set_tab_title(int id, std::string tab_title = "Tab");
		void set_tab_title(Panel* tab, std::string tab_title = "Tab");
		Panel* get_tab(int id);
		Panel* get_tab(std::string tab_title);
		virtual void action_performed(void* sender, Event& e) override;
		virtual void draw() override;
	};

	TabbedPanel::TabbedPanel(float x, float y, float width, float height, float border_width) : Panel("", x, y, width, height, border_width) {

	}

	void TabbedPanel::add_tab(std::string tab_title, Panel* tab) {
		Button* tab_selector = new Button(tab_title);
		tab_selector->set_default_border_width(default_border_width);
		tab_selector->add_event_listener(this);
		tab_selectors.push_back(tab_selector);
		if (tab == nullptr) {
			tab = new Panel();
		}
		tab->set_position(0, 20);
		tab->set_size(width, height - 20);
		tabs.push_back(tab);
		this->add_component(tab_selector);
		this->add_component(tab);

		for (int i = 0; i < tab_selectors.size(); ++i) {
			if (i == 0) {
				tabs[i]->set_visible(true);
			} else {
				tabs[i]->set_visible(false);
			}
		}
	}

	void TabbedPanel::remove_tab(std::string tab_title) {
		int id = -1;
		for (int i = 0; i < tab_selectors.size(); ++i) {
			if (tab_selectors[i]->get_label()->get_text() == tab_title) {
				id = i;
			}
		}
		if (id != -1) {
			tab_selectors[id]->remove_event_listener(this);
			this->remove_component(tab_selectors[id]);
			this->remove_component(tabs[id]);
			tab_selectors.erase(tab_selectors.begin() + id);
			tabs.erase(tabs.begin() + id);
		}
	}

	void TabbedPanel::remove_tab(Panel* tab) {
		int id = -1;
		for (int i = 0; i < tabs.size(); ++i) {
			if (tabs[i] == tab) {
				id = i;
			}
		}
		if (id != -1) {
			tab_selectors[id]->remove_event_listener(this);
			this->remove_component(tab_selectors[id]);
			this->remove_component(tabs[id]);
			tab_selectors.erase(tab_selectors.begin() + id);
			tabs.erase(tabs.begin() + id);
		}
	}

	void TabbedPanel::set_tab_title(int id, std::string tab_title) {
		tab_selectors[id]->get_label()->set_text(tab_title);
	}

	void TabbedPanel::set_tab_title(Panel* tab, std::string tab_title) {
		int id = -1;
		for (int i = 0; i < tabs.size(); ++i) {
			if (tabs[i] == tab) {
				id = i;
			}
		}
		if (id != -1) {
			tab_selectors[id]->get_label()->set_text(tab_title);
		}
	}

	Panel* TabbedPanel::get_tab(int id) {
		return this->tabs[id];
	}

	Panel* TabbedPanel::get_tab(std::string tab_title) {
		for (int i = 0; i < tab_selectors.size(); ++i) {
			if (tab_selectors[i]->get_label()->get_text() == tab_title) {
				return tabs[i];
			}
		}
		throw "No tab was found with the given name!";
	}

	void TabbedPanel::action_performed(void* sender, Event& e) {
		for (int i = 0; i < tab_selectors.size(); ++i) {
			if (sender == tab_selectors[i]) {
				tabs[i]->set_visible(true);
			} else {
				tabs[i]->set_visible(false);
			}
		}
	}

	void TabbedPanel::draw() {
		//float offset = default_border_width / 2;
		float offset = 0;
		for (int i = 0; i < tab_selectors.size(); ++i) {
			tab_selectors[i]->set_position(offset, 0);
			tab_selectors[i]->set_size(tab_selectors[i]->get_label()->get_text().size() * char_width + 2 * char_width, 20 + default_border_width);
			offset = offset + tab_selectors[i]->get_width() - default_border_width;
			if (tabs[i]->get_visible()) {
				//tab_selectors[i]->set_background_color(200, 200, 200, 255);
				tab_selectors[i]->set_background_color(180, 180, 180, 255);
			} else {
				//tab_selectors[i]->set_background_color(150, 150, 150, 255);
				tab_selectors[i]->set_background_color(120, 120, 120);
			}
		}

		//Component::draw();
	}

}