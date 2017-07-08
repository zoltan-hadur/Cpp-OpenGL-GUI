#pragma once

#include "Event.h"

namespace GLUI {

	class EventHandler {
	protected:
		EventHandler() {}
		virtual void handle_event(Event& e) = 0;
	};

}