#include "Input.h"
#include "Acceptor.h"

Input::Input(Acceptor* pAcceptor):
	m_pAcceptor(pAcceptor),
	m_pMsgBlock(nullptr)
{
	ACE::debug(OPEN_SYS_LOG);
}

Input::~Input()
{
	m_readStream.cancel();
	ACE_OS::closesocket(handle());
	if (nullptr != m_pMsgBlock)
	{
		m_pMsgBlock->release();
	}
	m_pMsgBlock = nullptr;
	if (m_pAcceptor)
	{
		m_pAcceptor->remove(this);
	}	
	outLog("Input::~Input()\n");
}

void Input::open(ACE_HANDLE h,ACE_Message_Block &b)
{
	m_readStream.open(*this, h, 0, ACE_Proactor::instance());
	m_pMsgBlock = new (::std::nothrow) ACE_Message_Block(ACE_DEFAULT_CDR_BUFSIZE);
	if (nullptr != m_pMsgBlock)
	{
		ACE_CDR::mb_align(m_pMsgBlock);
		m_readStream.read(*m_pMsgBlock, CMD_HEADER_SIZE);
	}	
}

void Input::handle_read_stream(const ReadStream &result)
{
	if (!result.success() ||
		result.bytes_transferred() == 0)
	{
		delete this;
	}
	else if (result.bytes_transferred() < result.bytes_to_read())
	{
		size_t nBytes = result.bytes_to_read() -
			result.bytes_transferred();

		if (m_readStream.read(*m_pMsgBlock, nBytes) < 0)
		{
			delete this;
		}
	}
	else if (m_pMsgBlock->length() == CMD_HEADER_SIZE)
	{
		ACE_InputCDR cdr(m_pMsgBlock);
		ACE_CDR::Boolean byte_order;
		cdr >> ACE_InputCDR::to_boolean(byte_order);
		cdr.reset_byte_order(byte_order);

		ACE_CDR::ULong length;
		cdr >> length;

		m_pMsgBlock->size(length + CMD_HEADER_SIZE);
		m_readStream.read(*m_pMsgBlock, length);
	}
	else
	{
		if (MutexOutputHandle::instance()->put(m_pMsgBlock) == -1)
		{
			m_pMsgBlock->release();
		}

		m_pMsgBlock = new (::std::nothrow) ACE_Message_Block(ACE_DEFAULT_CDR_BUFSIZE);
		if (nullptr != m_pMsgBlock)
		{
			ACE_CDR::mb_align(m_pMsgBlock);
			m_readStream.read(*m_pMsgBlock, CMD_HEADER_SIZE);
		}
	}
}
