#include "Common.h"
#include "AIO_CLD_Acceptor.h"

AIO_CLD_Acceptor::AIO_CLD_Acceptor(void)
{
}

AIO_CLD_Acceptor::~AIO_CLD_Acceptor(void)
{
}

void AIO_CLD_Acceptor::close()
{
  ACE_Unbounded_Set_Iterator<AIO_Input_Handler *> iter(clients_.begin()); 
  AIO_Input_Handler **ih; 
  while(iter.next(ih)) 
    delete *ih; 
}

AIO_Input_Handler* AIO_CLD_Acceptor::make_handler()
{
  printf("accept new client\n"); 
  AIO_Input_Handler *ih = 0; 
  ACE_NEW_RETURN(ih, AIO_Input_Handler(this), 0); 
  //ih = new (::std::nothrow)AIO_Input_Handler(this);
  //if (nullptr != ih)
  //{
	 // if (clients_.insert(ih) == -1)
	 // {
		//  delete ih;
		//  return 0;
	 // }
  //}
  if (clients_.insert(ih) == -1)
  {
	  delete ih;
	  return 0;
  }
  return ih; 
}