
#pragma once


#include "ace/Log_Msg.h" 
#include "ace/Time_Value.h"

class Logging_Record
{
public:
    enum { 
        MAXLOGMSGLEN = 4096, 
    }; 

    Logging_Record()
        : type_(0), 
        pid_(0), 
        len_(0), 
        log_msg_()
    {
    }

    Logging_Record(long t, long p, const ACE_Time_Value &tv)
        : type_(t), pid_(p), time_stamp_(tv), len_(0), log_msg_()
    {
    }

    void dump()
    {
        ACE_DEBUG((LM_DEBUG, "type = %d\n", type())); 
        ACE_DEBUG((LM_DEBUG, "pid = %d\n", pid())); 
        ACE_DEBUG((LM_DEBUG, "timestamp = %d.%d\n", time_stamp().sec(), time_stamp().usec())); 
        ACE_DEBUG((LM_DEBUG, "msg data len = %d\n", msg_data_len())); 
        ACE_DEBUG((LM_DEBUG, "msg = %s\n", msg_data())); 
    }

    long type() const { return type_; }
    void type(long t) { type_ = t; } 
    long pid() const { return pid_; } 
    void pid(long p) { pid_ = p; } 
    const ACE_Time_Value& time_stamp() const { return time_stamp_; } 
    void time_stamp(ACE_Time_Value& t) { time_stamp_ = t; } 
    size_t msg_data_len() const { return len_; } 
    void msg_data_len(size_t l) { len_ = l; } 
    const ACE_TCHAR* msg_data() const { return log_msg_; } 
    void msg_data(const ACE_TCHAR *data) { strcpy(log_msg_,data); len_ = strlen(log_msg_)+1; } 

private:
    long type_; 
    long pid_; 
    ACE_Time_Value time_stamp_; 
    size_t len_; 
    ACE_TCHAR log_msg_[MAXLOGMSGLEN]; 
}; 

int operator<< (ACE_OutputCDR &cdr, 
                const Logging_Record &log_record)
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
                Logging_Record &log_record)
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
    if(cdr.good_bit())
    {
        ACE_TCHAR log_msg[Logging_Record::MAXLOGMSGLEN + 1] = { 0 }; 
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
