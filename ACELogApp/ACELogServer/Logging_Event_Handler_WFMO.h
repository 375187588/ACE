
#ifndef __LOGGING_EVENT_HANDLER_WFMO_H__
#define __LOGGING_EVENT_HANDLER_WFMO_H__

#include "Logging_Event_Handler_Ex.h" 

class Logging_Event_Handler_WFMO : public Logging_Event_Handler_Ex
{
public:
  Logging_Event_Handler_WFMO(ACE_Reactor *reactor)
    : Logging_Event_Handler_Ex(reactor) { }

  virtual ~Logging_Event_Handler_WFMO() { }
  virtual int handle_input(ACE_HANDLE h)
  {
    ACE_GUARD_RETURN(ACE_SYNCH_MUTEX, monitor, lock_, -1); 
    return Logging_Event_Handler_Ex::handle_input(h); 
  }

private:
  ACE_Thread_Mutex lock_; 
}; 

#endif 