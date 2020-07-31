#include "Common.h"
#include "UserHandler.h" 
#include "ace/CDR_Stream.h" 
#include "Log_Record.h" 
#undef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 760 

int UserHandler::recv_log_record(ACE_Message_Block* &mblk)
{
	ACE_INET_Addr peer_addr;
	logging_peer_.get_remote_addr(peer_addr);
	mblk = new ACE_Message_Block(MAXHOSTNAMELEN + 1);
	//peer_addr.get_host_name(mblk->wr_ptr(), MAXHOSTNAMELEN); 
	strcpy(mblk->wr_ptr(), "localhost");
	mblk->wr_ptr(strlen(mblk->wr_ptr()) + 1);

	ACE_Message_Block *payload = new ACE_Message_Block(ACE_DEFAULT_CDR_BUFSIZE);
	ACE_CDR::mb_align(payload);

	if (logging_peer_.recv_n(payload->wr_ptr(), 8) == 8)
	{
		payload->wr_ptr(8);
		ACE_InputCDR cdr(payload);
		ACE_CDR::Boolean byte_order;
		cdr >> ACE_InputCDR::to_boolean(byte_order);
		cdr.reset_byte_order(byte_order);

		ACE_CDR::ULong length = 0;
		cdr >> length;

		payload->size(length + 8 + ACE_CDR::MAX_ALIGNMENT);

		if (logging_peer_.recv_n(payload->wr_ptr(), length) > 0)
		{
			payload->wr_ptr(length);
			mblk->cont(payload);
			return length;
		}
	}

	payload->release();
	mblk->release();
	payload = mblk = 0;
	std::cout << "UserHandler::recv_log_record release()" << std::endl;
	return -1;
}

int UserHandler::write_log_record(ACE_Message_Block *mblk)
{

	//if(ACE::debug())
	//{
	ACE_InputCDR cdr(mblk->cont());
	ACE_CDR::Boolean byte_order;
	ACE_CDR::ULong length;
	cdr >> ACE_InputCDR::to_boolean(byte_order);
	cdr.reset_byte_order(byte_order);
	cdr >> length;
	Logging_Record log_record;
	cdr >> log_record;
	strcat(const_cast<char*>(log_record.msg_data()), "\n");
	log_record.msg_data_len(log_record.msg_data_len() + 1);
	//log_record.print(mblk->rd_ptr(), 1, std::cerr); 
	log_record.dump(); 
	ACE_DEBUG((LM_DEBUG, "recv(%d): %s", logging_peer_.get_handle(), log_record.msg_data()));
	//}

	if (log_file_.send_n(log_record.msg_data(), log_record.msg_data_len()) == -1)
		return -1;

	return mblk->total_length();
}

int UserHandler::log_record()
{
	ACE_Message_Block *mblk = 0;
	if (recv_log_record(mblk) == -1)
		return -1;
	else {
		int result = write_log_record(mblk);
		mblk->release();
		return result == -1 ? -1 : 0;
	}
}