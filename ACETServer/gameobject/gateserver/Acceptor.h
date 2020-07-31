#pragma once
#include "Input.h" 

class Acceptor : public ACE_Asynch_Acceptor<Input>
{
public:
	Acceptor(void);
	virtual ~Acceptor(void);

	void close();
	void remove(Input *pIn) 
	{ 
		m_players.remove(pIn); 
	}
	int accept(size_t bytes_to_read, const void *act);

protected:
	virtual Input* make_handler();

protected:
	//元素是指针,不限制异步输入集合
	ACE_Unbounded_Set<Input*> m_players;
};
