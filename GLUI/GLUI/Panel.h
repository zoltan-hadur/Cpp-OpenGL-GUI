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
			this->draw(this->draw_background);

			if (this->parent) {
				GLint scissor_pos[4];
				glGetIntegerv(GL_SCISSOR_BOX, scissor_pos);

				float2 posp = float2(scissor_pos[0], glutGet(GLUT_WINDOW_HEIGHT) - (scissor_pos[1] + scissor_pos[3]));
				float2 sizep = float2(scissor_pos[2], scissor_pos[3]);
				float2 posc = this->get_absolute_position();
				float2 sizec = float2(this->get_width(), this->get_height());

				float2 posi = float2(std::max(posp.x, posc.x + (this->draw_background ? this->default_border_width : 0)),
									 std::max(posp.y, posc.y + (this->draw_background ? this->default_border_width : 0)));
				float2 posi2 = float2(std::min(posp.x + sizep.x, posc.x + sizec.x - (this->draw_background ? this->default_border_width : 0)),
									  std::min(posp.y + sizep.y, posc.y + sizec.y - (this->draw_background ? this->default_border_width : 0)));
				float2 sizei = float2(posi2.x - posi.x, posi2.y - posi.y);

				if (sizei.x > 5.0 && sizei.y > 5.0) {
					float x = posi.x;
					float y = glutGet(GLUT_WINDOW_HEIGHT) - (posi.y + sizei.y);
					float w = sizei.x;
					float h = sizei.y;

					glScissor(std::ceilf(x), std::ceilf(y), w, h);	// Allows partially drawing components

					auto children = this->children;
					for (auto c : children) {
						c->render();
					}
				}

				glScissor(scissor_pos[0], scissor_pos[1], scissor_pos[2], scissor_pos[3]);
			} else {
				this->draw(this->draw_background);
				auto children = this->children;
				for (auto c : children) {
					c->render();
				}
			}
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