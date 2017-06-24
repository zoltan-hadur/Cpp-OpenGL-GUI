#pragma once

#define _USE_MATH_DEFINES
#include <GL\freeglut.h>
#include <vector>
#include <algorithm>
#include "math.h"
#include "../Event/EventRaiser.h"

namespace GLUI {

	class Window;

	// Structure to store 2 floating points
	// Used mainly for positions
	struct float2 {
		float x;
		float y;

		float2() {
			this->x = 0;
			this->y = 0;
		}
		float2(float x, float y) {
			this->x = x;
			this->y = y;
		}
		float2 operator+(float2 p) {
			return float2(x + p.x, y + p.y);
		}
		float2 operator+(float p) {
			return float2(x + p, y + p);
		}
		float2 operator-(float2 p) {
			return float2(x - p.x, y - p.y);
		}
		float2 operator-(float p) {
			return float2(x - p, y - p);
		}
		float2 operator*(float2 p) {
			return float2(x * p.x, y * p.y);
		}
		float2 operator*(float p) {
			return float2(x * p, y * p);
		}
		float2 operator/(float2 p) {
			return float2(x / p.x, y / p.y);
		}
		float2 operator/(float p) {
			return float2(x / p, y / p);
		}
	};

	// Struct to store colors in RGBA form
	// Constructor takes the usual variables in 0-255 range,
	// but it stores them in the 0-1 range for OpenGL use
	class Color {
	private:
		float R;
		float G;
		float B;
		float A;
	public:
		// RGBA, range: 0-255
		Color(unsigned char R = 0U, unsigned char G = 0U, unsigned char B = 0U, unsigned char A = 255U) {
			this->R = R / 255.0f;
			this->G = G / 255.0f;
			this->B = B / 255.0f;
			this->A = A / 255.0f;
		}
		float get_r() {
			return this->R;
		}
		float get_g() {
			return this->G;
		}
		float get_b() {
			return this->B;
		}
		float get_a() {
			return this->A;
		}
		Color operator*(float f) {
			float r = std::min(255.0f, this->R * 255 * f);
			float g = std::min(255.0f, this->G * 255 * f);
			float b = std::min(255.0f, this->B * 255 * f);
			float a = std::min(255.0f, this->A * 255);
			return Color(r, g, b, a);
		}
	};

	// Abstract class, every gui element is a component
	class Component : public EventRaiser {
	protected:
		Component* parent;								// Parent component
		std::vector<Component*> children;				// Children components
		std::vector<Window*> minimized_windows;			// For minimizing windows
		static const unsigned char char_width = 9;		// Width of drawable char in pixels
		static const unsigned char char_height = 16;	// Height of drawable char in pixels
		float2 pos;										// Top left corner's position of the component relative to the parent component
		float width;									// Width of the component
		float height;									// Height of the component
		float border_width;								// Border width of the component
		float active_border_width;						// Border width of the component on event (if the derived class handles it)
		float default_border_width;						// Default border width of the component
		bool visible;									// Determines if the component is visible (if set to false, no children will be drawn)
		bool highlighted;								// Determines if the component is highlighted (i.e. mouse is above the component, if the derived class handles it)
		Color background_color;							// Color of the component's background
		Color border_color;								// Color of the component's border
		Color highlight_color;							// Color of the component's background when highlighted

