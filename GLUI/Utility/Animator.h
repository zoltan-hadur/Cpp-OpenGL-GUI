#pragma once

#include "..\Event\ActionPerformer.h"
#include "..\Utility\float2.h"
#include "..\Utility\Stopwatch.h"

namespace GLUI {

	class Animator : public ActionPerformer{
	protected:
		Stopwatch watch;						// For getting elapsed time
		Duration<Time::SECONDS> anim_time;		// The length of the animation
		float2 src;								// Starting position
		float2 dst;								// Target position
		float2 curr;							// Current position, goes from src to dst within time defined by anim_time
		bool overtime;
	public:
		Animator();
		template<typename Time_TYPE> Animator(Duration<Time_TYPE> anim_time);

		// Starts the animation and performs an animator started action
		void start();
		// Stops the animation and performs an animator stopped action
		void stop();
		// Updates the animation and performs an animator reached action when the animator reached it's destination
		void update();

		// Sets the starting position
		void set_src(float2 src);
		// Sets the target position
		void set_dst(float2 dst);
		// Sets the current position
		void set_curr(float2 curr);
		// Sets the animation length
		template<typename Time_TYPE> void set_anim_time(Duration<Time_TYPE> anim_time);
		// Gets the starting position
		float2 get_src();
		// Gets the target position
		float2 get_dst();
		// Gets the current position
		float2 get_curr();
		// Gets the animation length
		Duration<Time::SECONDS> get_anim_time();
	};

	Animator::Animator() {
		this->watch = Stopwatch();
		this->src = float2(0, 0);
		this->dst = float2(0, 0);
		this->curr = float2(0, 0);
		this->anim_time = Duration<Time::SECONDS>(1);
		this->overtime = false;
	}

	template<typename Time_TYPE> Animator::Animator(Duration<Time_TYPE> anim_time) {
		this->watch = Stopwatch();
		this->src = float2(0, 0);
		this->dst = float2(0, 0);
		this->curr = float2(0, 0);
		this->anim_time = anim_time;
		this->overtime = false;
	}

	// Starts the animation and performs an animator started action
	void Animator::start() {
		if (!this->watch.is_running()) {
			this->watch.start();
			ActionEvent e;
			e.animator_started = true;
			this->perform_action(this, e);
		}
	}

	// Stops the animation and performs an animator stopped action
	void Animator::stop() {
		if (this->watch.is_running()) {
			this->watch.stop();
			this->overtime = false;
			ActionEvent e;
			e.animator_stopped = true;
			this->perform_action(this, e);
		}
	}

	// Updates the animation and performs an animator reached action when the animator reached it's destination
	void Animator::update() {
		if (this->watch.is_running()) {
			long double t = std::min(this->watch.get_elapsed<Time::SECONDS>() / this->anim_time.count(), 1.0L);		// (0, 1) normalized time for multiplier

			float multiplier = std::tanh(2 * t) / std::tanh(2);														// Distance function from "acceleration, velocity, distance"
																													// http://www.wolframalpha.com/input/?i=plot+tanh(2*x)%2Ftanh(2)+from+0+to+1

			this->curr = this->src + (this->dst - this->src) * multiplier;											// Calculate the current position

			if (overtime) {																							// Check if the animation has ended
				this->watch.stop();
				this->overtime = false;
				ActionEvent e;
				e.animator_reached = true;
				this->perform_action(this, e);
			} else {
				this->overtime = t >= 1;
			}
		}
	}

	// Sets the starting position
	void Animator::set_src(float2 src) {
		this->src = src;
	}

	// Sets the target position
	void Animator::set_dst(float2 dst) {
		this->dst = dst;
	}

	// Sets the current position
	void Animator::set_curr(float2 curr) {
		this->curr = curr;
	}

	// Sets the animation length
	template<typename Time_TYPE> void Animator::set_anim_time(Duration<Time_TYPE> anim_time) {
		this->anim_time = anim_time;
	}

	// Gets the starting position
	float2 Animator::get_src() {
		return this->src;
	}

	// Gets the target position
	float2 Animator::get_dst() {
		return this->dst;
	}

	// Gets the current position
	float2 Animator::get_curr() {
		return this->curr;
	}

	// Gets the animation length
	Duration<Time::SECONDS> Animator::get_anim_time() {
		return this->anim_time;
	}

}