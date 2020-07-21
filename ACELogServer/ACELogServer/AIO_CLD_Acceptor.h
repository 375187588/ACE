#pragma once

#include "AIO_Input_Handler.h" 

class AIO_CLD_Acceptor :  public ACE_Asynch_Acceptor<AIO_Input_Handler>
{
public:
  AIO_CLD_Acceptor(void);
  virtual ~AIO_CLD_Acceptor(void);

  void close(); 
  void remove(AIO_Input_Handler *ih) { clients_.remove(ih); } 

protected:
  virtual AIO_Input_Handler* make_handler(); 

protected:
  ACE_Unbounded_Set<AIO_Input_Handler *> clients_; 
};
