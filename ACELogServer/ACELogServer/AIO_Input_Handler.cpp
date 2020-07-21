#include "Common.h"
#include "AIO_Input_Handler.h"
#include "AIO_CLD_Acceptor.h" 
#include "AIO_Output_Handler.h" 

AIO_Input_Handler::AIO_Input_Handler(AIO_CLD_Acceptor *acc) 
: acceptor_(acc) 
, mblk_(0) 
{
}

AIO_Input_Handler::~AIO_Input_Handler(void)
{
  reader_.cancel(); 
  ACE_OS::closesocket(handle()); 
  if(mblk_)
    mblk_->release(); 

  mblk_ = 0; 
  acceptor_->remove(this); 
}

void AIO_Input_Handler::open(ACE_HANDLE new_handle, ACE_Message_Block &)
{
  printf("open new input handler.\n"); 
  reader_.open(*this, new_handle, 0, proactor()); 
  ACE_NEW_NORETURN(mblk_, ACE_Message_Block(ACE_DEFAULT_CDR_BUFSIZE)); 
  ACE_CDR::mb_align(mblk_); 
  reader_.read(*mblk_, LOG_HEADER_SIZE); 
}

void AIO_Input_Handler::handle_read_stream(const ACE_Asynch_Read_Stream::Result &result)
{
  //printf("read complete.\n"); 
  if(!result.success() || result.bytes_transferred() == 0)
    delete this; 
  else if(result.bytes_transferred() < result.bytes_to_read())
    reader_.read(*mblk_, result.bytes_to_read() - result.bytes_transferred()); 
  else if(mblk_->length() == LOG_HEADER_SIZE)
  {
    ACE_InputCDR cdr(mblk_); 
    ACE_CDR::Boolean byte_order; 
    cdr >> ACE_InputCDR::to_boolean(byte_order); 
    cdr.reset_byte_order(byte_order); 

    ACE_CDR::ULong length; 
    cdr >> length; 
    
    mblk_->size(length + LOG_HEADER_SIZE); 
    reader_.read(*mblk_, length); 
  }
  else
  {
    if(OUTPUT_HANDLER::instance()->put(mblk_) == -1)
      mblk_->release(); 

    ACE_NEW_NORETURN(mblk_, ACE_Message_Block(ACE_DEFAULT_CDR_BUFSIZE)); 
    ACE_CDR::mb_align(mblk_); 
    reader_.read(*mblk_, LOG_HEADER_SIZE); 
  }
}