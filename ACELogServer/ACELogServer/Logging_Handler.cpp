
#include "Common.h"
#include "Logging_Handler.h" 
#include "ace/CDR_Stream.h" 
//#include "../common/Log_Record.h" 
#undef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 760 
int Logging_Handler::recv_log_record(ACE_Message_Block* &mblk)
{
    ACE_INET_Addr peer_addr; 
    logging_peer_.get_remote_addr(peer_addr); 
    mblk = new ACE_Message_Block(MAXHOSTNAMELEN+1); 
    //peer_addr.get_host_name(mblk->wr_ptr(), MAXHOSTNAMELEN); 
    strcpy(mblk->wr_ptr(), "localhost"); 
    mblk->wr_ptr(strlen(mblk->wr_ptr())+1); 

    ACE_Message_Block *payload = new ACE_Message_Block(ACE_DEFAULT_CDR_BUFSIZE); 
    ACE_CDR::mb_align(payload); 

    if(logging_peer_.recv_n(payload->wr_ptr(), 8) == 8)
    {
        payload->wr_ptr(8); 
        ACE_InputCDR cdr(payload); 
        ACE_CDR::Boolean byte_order; 
        cdr >> ACE_InputCDR::to_boolean(byte_order); 
        cdr.reset_byte_order(byte_order); 

        ACE_CDR::ULong length = 0; 
        cdr >> length; 

        payload->size(length + 8 + ACE_CDR::MAX_ALIGNMENT); 

        if(logging_peer_.recv_n(payload->wr_ptr(), length) > 0)
        {
            payload->wr_ptr(length); 
            mblk->cont(payload); 
            return length; 
        }
    }

    payload->release(); 
    mblk->release(); 
    payload = mblk = 0; 
    return -1; 
}