#pragma once

#include <vector>
#include "Panel.h"
#include "Button.h"
#include "..\Event\ActionEvent.h"
#include "..\Event\ActionListener.h"
#include "..\Event\Event.h"
#include "..\Utility\Color.h"
#include "..\Utility\float2.h"
#include "..\Utility\Stopwatch.h"

namespace GLUI {

	// A panel that can has multiplte tabs, and each tab has it's own components
	class TabbedPanel : public ActionListener, public Panel {
	protected:
		std::vector<Button*> btn_selectors;		// Stores the tab titles, who responsible for navigation between the tabs
		std::vector<Panel*> pnl_tabs;			// Stores the tabs
		Panel* pnl_main_tab;					// Displays the tabs

		Stopwatch watch;						// Timer for animation
		float2 acc_pos;							// Stores accumulated position values for animation
		float anim_time;						// Determines how long the animation last (not accurate because of non linearity)

		float2 pos_offset;						// Position offset for animation
		float2 orig_pos;						// Original position for animation

		bool animating;							// Indicates if the panel is animating or not

		int selected;							// Selected tab's index

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		virtual void action_performed(void* sender, ActionEvent& e) override;

		// Position, size and border's width
		TabbedPanel(float x = 0, float y = 0, float width = 100, float height = 100, float border_width = 2);
		// Adds a tab to the panel
		void add_tab(std::string tab_title = "Tab", Panel* tab = nullptr);
		// Removes a tab from the panel
		void remove_tab(std::string tab_title);
		// Removes a tab from the panel
		void remove_tab(Panel* tab);
		// Sets a tab's title
		void set_tab_title(int id, std::string tab_title = "Tab");
		// Sets a tab's title
		void set_tab_title(Panel* tab, std::string tab_title = "Tab");
		// Gets a tab
		Panel* get_tab(int id);
		// Gets a tab
		Panel* get_tab(std::string tab_title);
	};

	void TabbedPanel::handle_event(Event& e) {

	}

	void TabbedPanel::draw(bool draw_background) {
		Panel::draw(draw_background);

		float2 offset = float2(0, 0);
		for (int i = 0; i < btn_selectors.size(); ++i) {																		// Calculate tab selector buttons' positions
			btn_selectors[i]->set_size(float2(btn_selectors[i]->get_label()->get_text().size() * char_width + 2 * char_width, 20));		// Automatic width
			if (offset.x + btn_selectors[i]->get_width() > this->get_width()) {
				offset.x = 0;
				offset.y = offset.y + btn_selectors[i]->get_height() - btn_selectors[i]->get_default_border_width();
			}
			btn_selectors[i]->set_position(offset);
			offset.x = offset.x + btn_selectors[i]->get_width() - btn_selectors[i]->get_default_border_width();

			if (i == this->selected) {																							// Highlight the selected tab's title
				btn_selectors[i]->set_background_color(this->highlight_color);
			} else {
				btn_selectors[i]->set_background_color(Color(120, 120, 120, 255) / 255);
			}
		}

		this->pnl_main_tab->set_position(float2(0, offset.y + 20 - this->default_border_width));										// Position the main tab according to the tab selectors
		this->pnl_main_tab->set_size(this->get_size() - float2(0, offset.y + 20 - this->default_border_width));

		for (int i = 0; i < this->pnl_tabs.size(); ++i) {
			this->pnl_tabs[i]->set_position(float2(i*this->pnl_main_tab->get_width(), 0));
		}

		if (this->animating && this->watch.is_running()) {
			float dt = this->watch.get_delta<Time::SECONDS>();																			// Get delta time
			float et = this->watch.get_elapsed<Time::SECONDS>();																			// Get elapsed time

			float2 speed_pos = (this->pos_offset - this->acc_pos) * 20 * et / this->anim_time;									// Calculate the current speed to modify the tab positions

			this->acc_pos = this->acc_pos + speed_pos * dt;																		// Accumulate the tab position offset

			bool x_in_pos = false;
			bool y_in_pos = false;
			if (std::fabs(this->pos_offset.x - this->acc_pos.x) < 1.0f) {														// Check if the x coordinate of the tab position offset has reached it's target value
				this->acc_pos.x = this->pos_offset.x;
				x_in_pos = true;
			}
			if (std::fabs(this->pos_offset.y - this->acc_pos.y) < 1.0f) {														// Check if the y coordinate of the tab position offset has reached it's target value
				this->acc_pos.y = this->pos_offset.y;
				y_in_pos = true;
			}

			if (x_in_pos && y_in_pos) {																							// If the tab position offset reached it's target value
				this->watch.stop();																								// The animation is over
				this->animating = false;
			}

		}

		for (int i = 0; i < this->pnl_tabs.size(); ++i) {
			this->pnl_tabs[i]->set_visible(this->animating || i == this->selected);												// All tab is visible when animating (altough tabs wich are outside of the panel are not being drawn becouse of the scissor)
			this->pnl_tabs[i]->set_size(this->pnl_main_tab->get_size());						// Set the tabs' size to fit in to the main tab
			this->pnl_tabs[i]->set_position(this->pnl_tabs[i]->get_position() + this->acc_pos);									// Translate all tab to the selected tab be visible
		}
	}

