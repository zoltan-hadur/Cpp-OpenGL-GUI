#pragma once

namespace GLUI {

	// Structure to store 2 floating points
	// Used mainly for positions
	struct float2 {
		float x;
		float y;

		float2();
		float2(float x, float y);
		
		float2 operator+(float p);
		float2 operator-(float p);
		float2 operator*(float p);
		float2 operator/(float p);
		float2 operator+(float2 p);
		float2 operator-(float2 p);
		float2 operator*(float2 p);
		float2 operator/(float2 p);
	};

	float2::float2() {
		this->x = 0;
		this->y = 0;
	}

	float2::float2(float x, float y) {
		this->x = x;
		this->y = y;
	}

	float2 float2::operator+(float p) {
		return float2(x + p, y + p);
	}

	float2 float2::operator-(float p) {
		return float2(x - p, y - p);
	}

	float2 float2::operator*(float p) {
		return float2(x * p, y * p);
	}

	float2 float2::operator/(float p) {
		return float2(x / p, y / p);
	}

	float2 float2::operator+(float2 p) {
		return float2(x + p.x, y + p.y);
	}

	float2 float2::operator-(float2 p) {
		return float2(x - p.x, y - p.y);
	}

	float2 float2::operator*(float2 p) {
		return float2(x * p.x, y * p.y);
	}

	float2 float2::operator/(float2 p) {
		return float2(x / p.x, y / p.y);
	}

}