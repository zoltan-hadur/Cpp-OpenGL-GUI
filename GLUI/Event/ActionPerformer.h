#pragma once

#include <list>
#include "ActionListener.h"

namespace GLUI {

	class ActionPerformer {
	private:
		std::list<ActionListener*> action_listeners;
	protected:
		void perform_action(void* sender, ActionEvent& e);
	public:
		ActionPerformer();
		void add_action_listener(void* p);
		void remove_action_listener(void* p);
	};

	void ActionPerformer::perform_action(void* sender, ActionEvent& e) {
		for (auto listener : action_listeners) {
			listener->action_performed(sender, e);
		}
	}

	ActionPerformer::ActionPerformer() {
		action_listeners = std::list<ActionListener*>();
	}

	void ActionPerformer::add_action_listener(void* p) {
		action_listeners.push_back((ActionListener*)p);
	}

	void ActionPerformer::remove_action_listener(void* p) {
		action_listeners.remove((ActionListener*)p);
	}

}