#include "Common.h"
#include "Acceptor.h"

Acceptor::Acceptor(void)
{
}

Acceptor::~Acceptor(void)
{
	close();
	outLog("Acceptor::~Acceptor(void)\n");
}

void Acceptor::close()
{
	ACE_Unbounded_Set_Iterator<Input *> it(m_players.begin());
	Input **pIn;
	while (it.next(pIn))
	{
		delete *pIn;
		*pIn = nullptr;
	}
}

int Acceptor::accept(size_t bytes_to_read, const void *act)
{
	return ACE_Asynch_Acceptor<Input>::accept(bytes_to_read, act);
}

Input* Acceptor::make_handler()
{
	outLog("accept new client\n");
	Input *pIn = 0;
	ACE_NEW_RETURN(pIn, Input(this), 0);
	if (m_players.insert(pIn) == -1)
	{
		delete pIn;
		return nullptr;
	}
	return pIn;
}