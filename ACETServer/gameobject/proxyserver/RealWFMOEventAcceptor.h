#pragma once
#include "RealEventAcceptor.h" 

class RealWFMOEventAcceptor : public RealEventAcceptor
{
public:
	//typedef ACE_INET_Addr PEER_ADDR; 
	RealWFMOEventAcceptor(ACE_Reactor *r = ACE_Reactor::instance()) : RealEventAcceptor(r) { }
	virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);

protected:
	virtual ~RealWFMOEventAcceptor() {}
};
