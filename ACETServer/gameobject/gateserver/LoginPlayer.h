
#pragma once
#include "Common.h"

class LoginPlayer
{
public:
	LoginPlayer()
		: m_lType(0),
		m_lPid(0),
		m_nLen(0),
		m_szMsg()
	{
	}

	LoginPlayer(long t, long p, const ACE_Time_Value &tv) :
		m_lType(t),
		m_lPid(p),
		m_timeStamp(tv), 
		m_nLen(0), 
		m_szMsg()
	{
	}

	void dump()
	{
		outLog("type = %d\n", type());
		outLog("pid = %d\n", pid());
		outLog("timestamp = %d.%d\n", time_stamp().sec(), time_stamp().usec());
		outLog("msg data len = %d\n", msg_data_len());
		outLog("msg = %s\n", msg_data());
	}

	long type() const { return m_lType; }
	void type(long t) { m_lType = t; }
	long pid() const { return m_lPid; }
	void pid(long p) { m_lPid = p; }
	const ACE_Time_Value& time_stamp() const { return m_timeStamp; }
	void time_stamp(ACE_Time_Value& t) { m_timeStamp = t; }
	size_t msg_data_len() const { return m_nLen; }
	void msg_data_len(size_t l) { m_nLen = l; }
	const ACE_TCHAR* msg_data() const { return m_szMsg; }
	void msg_data(const ACE_TCHAR *data) 
	{
		strcpy_s(m_szMsg,4096, data); m_nLen = strlen(m_szMsg) + 1;
	}

private:
	long m_lType;
	long m_lPid;
	ACE_Time_Value m_timeStamp;
	size_t m_nLen;
	ACE_TCHAR m_szMsg[MAX_BUF_SIZE];
};

int operator<< (ACE_OutputCDR &cdr,const LoginPlayer &log_record)
{
	size_t msglen = log_record.msg_data_len();
	cdr << ACE_CDR::Long(log_record.type());
	cdr << ACE_CDR::Long(log_record.pid());
	cdr << ACE_CDR::Long(log_record.time_stamp().sec());
	cdr << ACE_CDR::Long(log_record.time_stamp().usec());
	cdr << ACE_CDR::ULong(msglen);

	cdr.write_char_array(log_record.msg_data(), msglen);
	return cdr.good_bit();
}

int operator>> (ACE_InputCDR &cdr,
	LoginPlayer &log_record)
{
	ACE_CDR::Long type = 0;
	ACE_CDR::Long pid = 0;
	ACE_CDR::Long sec = 0;
	ACE_CDR::Long usec = 0;
	ACE_CDR::ULong msglen = 0;
	cdr >> type;
	cdr >> pid;
	cdr >> sec;
	cdr >> usec;
	cdr >> msglen;
	if (cdr.good_bit())
	{
		ACE_TCHAR log_msg[MAX_BUF_SIZE + 1] = { 0 };
		log_record.type(type);
		log_record.pid(pid);
		ACE_Time_Value t = ACE_Time_Value(sec, usec);
		log_record.time_stamp(t);
		cdr.read_char_array(log_msg, msglen);
		log_msg[msglen] = 0;
		log_record.msg_data(log_msg);
	}

	return cdr.good_bit();
}
