#include "console_executor.h"

bool console_executor_c::open_stream(const std::string &file_name)
{
    // do nothing for console
    return true;
}

void console_executor_c::close_stream()
{
    // also do nothing for console
}

std::ostream* console_executor_c::get_stream()
{
    return &(std::cout);
}

void console_executor_c::receive_command(const cmd_vector_t &commands)
{
    std::unique_lock lck(m_mutex);
    for( auto cmd : commands )
    {
        m_command_queue.push_back(cmd);
    }
    m_cond_var.notify_one();
}

void console_executor_c::work()
{
    cmd_vector_t cmd_vector;
    while( not m_shutdown )
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        while( not m_shutdown and m_command_queue.empty())
        {
               m_cond_var.wait(lck);
        }
        if ( m_command_queue.size() > 0 )
        {
            for ( auto cmd : m_command_queue )
            {
                cmd_vector.push_back(cmd);
            }
            execute(cmd_vector); // not virtual
            cmd_vector.clear();
            m_command_queue.clear();
        }
    }
}

void console_executor_c::finish()
{
    if (m_started )
    {
        {
            std::unique_lock<std::mutex> lck(m_mutex);
            m_shutdown = true;
            m_cond_var.notify_all();
        }
        m_thread.join();
    }
}
