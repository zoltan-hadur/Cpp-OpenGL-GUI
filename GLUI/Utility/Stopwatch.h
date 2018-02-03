#pragma once

//#include <GL\freeglut.h>
#include "Time.h"

namespace GLUI {

	class Stopwatch {
	protected:
		TimePoint start_time;			// Timepoint when the stopwatch was started
		TimePoint last_time;			// The last timepoint
		Duration<Time::SECONDS> TTL;	// Time to live parameter

		bool running;			// Indicates that the stopwatch is running or not

		bool TTL_exceeded();
	public:
		Stopwatch();

		// Starts the stopwatch
		void start();
		// Starts the stopwatch with the given TTL
		template<typename Time_TYPE> void start(Duration<Time_TYPE> TTL);
		// Stops the stopwatch
		void stop();
		// Stops the stopwatch if it has exceeded it's TTL
		bool try_stop();
		// Resets the stopwatch
		void reset();
		// Returns true if the stopwatch is running, else false
		bool is_running();
		// Return the delta time
		template<typename T> long double get_delta();
		// Return the elapsed time since the stopwatch started
		template<typename T> long double get_elapsed();
	};

	bool Stopwatch::TTL_exceeded() {
		return this->get_elapsed<Time::SECONDS>() > this->TTL.count();
	}

	Stopwatch::Stopwatch() {
		this->start_time = Time::now();
		this->last_time = this->start_time;
		this->TTL = Duration<Time::HOURS>(24);

		this->running = false;
	}

	// Starts the stopwatch
	void Stopwatch::start() {
		this->start(Duration<Time::HOURS>(24));
	}

	// Starts the stopwatch with the given TTL
	template<typename Time_TYPE> void Stopwatch::start(Duration<Time_TYPE> TTL) {
		if (!running) {
			this->start_time = Time::now();
			this->last_time = this->start_time;
			this->TTL = TTL;
			this->running = true;
		}
	}

	// Stops the stopwatch
	void Stopwatch::stop() {
		if (running) {
			this->last_time = Time::now();
			this->running = false;
		}
	}

	// Stops the stopwatch if it has exceeded it's TTL
	bool Stopwatch::try_stop() {
		if (this->TTL_exceeded()) {
			this->stop();
			return true;
		}
		else {
			return false;
		}
	}

	// Resets the stopwatch
	void Stopwatch::reset() {
		this->stop();
		this->start(this->TTL);
	}

	// Returns true if the stopwatch is running, else false
	bool Stopwatch::is_running() {
		this->try_stop();
		return this->running;
	}

	// Return the delta time
	template<typename Time_TYPE> long double Stopwatch::get_delta() {
		if (running) {
			TimePoint now = Time::now();
			Duration<Time_TYPE> dt = now - this->last_time;
			this->last_time = now;
			return dt.count();
		}
		else {
			throw "Stopwatch is not running!";
		}
	}

	// Return the elapsed time since the stopwatch started
	template<typename Time_TYPE> long double Stopwatch::get_elapsed() {
		if (running) {
			return Duration<Time_TYPE>(Time::now() - this->start_time).count();
		}
		else {
			return Duration<Time_TYPE>(last_time - start_time).count();
		}
	}

}