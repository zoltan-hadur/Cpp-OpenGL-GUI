#pragma once

#include "ActionEvent.h"

namespace GLUI {

	class ActionListener {
	protected:
		ActionListener() {}
	public:
		virtual void action_performed(void* sender, ActionEvent& e) = 0;
	};

}