		// x,y coordinates of the top left corner, relative to the parent component's top left corner, width and height, and border width
		Component(float x = 0, float y = 0, float width = 100, float height = 100, float border_width = 2);
		// Modifies states and raises events according to the input events
		virtual void handle_event(Event& e);
		// Draws the component
		virtual void draw(bool draw_background = true);
	public:
		// Brings the component in front of other components, if the mouse was above the component when the event happend
		void bring_front(Event& e);
		// Brings the component in front of other components
		void bring_front();
		// Returns true when the mouse is above of the component, but an other component is between them (an other component covers the component under the mouse)
		bool is_covered(Event& e);
		// Return true if the given component is is an ancestor of this component
		bool is_parent(Component* c);
		// Adds a component
		virtual void add_component(Component* c);
		// Adds a minimized window
		virtual void add_minimized_window(Window* window);
		// Removes a component
		virtual void remove_component(Component* c);
		// Removes a minimed window
		virtual void remove_minimized_window(Window* window);
		// Sets the position of the component on screen in pixels relative to the parent component
		virtual void set_position(float x, float y);
		// Sets the position of the component on screen in pixels relative to the parent component
		virtual void set_position(float2 pos);
		// Sets the size of the component on screen in pixels
		virtual void set_size(float width, float height);
		// Sets the width of the border of the component on screen in pixels
		virtual void set_default_border_width(float border_width);
		// Sets the title of the component
		// Sets the background color
		virtual void set_background_color(Color background_color);
		// Sets the background color
		virtual void set_background_color(unsigned char R = 0U, unsigned char G = 0U, unsigned char B = 0U, unsigned char A = 255U);
		// Sets the border color
		virtual void set_border_color(Color background_color);
		// Sets the border color
		virtual void set_border_color(unsigned char R = 0U, unsigned char G = 0U, unsigned char B = 0U, unsigned char A = 255U);
		// Sets the highlight color
		virtual void set_highlight_color(Color background_color);
		// Sets the highlight color
		virtual void set_highlight_color(unsigned char R = 0U, unsigned char G = 0U, unsigned char B = 0U, unsigned char A = 255U);
		// Sets the visibility of the component (if set to false, no children will be drawn)
		virtual void set_visible(bool visible);
		// Gets a minimized window list
		virtual std::vector<Window*> get_minimized_windows();
		// Gets the position of the component relative to the parent component
		virtual float2 get_position();
		// Gets the absolute position of the component
		virtual float2 get_absolute_position();
		// Gets the width of the component
		virtual float get_width();
		// Gets the height of the component
		virtual float get_height();
		// Gets the border's width of the component
		virtual float get_default_border_width();
		// Gets the background color of the component
		virtual Color get_background_color();
		// Gets the border's color of the component
		virtual Color get_border_color();
		// Gets the highlight color of the component
		virtual Color get_highlight_color();
		// Gets visibility of the component
		virtual bool is_visible();

		// Handles input events (keyboard, mouse)
		// Just create events in the main loop and pass them to the top-level window
		virtual void event_handler(Event& e);
		// Draws the component and it's children components
		// Just call it on a top-level window object, and all children components will be drawn
		virtual void render();
	};

	// x,y coordinates of the top left corner, relative to the parent component, width and height, and border width
	Component::Component(float x, float y, float width, float height, float border_width) {
		this->parent = nullptr;
		this->pos = float2(x, y);
		this->width = width;
		this->height = height;
		this->border_width = border_width;
		this->default_border_width = border_width;
		this->active_border_width = border_width + 2;
		this->visible = true;
		this->highlighted = false;
		this->background_color = Color(100, 100, 100, 180);
		this->border_color = Color(200, 200, 200, 255);
		//this->highlight_color = Color(66, 134, 244, 255);
		this->highlight_color = Color(160, 160, 160, 255);
	}

	// Raises events according to input events
	void Component::handle_event(Event& e) {

	}

