#pragma once

#include <vector>
#include "Panel.h"
#include "Button.h"

namespace GLUI {

	class TabbedPanel : public EventListener, public Panel {
	protected:
		std::vector<Button*> tab_selectors;
		std::vector<Panel*> tabs;
		Panel* main_tab;
		Stopwatch watch;
		float anim_time;
		float offset;
		float acc;
		float acc2;
		bool animating;
		bool positive;
		int selected;
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
		virtual void draw(bool draw_background = true) override;
	};

	TabbedPanel::TabbedPanel(float x, float y, float width, float height, float border_width) : Panel(true, x, y, width, height, border_width) {
		this->watch = Stopwatch();
		this->anim_time = 1.0;
		this->offset = 0;
		this->acc = 0;
		this->acc2 = 0;
		this->animating = false;
		this->positive = false;
		this->selected = 0;

		this->main_tab = new Panel();
		this->main_tab->set_default_border_width(border_width);
		this->main_tab->set_background_color(255, 255, 255, 0);
		this->main_tab->set_draw_background(false);
		//this->main_tab->set_use_scissor(true);
		this->add_component(main_tab);
	}

	void TabbedPanel::add_tab(std::string tab_title, Panel* tab) {
		Button* tab_selector = new Button(tab_title);
		tab_selector->set_default_border_width(default_border_width);
		tab_selector->set_wait_time(std::numeric_limits<float>::max());
		tab_selector->add_event_listener(this);
		tab_selectors.push_back(tab_selector);
		this->add_component(tab_selector);

		if (tab == nullptr) {
			tab = new Panel();
		}
		//tab->set_position(0, 20);
		//tab->set_size(width, height - 20);
		tab->set_background_color(tab->get_background_color().get_r(),
								  tab->get_background_color().get_g(),
								  tab->get_background_color().get_b(),
								  0);
		tab->set_draw_background(false);
		//tab->set_use_scissor(true);
		tab->set_visible(false);
		tabs.push_back(tab);
		this->main_tab->add_component(tab);
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
			this->main_tab->remove_component(tabs[id]);
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
			this->main_tab->remove_component(tabs[id]);
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
				if (e.button_released) {
					if (i != this->selected) {
						if (i - this->selected != 0) {
							this->animating = true;
							this->watch.start();
						}
						this->positive = this->selected - i >= 0;
						this->acc2 = 0;
						this->offset = this->offset + this->width * (this->selected - i);
						this->selected = i;
					}
				}
			}
		}
	}

	void TabbedPanel::draw(bool draw_background) {
		Panel::draw(this->draw_background);
		//((TabbedPanel*)main_tab)->draw(this->draw_background);

		float2 offset = float2(0, 0);
		for (int i = 0; i < tab_selectors.size(); ++i) {
			tab_selectors[i]->set_size(tab_selectors[i]->get_label()->get_text().size() * char_width + 2 * char_width, 20);
			if (offset.x + tab_selectors[i]->get_width() > this->width) {
				offset.x = 0;
				offset.y = offset.y + tab_selectors[i]->get_height() - tab_selectors[i]->get_default_border_width();
			}
			tab_selectors[i]->set_position(offset);
			offset.x = offset.x + tab_selectors[i]->get_width() - tab_selectors[i]->get_default_border_width();

			if (i == this->selected) {
				tab_selectors[i]->set_background_color(180, 180, 180, 255);
			} else {
				tab_selectors[i]->set_background_color(120, 120, 120);
			}
		}

		this->main_tab->set_position(0, offset.y + 20 - this->default_border_width);
		this->main_tab->set_size(this->width, this->height - (offset.y + 20 - this->default_border_width));
		//this->set_use_scissor(true);
		this->main_tab->set_use_scissor(true);

		for (int i = 0; i < this->tabs.size(); ++i) {
			this->tabs[i]->set_position(i*this->main_tab->get_width(), 0);
		}

		if (this->animating && this->watch.is_running()) {
			float dt = this->positive ? this->watch.get_delta_time() : -this->watch.get_delta_time();
			this->acc2 = this->acc2 + std::fabs(dt);
			float speed = 20 * (std::fabs(this->offset - this->acc) * this->acc2) / this->anim_time + 20;

			this->acc = this->acc + speed*dt;

			if ((this->positive && this->acc > this->offset) || (!this->positive && this->acc < this->offset)) {
				this->acc = this->offset;
				this->acc2 = 0;
				this->watch.stop();
				this->animating = false;
			}
		}

		for (int i = 0; i < this->tabs.size(); ++i) {
			this->tabs[i]->set_visible(this->animating || i == this->selected);
			//this->tabs[i]->set_visible(true);
			this->tabs[i]->set_use_scissor(true);
			//this->tabs[i]->set_draw_background(true);
			this->tabs[i]->set_size(this->main_tab->get_width(), this->main_tab->get_height());
			this->tabs[i]->set_position(this->tabs[i]->get_position() + float2(acc, 0));
		}
	}

}