#pragma once

#include "Component.h"

namespace GLUI {

	// Panel, that can store and display various components
	class Panel : public Component {
	protected:
		bool draw_background;

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
		virtual void render() override;
	public:
		// Draw background or not, the coordinates, the size, and the border's width
		Panel(bool draw_background = true, float x = 0, float y = 0, float width = 100, float height = 100, float border_width = 2);

		void set_draw_background(bool draw_background);
		bool is_draw_background();
	};

	void Panel::handle_event(Event& e) {
		
	}

	void Panel::draw(bool draw_background) {
		Component::draw(draw_background);
	}

	void Panel::render() {
		if (this->visible) {
			glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();								// Save current projection matrix
			glOrtho(0.0f, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0.0f, -1.0f, 1.0f);	// Transform it to able to draw in pixel coordinates
			glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();								// Save current modelview matrix
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glEnable(GL_BLEND);						// Enable transparency

			this->draw(this->draw_background);

			float2 pos = this->get_absolute_position();
			pos.x = pos.x + this->default_border_width;
			pos.y = glutGet(GLUT_WINDOW_HEIGHT) - (pos.y + this->height - this->default_border_width);								// y is inverted
			glScissor(pos.x, pos.y, this->width - this->default_border_width * 2, this->height - this->default_border_width * 2);	// Allows partially drawing components
			glEnable(GL_SCISSOR_TEST);
			auto children = this->children;
			for (auto c : children) {
				c->render();
			}
			glDisable(GL_SCISSOR_TEST);
			glDisable(GL_BLEND);
			glPopMatrix(); glMatrixMode(GL_PROJECTION);
			glPopMatrix(); glMatrixMode(GL_MODELVIEW);
		}
	}

	// Draw background or not, the coordinates, the size, and the border's width
	Panel::Panel(bool draw_background, float x, float y, float width, float height, float border_width) : Component(x, y, width, height, border_width) {
		this->draw_background = draw_background;
	}

	void Panel::set_draw_background(bool draw_background) {
		this->draw_background = draw_background;
	}

	bool Panel::is_draw_background() {
		return this->draw_background;
	}

}