	// Draws the component
	void Component::draw(bool draw_background) {
		if (!draw_background) {
			return;
		}

		float2 pos = this->get_absolute_position();

		//// Draw border
		//// Top left quarter-circle
		//glColor4f(this->border_color.get_r(),
		//		  this->border_color.get_g(),
		//		  this->border_color.get_b(),
		//		  this->border_color.get_a());
		//glBegin(GL_POLYGON);
		//for (int i = 0; i <= this->border_width; ++i) {
		//	float t = M_PI + i * M_PI / 2.0f / this->border_width;
		//	float r = this->border_width;
		//	float x = r*cos(t) + pos.x + this->border_width;
		//	float y = r*sin(t) + pos.y + this->border_width;
		//	glVertex2f(x, y);
		//}
		//// Top bar
		//glVertex2f(pos.x + this->border_width, pos.y);
		//glVertex2f(pos.x + this->width - this->border_width, pos.y);
		//// Top right quarter-circle
		//for (int i = 0; i <= this->border_width; ++i) {
		//	float t = -M_PI / 2.0f + i * M_PI / 2.0f / this->border_width;
		//	float r = this->border_width;
		//	float x = r*cos(t) + pos.x + this->width - this->border_width;
		//	float y = r*sin(t) + pos.y + this->border_width;
		//	glVertex2f(x, y);
		//}
		//glEnd();
		//// Bot right quarter-circle
		//glBegin(GL_POLYGON);
		//for (int i = 0; i <= this->border_width; ++i) {
		//	float t = i * M_PI / 2.0f / this->border_width;
		//	float r = this->border_width;
		//	float x = r*cos(t) + pos.x + this->width - this->border_width;
		//	float y = r*sin(t) + pos.y + this->height - this->border_width;
		//	glVertex2f(x, y);
		//}
		//// Bot bar
		//glVertex2f(pos.x + this->width - this->border_width, pos.y + this->height);
		//glVertex2f(pos.x + this->border_width, pos.y + this->height);
		//// Bot left quarter-circle
		//for (int i = 0; i <= this->border_width; ++i) {
		//	float t = M_PI / 2.0f + i * M_PI / 2.0f / this->border_width;
		//	float r = this->border_width;
		//	float x = r*cos(t) + pos.x + this->border_width;
		//	float y = r*sin(t) + pos.y + this->height - this->border_width;
		//	glVertex2f(x, y);
		//}
		//glEnd();
		//// Left bar
		//glBegin(GL_QUADS);
		//glVertex2f(pos.x, pos.y + this->border_width);
		//glVertex2f(pos.x + this->border_width, pos.y + this->border_width);
		//glVertex2f(pos.x + this->border_width, pos.y + this->height - this->border_width);
		//glVertex2f(pos.x, pos.y + this->height - this->border_width);
		//// Right bar
		//glVertex2f(pos.x + this->width - this->border_width, pos.y + this->border_width);
		//glVertex2f(pos.x + this->width, pos.y + this->border_width);
		//glVertex2f(pos.x + this->width, pos.y + this->height - this->border_width);
		//glVertex2f(pos.x + this->width - this->border_width, pos.y + this->height - this->border_width);
		//glEnd();

		// Draw border
		glColor4f(this->border_color.get_r(),
				  this->border_color.get_g(),
				  this->border_color.get_b(),
				  this->border_color.get_a());
		glBegin(GL_QUADS);
		// Left
		glVertex2f(pos.x, pos.y);
		glVertex2f(pos.x + this->border_width, pos.y);
		glVertex2f(pos.x + this->border_width, pos.y + this->height);
		glVertex2f(pos.x, pos.y + this->height);
		// Right
		glVertex2f(pos.x + this->width - this->border_width, pos.y);
		glVertex2f(pos.x + this->width, pos.y);
		glVertex2f(pos.x + this->width, pos.y + this->height);
		glVertex2f(pos.x + this->width - this->border_width, pos.y + this->height);
		// Up
		glVertex2f(pos.x, pos.y);
		glVertex2f(pos.x + this->width, pos.y);
		glVertex2f(pos.x + this->width, pos.y + this->border_width);
		glVertex2f(pos.x, pos.y + this->border_width);
		// Down
		glVertex2f(pos.x, pos.y + this->height - this->border_width);
		glVertex2f(pos.x + this->width, pos.y + this->height - this->border_width);
		glVertex2f(pos.x + this->width, pos.y + this->height);
		glVertex2f(pos.x, pos.y + this->height);
		glEnd();

		// Draw background
		if (highlighted) {
			glColor4f(this->highlight_color.get_r(),
					  this->highlight_color.get_g(),
					  this->highlight_color.get_b(),
					  this->highlight_color.get_a());
		} else {
			glColor4f(this->background_color.get_r(),
					  this->background_color.get_g(),
					  this->background_color.get_b(),
					  this->background_color.get_a());
		}
		glBegin(GL_QUADS);
		glVertex2f(pos.x + this->border_width, pos.y + this->border_width);
		glVertex2f(pos.x + this->width - this->border_width, pos.y + this->border_width);
		glVertex2f(pos.x + this->width - this->border_width, pos.y + this->height - this->border_width);
		glVertex2f(pos.x + this->border_width, pos.y + this->height - this->border_width);
		glEnd();
	}

