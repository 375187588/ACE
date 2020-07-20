
#include "Common.h" 
#include "Logging_Acceptor_WFMO.h" 
#include "Logging_Event_Handler_WFMO.h" 


int Logging_Acceptor_WFMO::handle_input(ACE_HANDLE)
{
  Logging_Event_Handler_WFMO *peer_handler = 0; 
  ACE_NEW_RETURN(peer_handler, 
    Logging_Event_Handler_WFMO(reactor()), -1); 

  if(acceptor().accept(peer_handler->peer()) == -1)
  {
    delete peer_handler; 
    return -1; 
  }
  else if(peer_handler->open() == -1)
  {
    peer_handler->handle_close(); 
    return -1; 
  }

  return 0; 
}

