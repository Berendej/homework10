#include <chrono>
#include <ostream>
#include <thread>
#include <iostream>

#include "executor.h"

int executor_c::s_instance_id = 0;
std::mutex executor_c::s_instatinate_mutex;

executor_c::executor_c() : m_shutdown(false), 
                           m_started(false)
{
    std::unique_lock lck(s_instatinate_mutex);
    m_instance_id = ++s_instance_id;
}

std::string get_current_timestamp()
{
    using std::chrono::system_clock;
    auto currentTime = std::chrono::system_clock::now();
    char buffer[80];
    auto transformed = currentTime.time_since_epoch().count() / 1000000;
    auto millis = transformed % 1000;
    std::time_t tt;
    tt = system_clock::to_time_t ( currentTime );
    auto timeinfo = localtime (&tt);
    strftime (buffer,80,"%F %H:%M:%S",timeinfo);
    sprintf(buffer, "%s:%03d",buffer,(int)millis);
    return std::string(buffer);
}

std::string executor_c::compose_log_name()
{
    std::string result = file_prefix();
    result += get_current_timestamp();
    return result;
}

void executor_c::execute(cmd_vector_t &commands)
{
    std::string file_name(compose_log_name());
    open_stream(file_name);
    std::ostream *p_stream = get_stream();
    (*p_stream) << "bulk: ";
    // to determine last iteration use hand made cycle
    i_cmd_vector_t it = commands.begin();
    while ( it != commands.end() )
    {
        *p_stream <<  (*it);
        it++;
        if (it  != commands.end() )
        {
            *p_stream << ", ";
        }
    }
    *p_stream << std::endl;
    close_stream(); 
}

int executor_c::start()
{
    m_thread = std::move( std::thread(&executor_c::work, this ) );
    m_started = true;
    return true;
}
