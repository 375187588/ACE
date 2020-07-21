#pragma once
//#include "e:\codes\ace_wrappers\ace\asynch_connector.h"

#include "AIO_Output_Handler.h" 

class AIO_CLD_Connector : public ACE_Asynch_Connector<AIO_Output_Handler>
{
public:
  AIO_CLD_Connector(void);
  virtual ~AIO_CLD_Connector(void);
  enum { INITIAL_RETRY_DELAY = 3, MAX_RETRY_DELAY = 60, }; 

  virtual int validate_connection(
    const ACE_Asynch_Connect::Result &result, 
    const ACE_INET_Addr &remote, 
    const ACE_INET_Addr &local); 

  virtual void handle_time_out(const ACE_Time_Value &, const void *) { connect(remote_addr_); } 
  int reconnect() { return connect(remote_addr_); } 
  
protected:
  virtual AIO_Output_Handler* make_handler() 
  { return OUTPUT_HANDLER::instance(); } 

protected:
  ACE_INET_Addr remote_addr_; 
  int retry_delay_; 
};

typedef ACE_Unmanaged_Singleton<AIO_CLD_Connector, ACE_Null_Mutex> CLD_CONNECTOR; 