	// Brings the component in front of other components, if the mouse was above the component when the event happend
	void Component::bring_front(Event& e) {
		bool found = false;
		auto children = this->children;																			// A seperate container because it will be modified during the iteration
		do {
			found = false;
			for (int i = children.size() - 1; i >= 0 && !found; --i) {											// Iterate through the children component backward (because the drawing happens forward, hence the last component is above of every component)
				auto c = children[i];
				if (c->is_visible()) {
					float2 pos = c->get_absolute_position();
					if (pos.x < e.x && e.x < pos.x + c->width && pos.y < e.y && e.y < pos.y + c->height) {		// If the component is visible and below of the mouse
						if (c->parent) {
							c->parent->remove_component(c);														// A remove
							c->parent->add_component(c);														// And then an add moves the component to the end of the list
						}
						children = children[i]->children;
						found = true;
					}
				}
			}
		} while (children.size() > 1 && found);																	// Do until there is no more children, or not found any component that could be bring front
	}

	// Brings the component in front of other components
	void Component::bring_front() {
		if (this->parent) {																						// If the component has a parent
			this->parent->remove_component(this);																// Move this component
			this->parent->add_component(this);																	// To the end of the list
			this->parent->bring_front();																		// And do this recursively
		}
	}

	// Returns true when the mouse is above of the component, but an other component is between them (an other component covers the component under the mouse)
	bool Component::is_covered(Event& e) {
		if (this->parent == nullptr) {																			// A top level window cannot be covered
			return false;
		}
		Component* parent = this;
		while (parent->parent) {																				// Get one of the top level window's children
			parent = parent->parent;
		}

		bool covered = false;
		auto children = parent->children;
		do {
			covered = false;
			for (int i = children.size() - 1; i >= 0 && !covered; --i) {
				auto c = children[i];
				if (c != this) {

					float2 posp = c->get_absolute_position();
					float2 sizep = float2(c->get_width(), c->get_height());
					float2 post = this->get_absolute_position();
					float2 sizet = float2(this->get_width(), this->get_height());

					float2 posi = float2(std::max(posp.x, post.x),
										 std::max(posp.y, post.y));
					float2 posi2 = float2(std::min(posp.x + sizep.x, post.x + sizet.x),
										  std::min(posp.y + sizep.y, post.y + sizet.y));
					float2 sizei = float2(posi2.x - posi.x, posi2.y - posi.y);

					if (posi.x < e.x && e.x < posi.x + sizei.x && posi.y < e.y && e.y < posi.y + sizei.y) {		// The mouse is inside the intersected rectangle (i.e. above of this component and the potential covering component)
						if (this->is_parent(c)) {																// If the potential covering component contains this component
							covered = false;																	// Then of course it's not covering it's own component
							children = c->children;																// Get it's children for iterating through them
							break;
						} else {																				// If the potential covering component does not contain this component
							return true;																		// Return true, because then it is a covering component
						}
					} else {																					// The potential covering component has no
						covered = false;																		// intersection with this component, thus the false value
					}
				} else {																						// Because the loop goes in z depth (from last drawen to first drawen)
					return false;																				// If it's reached itself, return false, because it does not cover itself, and all the remain children should be drawen earlier than this component
				}
				if (children.size() <= 1 || i == 0) {
					return covered;
				}
			}
		} while (!covered && children.size() > 1);
		return covered;
	}