	void TabbedPanel::action_performed(void* sender, ActionEvent& e) {
		for (int i = 0; i < btn_selectors.size(); ++i) {																		// Iterate through the tab selector buttons to find wich one was pressed
			if (sender == btn_selectors[i] && e.button_released) {																// If found it
				if (i != this->selected) {																						// If it's not currently selected
					this->pos_offset = this->pos_offset + float2(this->get_width() * (this->selected - i), 0);						// Calculate the offset
					this->selected = i;
					this->animating = true;																						// Start animation

					if (this->watch.is_running()) {																				// If the watch is running
						this->watch.reset();																					// Reset it
					} else {																									// Else
						this->watch.start();																					// Start it
					}
				}
			}
		}
	}

	// Position, size and border's width
	TabbedPanel::TabbedPanel(float x, float y, float width, float height, float border_width) : Panel(true, x, y, width, height, border_width) {
		this->watch = Stopwatch();
		this->acc_pos = float2(0, 0);
		this->anim_time = 1;

		this->pos_offset = float2(0, 0);
		this->orig_pos = float2(0, 0);

		this->animating = false;
		this->selected = 0;

		this->pnl_main_tab = new Panel();
		this->pnl_main_tab->set_default_border_width(border_width);
		this->pnl_main_tab->set_background_color(Color(0, 0, 0, 0));
		this->pnl_main_tab->set_draw_background(false);
		this->add_component(pnl_main_tab);
	}

	// Adds a tab to the panel
	void TabbedPanel::add_tab(std::string tab_title, Panel* tab) {
		Button* tab_selector = new Button(tab_title);
		tab_selector->set_default_border_width(default_border_width);
		tab_selector->set_wait_time(Duration<Time::HOURS>(24));
		tab_selector->add_action_listener(this);
		btn_selectors.push_back(tab_selector);
		this->add_component(tab_selector);

		if (tab == nullptr) {
			tab = new Panel();
		}

		tab->set_background_color(Color(0, 0, 0, 0));
		tab->set_visible(false);
		pnl_tabs.push_back(tab);
		this->pnl_main_tab->add_component(tab);
	}

	// Removes a tab from the panel
	void TabbedPanel::remove_tab(std::string tab_title) {
		int id = -1;
		for (int i = 0; i < btn_selectors.size(); ++i) {
			if (btn_selectors[i]->get_label()->get_text() == tab_title) {
				id = i;
			}
		}
		if (id != -1) {
			btn_selectors[id]->remove_action_listener(this);
			this->remove_component(btn_selectors[id]);
			this->pnl_main_tab->remove_component(pnl_tabs[id]);
			btn_selectors.erase(btn_selectors.begin() + id);
			pnl_tabs.erase(pnl_tabs.begin() + id);
		}
	}

	// Removes a tab from the panel
	void TabbedPanel::remove_tab(Panel* tab) {
		int id = -1;
		for (int i = 0; i < pnl_tabs.size(); ++i) {
			if (pnl_tabs[i] == tab) {
				id = i;
			}
		}
		if (id != -1) {
			btn_selectors[id]->remove_action_listener(this);
			this->remove_component(btn_selectors[id]);
			this->pnl_main_tab->remove_component(pnl_tabs[id]);
			btn_selectors.erase(btn_selectors.begin() + id);
			pnl_tabs.erase(pnl_tabs.begin() + id);
		}
	}

	// Sets a tab's title
	void TabbedPanel::set_tab_title(int id, std::string tab_title) {
		btn_selectors[id]->get_label()->set_text(tab_title);
	}

	// Sets a tab's title
	void TabbedPanel::set_tab_title(Panel* tab, std::string tab_title) {
		int id = -1;
		for (int i = 0; i < pnl_tabs.size(); ++i) {
			if (pnl_tabs[i] == tab) {
				id = i;
			}
		}
		if (id != -1) {
			btn_selectors[id]->get_label()->set_text(tab_title);
		}
	}

	// Gets a tab
	Panel* TabbedPanel::get_tab(int id) {
		return this->pnl_tabs[id];
	}

	// Gets a tab
	Panel* TabbedPanel::get_tab(std::string tab_title) {
		for (int i = 0; i < btn_selectors.size(); ++i) {
			if (btn_selectors[i]->get_label()->get_text() == tab_title) {
				return pnl_tabs[i];
			}
		}
		throw "No tab was found with the given name!";
	}

}