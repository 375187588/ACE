#pragma once
#include "Common.h"


class Output : public ACE_Task<ACE_NULL_SYNCH>,
	public ACE_Service_Handler
{
public:
	Output();
	virtual ~Output();

	void open(ACE_HANDLE h, ACE_Message_Block&);
	void handle_write_stream(const WriteStream &result);

	virtual int put(ACE_Message_Block* , ACE_Time_Value* = 0);
	virtual void handle_read_stream(const ReadStream &result);
	void startWrite(ACE_Message_Block* pmb = 0);

private:
	//�������� �ͻ������
	ACE_Asynch_Write_Stream m_writeStream;

	//��ȡ���� ��������
	ACE_Asynch_Read_Stream m_readStream;

	//�Ƿ���д
	bool m_bCantWrite;
};

using  MutexOutputHandle = ACE_Unmanaged_Singleton<Output, ACE_Null_Mutex>;