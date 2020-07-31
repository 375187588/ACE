#include "Acceptor.h"

Acceptor::Acceptor(void)
{
	ACE::debug(OPEN_SYS_LOG);
}

Acceptor::~Acceptor(void)
{
	close();
	outLog("Acceptor::~Acceptor(void)\n");
}

int Acceptor::accept(size_t readBytes, const void *act)
{
	outLog("Acceptor::accept:%d\n", readBytes);
	return ACE_Asynch_Acceptor<Input>::accept(readBytes, act);
}

void Acceptor::close()
{
	ACE_Unbounded_Set_Iterator<Input*> it(
		m_players.begin()
	);

	Input **p;
	while (it.next(p))
	{
		delete *p;
	}
}

Input* Acceptor::make_handler()
{
	printf("accept new client\n");
	Input *pIn = 0;
	ACE_NEW_RETURN(pIn, Input(this), 0);
	if (m_players.insert(pIn) == -1)
	{
		delete pIn;
		return 0;
	}
	return pIn;
}