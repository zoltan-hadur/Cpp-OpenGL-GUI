#pragma once

#include <algorithm>

namespace GLUI {

	// Structure to store colors in RGBA form
	struct Color {
		float R;
		float G;
		float B;
		float A;

		Color(float R = 0, float G = 0, float B = 0, float A = 1);

		Color operator+(float f);
		Color operator-(float f);
		Color operator*(float f);
		Color operator/(float f);
		Color operator+(Color c);
		Color operator-(Color c);
		Color operator*(Color c);
		Color operator/(Color c);
	};

	// RGBA in range 0-1
	Color::Color(float R, float G, float B, float A) {
		this->R = R;
		this->G = G;
		this->B = B;
		this->A = A;
	}

	Color Color::operator+(float f) {
		return Color(this->R + f,
					 this->G + f,
					 this->B + f,
					 this->A + f);
	}

	Color Color::operator-(float f) {
		return Color(this->R - f,
					 this->G - f,
					 this->B - f,
					 this->A - f);
	}

	Color Color::operator*(float f) {
		return Color(this->R * f,
					 this->G * f,
					 this->B * f,
					 this->A * f);
	}

	Color Color::operator/(float f) {
		return Color(this->R / f,
					 this->G / f,
					 this->B / f,
					 this->A / f);
	}

	Color Color::operator+(Color c) {
		return Color(this->R + c.R,
					 this->G + c.G,
					 this->B + c.B,
					 this->A + c.A);
	}

	Color Color::operator-(Color c) {
		return Color(this->R - c.R,
					 this->G - c.G,
					 this->B - c.B,
					 this->A - c.A);
	}

	Color Color::operator*(Color c) {
		return Color(this->R * c.R,
					 this->G * c.G,
					 this->B * c.B,
					 this->A * c.A);
	}

	Color Color::operator/(Color c) {
		return Color(this->R / c.R,
					 this->G / c.G,
					 this->B / c.B,
					 this->A / c.A);
	}

}