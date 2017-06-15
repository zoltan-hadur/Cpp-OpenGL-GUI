#pragma once

#include <list>
#include "Event.h"
#include "EventListener.h"

class EventRaiser {
private:
	std::list<EventListener*> event_listeners;
public:
	EventRaiser();
	void add_event_listener(void* p);
	void remove_event_listener(void* p);
	void raise_event(void* sender, Event& e);
};

EventRaiser::EventRaiser() {
	event_listeners = std::list<EventListener*>();
}

void EventRaiser::add_event_listener(void* p) {
	event_listeners.push_back((EventListener*)p);
}

void EventRaiser::remove_event_listener(void* p) {
	event_listeners.remove((EventListener*)p);
}

void EventRaiser::raise_event(void* sender, Event& e) {
	for (auto el : event_listeners) {
		el->action_performed(sender, e);
	}
}