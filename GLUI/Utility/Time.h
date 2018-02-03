#pragma once

#include <chrono>
#include <thread>

namespace GLUI {

	typedef std::chrono::time_point<std::chrono::steady_clock> TimePoint;
	template <typename Time_TYPE> using Duration = std::chrono::duration<long double, Time_TYPE>;

	namespace Time {

		typedef std::nano NANO_SECONDS;
		typedef std::micro MICRO_SECONDS;
		typedef std::milli MILLI_SECONDS;
		typedef std::ratio<1i64> SECONDS;
		typedef std::ratio<60i64> MINUTES;
		typedef std::ratio<3600i64> HOURS;

		std::chrono::time_point<std::chrono::steady_clock> now() {
			return std::chrono::steady_clock::now();
		}

		template <typename Time_TYPE> void wait(Duration<Time_TYPE> duration) {
			std::this_thread::sleep_for(duration);
		}

	}

}