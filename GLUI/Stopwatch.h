#pragma once

#include <GL\freeglut.h>

class Stopwatch {
private:
	float start_time;
	float last_time;
	float TTL;
	bool running;
public:
	Stopwatch();
	void start(float time_to_live = 0);
	void stop();
	bool try_stop();
	void reset();
	bool is_running();
	float get_delta_time();
	float get_elapsed_time();
};

Stopwatch::Stopwatch() {
	start_time = 0;
	last_time = 0;
	TTL = 0;
	running = false;
}

void Stopwatch::start(float time_to_live) {
	if (!running) {
		start_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
		last_time = start_time;
		TTL = time_to_live;
		running = true;
	}
}

void Stopwatch::stop() {
	if (running) {
		last_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
		running = false;
	}
}

bool Stopwatch::try_stop() {
	if (this->get_elapsed_time() > TTL) {
		this->stop();
		return true;
	} else {
		return false;
	}
}

void Stopwatch::reset() {
	this->stop();
	this->start();
}

bool Stopwatch::is_running() {
	return running;
}

float Stopwatch::get_delta_time() {
	if (running) {
		float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
		float dt = time - last_time;
		last_time = time;
		return dt;
	} else {
		throw "Stopwatch is not running!";
	}
}

float Stopwatch::get_elapsed_time() {
	if (running) {
		return glutGet(GLUT_ELAPSED_TIME) / 1000.0f - start_time;
	} else {
		return last_time - start_time;
	}
}