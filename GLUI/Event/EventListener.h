#pragma once

#include "Event.h"

class EventListener {
public:
	EventListener() {};
	virtual void action_performed(void* sender, Event& e) = 0;
};