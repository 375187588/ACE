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
	//Ԫ����ָ��,�������첽���뼯��
	ACE_Unbounded_Set<Input*> m_players;
};
