#include "Common.h"
#include "AIO_CLD_Connector.h"

AIO_CLD_Connector::AIO_CLD_Connector(void)
: retry_delay_(INITIAL_RETRY_DELAY)
{
  open(); 
}

AIO_CLD_Connector::~AIO_CLD_Connector(void)
{
}

int AIO_CLD_Connector::validate_connection(
  const ACE_Asynch_Connect::Result &result, 
  const ACE_INET_Addr &remote, 
  const ACE_INET_Addr &local)
{
  printf("validate new connection to server.\n"); 
  remote_addr_ = remote; 
  if(!result.success())
  {
    ACE_Time_Value delay(retry_delay_); 
    retry_delay_ *= 2; 
    if(retry_delay_ > MAX_RETRY_DELAY)
      retry_delay_ = MAX_RETRY_DELAY; 

   // proactor()->schedule_timer(*this, 0, delay); 
    return -1; 
  }

  retry_delay_ = INITIAL_RETRY_DELAY; 
  return 0; 
}