	// Return true if the given component is is an ancestor of this component
	bool Component::is_parent(Component* c) {
		Component* parent = this;
		while (parent) {
			if (parent == c) {
				return true;
			}
			parent = parent->parent;
		}
		return false;
	}

	// Adds a component
	void Component::add_component(Component* c) {
		c->parent = this;
		this->children.push_back(c);
	}

	// Adds a minimized window
	void Component::add_minimized_window(Window* window) {
		this->minimized_windows.push_back(window);
	}

	// Removes a component
	void Component::remove_component(Component* c) {
		this->children.erase(std::remove(this->children.begin(), this->children.end(), c), this->children.end());
	}

	// Removes a minimed window
	void Component::remove_minimized_window(Window* window) {
		this->minimized_windows.erase(std::remove(this->minimized_windows.begin(), this->minimized_windows.end(), window), this->minimized_windows.end());
	}

	// Sets the position of the component on screen in pixels relative to the parent component
	void Component::set_position(float x, float y) {
		this->pos = float2(x, y);
	}

	// Sets the position of the component on screen in pixels relative to the parent component
	void Component::set_position(float2 pos) {
		this->pos = pos;
	}

	// Sets the size of the component on screen in pixels
	void Component::set_size(float width, float height) {
		this->width = width;
		this->height = height;
	}

	// Sets the width of the border of the component on screen in pixels
	void Component::set_default_border_width(float border_width) {
		this->default_border_width = border_width;
		this->border_width = border_width;
		this->active_border_width = border_width + 2;
	}

	// Sets the background color
	void Component::set_background_color(Color background_color) {
		this->background_color = background_color;
	}

	// Sets the background color
	void Component::set_background_color(unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
		this->background_color = Color(R, G, B, A);
	}

	// Sets the border color
	void Component::set_border_color(Color background_color) {
		this->border_color = background_color;
	}

	// Sets the border color
	void Component::set_border_color(unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
		this->border_color = Color(R, G, B, A);
	}

	// Sets the highlight color
	void Component::set_highlight_color(Color background_color) {
		this->highlight_color = highlight_color;
	}

	// Sets the highlight color
	void Component::set_highlight_color(unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
		this->highlight_color = Color(R, G, B, A);
	}

	// Sets the visibility of the component (if set to false, no children will be drawn)
	void Component::set_visible(bool visible) {
		this->visible = visible;
		for (auto c : this->children) {
			c->set_visible(visible);
		}
	}

	// Gets a minimized window list
	std::vector<Window*> Component::get_minimized_windows() {
		return this->minimized_windows;
	}

	// Gets the position of the component relative to the parent component
	float2 Component::get_position() {
		return this->pos;
	}

	// Gets the absolute position of the component
	float2 Component::get_absolute_position() {
		if (this->parent) {
			return this->pos + this->parent->get_absolute_position();
		} else {
			return this->pos;
		}
	}

	// Gets the width of the component
	float Component::get_width() {
		return this->width;
	}

	// Gets the height of the component
	float Component::get_height() {
		return this->height;
	}

	// Gets the border's width of the component
	float Component::get_default_border_width() {
		return this->default_border_width;
	}

	// Gets the background color of the component
	Color Component::get_background_color() {
		return this->background_color;
	}

	// Gets the border's color of the component
	Color Component::get_border_color() {
		return this->border_color;
	}

	// Gets the highlight color of the component
	Color Component::get_highlight_color() {
		return this->highlight_color;
	}

	// Gets visibility of the component
	bool Component::is_visible() {
		return this->visible;
	}

	// Handles input events (keyboard, mouse)
	void Component::event_handler(Event& e) {
		if (this->visible) {
			e.mouse_covered = this->is_covered(e);
			this->handle_event(e);
			auto children = this->children;
			for (auto c : children) {
				c->event_handler(e);
			}
		}
	}

	// Draws the component and it's children components
	// Just call it on a window object, and all children components will be drawn
	void Component::render() {
		if (this->visible) {
			this->draw();
			auto children = this->children;
			for (auto c : children) {
				c->render();
			}
		}
	}

}