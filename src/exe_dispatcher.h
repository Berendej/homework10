#ifndef _EXE_DISPATCHER_H_
#define _EXE_DISPATCHER_H_

#include "console_executor.h"
#include "file_executor.h"

class exe_dispatcher_c
{
    console_executor_c m_console_executor;
    file_executor_c m_file_executor1;
    file_executor_c m_file_executor2;
    exe_dispatcher_c()
    {
    }

public:
    static exe_dispatcher_c& get_instance()
    {
        static exe_dispatcher_c instance;
        return instance;
    }

    bool start();
    void receive(const cmd_vector_t &commands);
    void finish();
};

#endif