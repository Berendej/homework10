#ifndef _CONSOLE_EXECUTOR_H_
#define _CONSOLE_EXECUTOR_H_

#include <condition_variable>
#include <mutex>
#include <iostream>

#include "executor.h"

class console_executor_c : public executor_c
{
    cmd_vector_t              m_command_queue;
    std::mutex              m_mutex;
    std::condition_variable m_cond_var;

public:
    virtual bool open_stream(const std::string &file_name);
    virtual void close_stream();
    virtual std::ostream* get_stream();
    virtual void work();
    virtual void finish();

    void receive_command(const cmd_vector_t &commands);
};


#endif