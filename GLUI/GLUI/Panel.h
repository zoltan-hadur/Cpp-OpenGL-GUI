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

			if (this->use_scissor && this->parent) {
				GLint scissor_pos[4];
				glGetIntegerv(GL_SCISSOR_BOX, scissor_pos);
				GLint scissor_test;
				glGetIntegerv(GL_SCISSOR_TEST, &scissor_test);

				float2 posp = this->parent->get_absolute_position();
				float2 sizep = float2(this->parent->get_width(), this->parent->get_height());
				float2 posc = this->get_absolute_position();
				float2 sizec = float2(this->get_width(), this->get_height());

				if (scissor_test) {
					posp.x = scissor_pos[0];
					posp.y = glutGet(GLUT_WINDOW_HEIGHT) - (scissor_pos[1] + scissor_pos[3]);
					sizep.x = scissor_pos[2];
					sizep.y = scissor_pos[3];
				}

				float2 posi = float2(std::max(posp.x, posc.x + (this->draw_background ? this->default_border_width : 0)),
									 std::max(posp.y, posc.y + (this->draw_background ? this->default_border_width : 0)));
				float2 posi2 = float2(std::min(posp.x + sizep.x, posc.x + sizec.x - (this->draw_background ? this->default_border_width : 0)),
									  std::min(posp.y + sizep.y, posc.y + sizec.y - (this->draw_background ? this->default_border_width : 0)));
				float2 sizei = float2(posi2.x - posi.x, posi2.y - posi.y);

				if (sizei.x > 5.0 && sizei.y > 5.0) {
					if (this->draw_background) {

						float x = posi.x;
						float y = glutGet(GLUT_WINDOW_HEIGHT) - (posi.y + sizei.y);
						float w = sizei.x;
						float h = sizei.y;

						glScissor(std::ceilf(x), std::ceilf(y), w, h);	// Allows partially drawing components
						glEnable(GL_SCISSOR_TEST);

					} else {
						float2 pos = posi;
						pos.x = pos.x;
						pos.y = glutGet(GLUT_WINDOW_HEIGHT) - (pos.y + sizei.y);								// y is inverted
						glScissor(pos.x, pos.y, sizei.x, sizei.y);	// Allows partially drawing components
						glEnable(GL_SCISSOR_TEST);
					}


					//this->draw(this->draw_background);
					auto children = this->children;
					for (auto c : children) {
						c->render();
					}
				}

				if (!scissor_test) {
					glDisable(GL_SCISSOR_TEST);
				}
				glScissor(scissor_pos[0], scissor_pos[1], scissor_pos[2], scissor_pos[3]);
			} else {
				this->draw(this->draw_background);
				auto children = this->children;
				for (auto c : children) {
					c->render();
				}
			}

			//if (this->use_scissor) {
			//	glDisable(GL_SCISSOR_TEST);
			//}

			glDisable(GL_BLEND);
			glPopMatrix(); glMatrixMode(GL_PROJECTION);
			glPopMatrix(); glMatrixMode(GL_MODELVIEW);
		}
	}

	// Draw background or not, the coordinates, the size, and the border's width
	Panel::Panel(bool draw_background, float x, float y, float width, float height, float border_width) : Component(x, y, width, height, border_width) {
		this->draw_background = draw_background;
		//this->use_scissor = true;
	}

	void Panel::set_draw_background(bool draw_background) {
		this->draw_background = draw_background;
	}

	bool Panel::is_draw_background() {
		return this->draw_background;
	}

}