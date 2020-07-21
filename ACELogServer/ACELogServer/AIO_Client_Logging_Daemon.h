#pragma once
//#include "ace/Task.h"
//#include "ace/INET_Addr.h" 
#include "AIO_CLD_Acceptor.h" 

class AIO_Client_Logging_Daemon : public ACE_Task<ACE_NULL_SYNCH>
{
public:
  AIO_Client_Logging_Daemon(void);
  virtual ~AIO_Client_Logging_Daemon(void);

  virtual int init(int argc, ACE_TCHAR *argv[]); 
  virtual int fini(); 
  virtual int info(ACE_TCHAR **bufferp, size_t length = 0) const; 
  virtual int svc(); 

protected:
  ACE_INET_Addr cld_addr_; 
  ACE_INET_Addr sld_addr_; 
  AIO_CLD_Acceptor acceptor_; 
};

ACE_FACTORY_DECLARE(ACE_Local_Service, AIO_Client_Logging_Daemon)