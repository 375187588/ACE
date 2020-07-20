
#ifndef __LOGGING_EVENT_HANDLER_H__
#define __LOGGING_EVENT_HANDLER_H__

#include "ace/FILE_IO.h" 
#include "ace/FILE_Connector.h" 
#include "ace/Event_Handler.h" 
#include "ace/Reactor.h" 
#include "ace/OS_NS_String.h" 
#include "Logging_Handler.h" 

class ACE_Message_Block; 
class Logging_Event_Handler : public ACE_Event_Handler 
{
public:
  Logging_Event_Handler(ACE_Reactor *reactor)
    : ACE_Event_Handler(reactor), 
    logging_handler_(log_file_) { }

  virtual ~Logging_Event_Handler() { }
  virtual int open(); 
  virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE); 
  virtual int handle_close(ACE_HANDLE = ACE_INVALID_HANDLE, ACE_Reactor_Mask = 0); 
  virtual ACE_HANDLE get_handle() const; 
  ACE_SOCK_Stream& peer() { return logging_handler_.peer(); } 
  ACE_FILE_IO& log_file() { return log_file_; } 

private:
  ACE_FILE_IO log_file_; 
  Logging_Handler logging_handler_; 
}; 

#endif 