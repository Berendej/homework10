#pragma once

#include <condition_variable>

#include "executor.h"

class file_executor_c : public executor_c
{
    static cmd_vector_t s_command_queue;
    static std::mutex   s_mutex;
    static std::condition_variable s_cond_var;

    std::ofstream *m_p_stream;
public:
    file_executor_c();

    virtual std::ostream* get_stream();
    virtual bool open_stream(const std::string &file_name);
    virtual void close_stream();
    virtual std::string file_prefix();
    virtual void work();
    virtual void finish();

    static void receive_command(const cmd_vector_t &commands);

};
