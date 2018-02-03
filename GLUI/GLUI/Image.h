#pragma once

#include <GL\freeglut.h>
#include <GL\glext.h>
#include <string>
#include "Component.h"
#include "..\Event\Event.h"
#include "..\Utility\BMP.h"
#include "..\Utility\float2.h"

namespace GLUI {

	// An image that can be loaded from a file and can be displayed like every other component
	class Image : public Component {
	protected:
		GLuint tex_id;		// The texture's id (because the image is stored in a texture)

		virtual void handle_event(Event& e) override;
		virtual void draw(bool draw_background = true) override;
	public:
		// Path to the image, it's position on screen and it's size
		Image(std::string file_path = "NonExistent", float x = 0, float y = 0, float width = 0, float height = 0);

		// Retores the original size of the image
		void restore_original_size();
		// Loads the image from the file
		void load_image(std::string file_path);
		// Gets the image's width (NOT the component width!)
		int get_image_width();
		// Gets the image's height (NOT the compoent height!)
		int get_image_height();
	};

	void Image::handle_event(Event& e) {

	}

	void Image::draw(bool draw_background) {
		Component::draw(draw_background);				// Draw base

		float2 pos = this->get_absolute_position();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->tex_id);
		glColor4f(1, 1, 1, 1);
		glBegin(GL_QUADS);								// Draw the image to the screen
		if (draw_background) {
			glTexCoord2f(0, 0); glVertex2f(pos.x + this->default_border_width, pos.y + this->get_height() - this->default_border_width);
			glTexCoord2f(0, 1); glVertex2f(pos.x + this->default_border_width, pos.y + this->default_border_width);
			glTexCoord2f(1, 1); glVertex2f(pos.x + this->get_width() - this->default_border_width, pos.y + this->default_border_width);
			glTexCoord2f(1, 0); glVertex2f(pos.x + this->get_width() - this->default_border_width, pos.y + this->get_height() - this->default_border_width);
		} else {
			glTexCoord2f(0, 0); glVertex2f(pos.x, pos.y + this->get_height());
			glTexCoord2f(0, 1); glVertex2f(pos.x, pos.y);
			glTexCoord2f(1, 1); glVertex2f(pos.x + this->get_width(), pos.y);
			glTexCoord2f(1, 0); glVertex2f(pos.x + this->get_width(), pos.y + this->get_height());
		}
		glEnd();
	}

	// Path to the image, it's position on screen and it's size
	Image::Image(std::string file_path, float x, float y, float width, float height) : Component(x, y, width, height, 0) {
		this->load_image(file_path);
		this->set_size(float2(width == 0 ? this->get_image_width() : width,
							  height == 0 ? this->get_image_height() : height));
	}

	// Retores the original size of the image
	void Image::restore_original_size() {
		this->set_size(float2(this->get_image_width(),
							  this->get_image_height()));
	}

	// Loads the image from the file
	void Image::load_image(std::string file_path) {
		std::vector<BMP::uchar> image;																		// This will store the raw data
		int width;
		int height;
		BMP::read(file_path, image, width, height);															// Load the raw data from the file

		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &this->tex_id);																	// Generate a new texture for the image
		glBindTexture(GL_TEXTURE_2D, this->tex_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, &image[0]);		// Upload the raw data to the texture
	}

	// Gets the image's width (NOT the component width!)
	int Image::get_image_width() {
		GLint width = -1;
		glBindTexture(GL_TEXTURE_2D, this->tex_id);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
		if (width < 0) {
			throw "Could not get image's width!";
		}
		return width;
	}

	// Gets the image's height (NOT the compoent height!)
	int Image::get_image_height() {
		GLint height = -1;
		glBindTexture(GL_TEXTURE_2D, this->tex_id);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
		if (height < 0) {
			throw "Could not get image's height!";
		}
		return height;
	}

}