#pragma once
//#include "e:\codes\ace_wrappers\ace\asynch_io.h"

class AIO_CLD_Acceptor; 
class AIO_Input_Handler : public ACE_Service_Handler
{
public:
  AIO_Input_Handler(AIO_CLD_Acceptor *acc = 0);
  virtual ~AIO_Input_Handler(void);

  virtual void open(ACE_HANDLE new_handle, ACE_Message_Block &message_block); 

protected:
  enum { LOG_HEADER_SIZE = 8, }; 
  virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result &result); 

  AIO_CLD_Acceptor *acceptor_; 
  ACE_Message_Block *mblk_; 
  ACE_Asynch_Read_Stream reader_; 
};
