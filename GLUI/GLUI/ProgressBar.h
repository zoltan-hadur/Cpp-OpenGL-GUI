#pragma once

#include <string>
#include "Component.h"
#include "../Stopwatch.h"

namespace GLUI {

	class ProgressBar : public Component {
	protected:
		enum class ANIMATION;

		ANIMATION anim;				// Wich animation should be displayed
		Label* lbl_progressing;		// For progressing animation
		Label* lbl_percent;			// For displaying the percentage
		Stopwatch watch;			// For progressing animation
		Color fill_color;			// Color of the progress bar

		float period_time;			// Progressing animation's period
		bool progressing;			// True if progressing, false if not

		float min;
		float max;
		float value;				// Calculate percentage based on this value (0% if value==min and 100% if value==max)

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		enum class ANIMATION {		// Animation types
			TEXT_DOTS,				// Progressing, Progressing., Progressing.., Progressing...
			TEXT_SPINNING,			/* Progressing |, Progressing /, Progressing -, Progressing \*/
			STRIPES,				// Stripes flowing right
			WINDOWS					// A brighter area goes from left to right
		};

		// Min, max, coordinates, size, border's width
		ProgressBar(float min = 0, float max = 1, float x = 0, float y = 0, float width = 100, float height = 20, float border_width = 1);
		// Starts the progressing animation and resets the value
		void start_progress();
		// Stops the progressing animation and prevents any modification tot he value
		void stop_progress();
		// Sets the animation's period (minimum is 0.1 sec)
		// For TEXT_DOTS and TEXT_SPINNING, it's a real period	(recommended is 2.0 and 1.0 sec)
		// For STRIPES, it determines how much time needed to flow 2 stripe (recommended is 1.0 sec)
		// For WINDOWS, it determines how much time needed to start the animation again (recommended is 1.0 sec)
		void set_period_time(float period_time);
		// Sets the min
		void set_min(float min);
		// Sets the max
		void set_max(float max);
		// Sets the value (will be between min and max)
		void set_value(float value);
		// Sets the filling color
		void set_fill_color(unsigned char R = 0U, unsigned char G = 0U, unsigned char B = 0U, unsigned char A = 255U);
		// Sets the filling color
		void set_fill_color(Color fill_color);
		// Sets the animation
		void set_animation(ANIMATION anim);
		// Gets the animation period
		float get_period_time();
		// Gets the min
		float get_min();
		// Gets the max
		float get_max();
		// Gets the value
		float get_value();
		// Gets the filling color
		Color get_fill_color();
		// Gets the animation
		ANIMATION get_animation();
	};

	void ProgressBar::handle_event(Event& e) {

	}

