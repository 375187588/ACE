#define _CRT_SECURE_NO_WARNINGS
#include "Common.h"
#include "Input.h"
#include "Acceptor.h" 
#include "Ouput.h" 
#include "LoginPlayer.h"
Input::Input(Acceptor *acc)
	: m_acceptor(acc)
	, m_pAmb(0)
{
}

Input::~Input(void)
{
	m_reader.cancel();
	ACE_OS::closesocket(handle());
	if (m_pAmb)
		m_pAmb->release();

	m_pAmb = 0;
	m_acceptor->remove(this);
	outLog("Input::~Input(void)\n");
}

void Input::open(ACE_HANDLE new_handle, ACE_Message_Block &)
{
	outLog("open new input handler.\n");
	m_reader.open(*this, new_handle, 0, proactor());
	ACE_NEW_NORETURN(m_pAmb, ACE_Message_Block(ACE_DEFAULT_CDR_BUFSIZE));
	ACE_CDR::mb_align(m_pAmb);
	m_reader.read(*m_pAmb, LOG_HEADER_SIZE);
}

void Input::handle_read_stream(const ACE_Asynch_Read_Stream::Result &result)
{
	//outLog("read complete.\n"); 
	if (!result.success() || result.bytes_transferred() == 0)
	{
		delete this;
	}
	else if (result.bytes_transferred() < result.bytes_to_read())
	{
		m_reader.read(*m_pAmb, result.bytes_to_read() - result.bytes_transferred());
	}
	else if (m_pAmb->length() == LOG_HEADER_SIZE)
	{
		ACE_InputCDR cdr(m_pAmb);
		ACE_CDR::Boolean byte_order;
		cdr >> ACE_InputCDR::to_boolean(byte_order);
		cdr.reset_byte_order(byte_order);

		ACE_CDR::ULong length;
		cdr >> length;

		m_pAmb->size(length + LOG_HEADER_SIZE);
		m_reader.read(*m_pAmb, length);
		
		//////////////////////////////////////		
	}
	else
	{
		if (OUTPUT_HANDLER::instance()->put(m_pAmb) == -1)
		{
			m_pAmb->release();
		}
		
		ACE_NEW_NORETURN(m_pAmb, ACE_Message_Block(ACE_DEFAULT_CDR_BUFSIZE));
		ACE_CDR::mb_align(m_pAmb);
		m_reader.read(*m_pAmb, LOG_HEADER_SIZE);
	}
}