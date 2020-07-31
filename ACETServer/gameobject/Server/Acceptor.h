#pragma once
#include "Input.h"

class Acceptor : public ACE_Asynch_Acceptor<Input>
{
public:
	Acceptor(void);
	virtual ~Acceptor(void);

public:
	virtual int accept(size_t , const void *);
	void close();
	void remove(Input* pInput)
	{ 
		m_players.remove(pInput); 
	}
	virtual Input* make_handler();
protected:
	ACE_Unbounded_Set<Input*> m_players;
}; 

