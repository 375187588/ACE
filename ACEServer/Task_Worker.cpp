#include "Task_Worker.h"
#include "ace/OS.h"
#include "ace/Message_Block.h"
#include "Task_Worker.h"

namespace ACE_Server
{
	int Task_Worker::svc(void)
	{
		while (1)
		{
			ACE_Message_Block *mb = NULL;
			if (this->getq(mb) == -1)
			{
				continue;
			}
			process_task(mb);
		}
		return 0;
	}

	void Task_Worker::process_task(ACE_Message_Block *mb)
	{
		//�������ݴ���,���ݵ���ʼ��ַΪmb->rd_ptr(),����Ϊmb->length()
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Processing task: %s length %d\n"), mb->rd_ptr(), mb->length()));
		ACE_OS::sleep(0); //ģ�����ݴ������

		mb->release();
	}
}
