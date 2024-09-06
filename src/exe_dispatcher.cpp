#include "exe_dispatcher.h"

bool exe_dispatcher_c::start()
{
    if (  (not m_console_executor.is_working())
         and ( not m_console_executor.start()) )
    {
        return false;
    }
    
    if (     (not m_file_executor1.is_working() )
         and (not m_file_executor1.start()) )
    {
        return false;
    }
    if ( (not m_file_executor2.is_working())
        and ( not m_file_executor2.start()) )
    {
        return false;
    }
    return true;
}

void exe_dispatcher_c::finish()
{
    m_console_executor.finish();
    m_file_executor1.finish();
    m_file_executor2.finish();
}

void exe_dispatcher_c::receive(const cmd_vector_t &commands)
{
    if ( m_console_executor.is_working() )
    {
        m_console_executor.receive_command(commands);
    }
    if (m_file_executor1.is_working() or
        m_file_executor2.is_working() )
    {
        file_executor_c::receive_command(commands);
    }
}

