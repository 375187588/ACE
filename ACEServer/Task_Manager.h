#pragma once
#include "ace/Task.h"
#include "ace/Synch.h"

namespace ACE_Server
{
	class Task_Manager : public ACE_Task<ACE_MT_SYNCH>
	{
	public:
		virtual int svc(void);
	};
}