	void ProgressBar::draw(bool draw_background) {
		Component::draw();																											// Draw base

		float percent = (this->value - this->min) / (this->max - this->min);														// Calculate percentage (range: 0-1)

		char buf[10];
		std::sprintf(buf, "%6.2f %%", percent * 100);																				// Create the string storing the percentage in the 100.00 % form
		this->lbl_percent->set_text(std::string(buf));
		this->lbl_percent->set_position(0, 0);																						// As the label aligned mid-mid by default
		this->lbl_percent->set_size(this->width, this->height);																		// The label will be in the center of the progress bar


		if (this->anim == ANIMATION::TEXT_DOTS || this->anim == ANIMATION::TEXT_SPINNING) {											// Handle animation
			if (this->progressing) {																								// Only animating when something is progressing
				if (this->watch.is_running()) {																						// Must check, else an exception will be thrown
					switch (this->anim) {
						case ANIMATION::TEXT_DOTS:																					// Text goes in this order
						{																											// Progressing
							if (watch.get_elapsed_time() < 1.0f / 4.0f * this->period_time) {										// Progressing.
								this->lbl_progressing->set_text(" Progressing");													// Progressing..
							} else if (watch.get_elapsed_time() < 2.0f / 4.0f * this->period_time) {								// Progressing...
								this->lbl_progressing->set_text(" Progressing.");
							} else if (watch.get_elapsed_time() < 3.0f / 4.0f * this->period_time) {								// Each displayed for 1/4 of the period
								this->lbl_progressing->set_text(" Progressing..");
							} else if (watch.get_elapsed_time() < 4.0f / 4.0f * this->period_time) {
								this->lbl_progressing->set_text(" Progressing...");
							} else {
								this->watch.reset();																				// Reset the watch if a full period elapsed
							}
							break;
						}
						case ANIMATION::TEXT_SPINNING:																				// Text goes in this order
						{																											// Progressing |
							if (watch.get_elapsed_time() < 1.0f / 4.0f * this->period_time) {										/* Progressing / */
								this->lbl_progressing->set_text(" Progressing |");													// Progressing -
							} else if (watch.get_elapsed_time() < 2.0f / 4.0f * this->period_time) {								/* Progressing \ */
								this->lbl_progressing->set_text(" Progressing /");
							} else if (watch.get_elapsed_time() < 3.0f / 4.0f * this->period_time) {								// Each displayed for 1/4 of the period
								unsigned char c = '\x12';																			// Wider '-' character (at least on my pc)
								this->lbl_progressing->set_text(" Progressing " + std::string(reinterpret_cast<char*>(&c), 1));
							} else if (watch.get_elapsed_time() < 4.0f / 4.0f * this->period_time) {
								this->lbl_progressing->set_text(" Progressing \\");
							} else {
								this->watch.reset();																				// Reset the watch if a full period elapsed
							}
							break;
						}
					}
				}
			}
			this->lbl_progressing->set_visible(this->progressing);																	// The label 'animation' only visible when progressing
			this->lbl_progressing->set_position(0, 0);
			this->lbl_progressing->set_size(this->width, this->height);
			this->lbl_progressing->set_h_align(Label::H_ALIGN::LEFT);																// The label's text changes, but I want it to be always mid-left aligned
		} else {
			this->lbl_progressing->set_visible(false);																				// Not visible, because not a TEXT animation is applied
		}


		// Draw progress bar
		if (percent > 0) {
			// Always drawing a rectangle that overlaps the appropriate percentage of the area
			float2 pos = this->get_absolute_position();
			glColor4f(this->fill_color.get_r(),
					  this->fill_color.get_g(),
					  this->fill_color.get_b(),
					  this->fill_color.get_a());
			glBegin(GL_QUADS);
			glVertex2f(pos.x + this->default_border_width, pos.y + this->default_border_width);
			glVertex2f(pos.x + (this->width - this->default_border_width)*percent, pos.y + this->default_border_width);
			glVertex2f(pos.x + (this->width - this->default_border_width)*percent, pos.y + this->height - this->default_border_width);
			glVertex2f(pos.x + this->default_border_width, pos.y + this->height - this->default_border_width);
			glEnd();

			// Draw animation
			if (this->progressing) {
				GLint scissor_pos[4];
				glGetIntegerv(GL_SCISSOR_BOX, scissor_pos);
				GLint scissor_test;
				glGetIntegerv(GL_SCISSOR_TEST, &scissor_test);

				float2 posp = this->parent->get_absolute_position();
				float2 sizep = float2(this->parent->get_width(), this->parent->get_height());
				float2 posc = this->get_absolute_position();
				float2 sizec = float2(this->get_width() * percent, this->get_height());

				if (scissor_test) {
					posp.x = scissor_pos[0];
					posp.y = glutGet(GLUT_WINDOW_HEIGHT) - (scissor_pos[1] + scissor_pos[3]);
					sizep.x = scissor_pos[2];
					sizep.y = scissor_pos[3];
				}

				float2 posi = float2(std::max(posp.x, posc.x), std::max(posp.y, posc.y));
				float2 posi2 = float2(std::min(posp.x + sizep.x, posc.x + sizec.x), std::min(posp.y + sizep.y, posc.y + sizec.y));
				float2 sizei = float2(posi2.x - posi.x, posi2.y - posi.y);

				if (sizei.x > 5.0 && sizei.y > 5.0) {
					float2 pos = posi;
					pos.x = pos.x + this->default_border_width;
					pos.y = glutGet(GLUT_WINDOW_HEIGHT) - (pos.y + sizei.y - this->default_border_width);								// y is inverted
					glScissor(pos.x, pos.y, sizei.x - this->default_border_width * 2, sizei.y - this->default_border_width * 2);	// Allows partially drawing components
					glEnable(GL_SCISSOR_TEST);

					switch (this->anim) {
						case ANIMATION::STRIPES:
						{
							if (this->watch.is_running()) {
								if (this->watch.get_elapsed_time() > this->period_time) {					// Reset watch if a full period has elapsed
									this->watch.reset();
								}
							}

							float offset = this->watch.get_elapsed_time() / period_time;					// How much needed to be translated
							pos = this->get_absolute_position() - float2(15, 0) + float2(20 * offset, 0);	// -15 so the bottom left vertex starts from 0, and 20 becouse the drawable parallelogram is 20 wide
							glBegin(GL_QUADS);
							for (int i = 0; i <= (this->width / 20 + 1)*percent; ++i) {						// Draw the parallelogram n times
								glColor4f(this->fill_color.get_r(),
										  this->fill_color.get_g(),
										  this->fill_color.get_b(),
										  this->fill_color.get_a());
								glVertex2f(pos.x, pos.y);
								glVertex2f(pos.x + 10, pos.y);
								glVertex2f(pos.x + 5, pos.y + this->height);
								glVertex2f(pos.x - 5, pos.y + this->height);
								pos = pos + float2(10, 0);

								glColor4f(this->highlight_color.get_r(),
										  this->highlight_color.get_g(),
										  this->highlight_color.get_b(),
										  this->highlight_color.get_a());
								glVertex2f(pos.x, pos.y);
								glVertex2f(pos.x + 10, pos.y);
								glVertex2f(pos.x + 5, pos.y + this->height);
								glVertex2f(pos.x - 5, pos.y + this->height);
								pos = pos + float2(10, 0);
							}
							glEnd();
							break;
						}
						case ANIMATION::WINDOWS:
						{
							float speed = (60 + this->width) / 3.0;		// Speed of the travelling birghter area
							if (this->watch.is_running()) {
								float dist_over = this->watch.get_elapsed_time() * speed - (60 + this->width*percent);	// How much the brighter area travelled outside the progress bar
								if (dist_over > 0) {
									if (dist_over / speed > this->period_time) {	// If travelled outside more than the period time,
										this->watch.reset();						// Reset the watch
									}
								}
							}
							pos = this->get_absolute_position() - float2(60, 0) + float2(this->watch.get_elapsed_time()*speed, 0);	// -60 because the brighter area is 60 wide
							glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);														// For proper blending
							glBegin(GL_QUADS);																						// The brighter area is actually
							glColor4f(this->fill_color.get_r(),																		// Two 30 wide rectangle
									  this->fill_color.get_g(),																		// With color interpolation
									  this->fill_color.get_b(),																		// From 0,fill_color to 30,brighter color
									  this->fill_color.get_a());																	// And from 30, brighter color to 60, fill_color
							glVertex2f(pos.x, pos.y + this->height);																// So the 60 wide rectangle's 2 edge has fill_color
							glVertex2f(pos.x, pos.y);																				// While the middle has a brighter color
							glColor4f(0.9, 0.9, 0.9, 1);
							glVertex2f(pos.x + 30, pos.y);
							glVertex2f(pos.x + 30, pos.y + this->height);
							pos = pos + float2(30, 0);

							glVertex2f(pos.x, pos.y + this->height);
							glVertex2f(pos.x, pos.y);
							glColor4f(this->fill_color.get_r(),
									  this->fill_color.get_g(),
									  this->fill_color.get_b(),
									  this->fill_color.get_a());
							glVertex2f(pos.x + 30, pos.y);
							glVertex2f(pos.x + 30, pos.y + this->height);

							glEnd();
							glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							break;
						}
						default:
						{
							break;
						}
					}
					if (!scissor_test) {
						glDisable(GL_SCISSOR_TEST);
					}
					glScissor(scissor_pos[0], scissor_pos[1], scissor_pos[2], scissor_pos[3]);
				}
			}
		}
	}

	ProgressBar::ProgressBar(float min, float max, float x, float y, float width, float height, float border_width) : Component(x, y, width, height, border_width) {
		this->anim = ANIMATION::STRIPES;
		this->period_time = 1.0;
		this->progressing = false;
		this->min = min;
		this->max = max;
		this->value = min;
		this->background_color = Color(120, 120, 120, 255);		// Default grey color
		this->fill_color = Color(180, 180, 180, 255);
		this->lbl_progressing = new Label();
		this->lbl_percent = new Label();
		this->add_component(this->lbl_progressing);
		this->add_component(this->lbl_percent);
	}

	// Starts the progressing and resets the value
	void ProgressBar::start_progress() {
		this->progressing = true;
		this->value = this->min;
		if (this->watch.is_running()) {
			this->watch.reset();
		} else {
			this->watch.start();
		}
	}

	// Stops the progressing and prevents any modification tot he value
	void ProgressBar::stop_progress() {
		this->progressing = false;
		this->watch.stop();
	}

	// Sets the animation's period (minimum is 0.1 sec)
	// For TEXT_DOTS and TEXT_SPINNING, it's a real period	(recommended is 2.0 and 1.0 sec)
	// For STRIPES, it determines how much time needed to flow 2 stripe (recommended is 1.0 sec)
	// For WINDOWS, it determines how much time needed to start the animation again (recommended is 1.0 sec)
	void ProgressBar::set_period_time(float period_time) {
		this->period_time = std::max(period_time, 0.1f);
	}

	// Sets the min (only when not progressing)
	void ProgressBar::set_min(float min) {
		if (!this->progressing) {
			this->min = std::min(min, this->max);		// Can't be bigger than max
			this->value = std::max(this->value, min);	// Adjust the value if new min is bigger than value
		}
	}

	// Sets the max (only when not progressing)
	void ProgressBar::set_max(float max) {
		if (!this->progressing) {
			this->max = std::max(max, this->min);		// Can't be smaller than min
			this->value = std::min(this->value, max);	// Adjust eh value if new max is smaller than value
		}
	}

	// Sets the value (only when progressing, and will be between min and max)
	void ProgressBar::set_value(float value) {
		if (this->progressing) {
			this->value = std::max(std::min(value, this->max), this->min);	// Must be between min and max
		}
	}

	// Sets the filling color
	void ProgressBar::set_fill_color(unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
		this->fill_color = Color(R, G, B, A);
	}

	// Sets the filling color
	void ProgressBar::set_fill_color(Color fill_color) {
		this->fill_color = fill_color;
	}

	// Sets the animation
	void ProgressBar::set_animation(ANIMATION anim) {

	}

	// Gets the animation period
	float ProgressBar::get_period_time() {
		return this->period_time;
	}

	// Gets the min
	float ProgressBar::get_min() {
		return this->min;
	}

	// Gets the max
	float ProgressBar::get_max() {
		return this->max;
	}

	// Gets the value
	float ProgressBar::get_value() {
		return this->value;
	}

	// Gets the filling color
	Color ProgressBar::get_fill_color() {
		return this->fill_color;
	}

	// Gets the animation
	ProgressBar::ANIMATION ProgressBar::get_animation() {
		return this->anim;
	}

}