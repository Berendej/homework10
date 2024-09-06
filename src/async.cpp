#include <iostream>

#include "exe_dispatcher.h"
using std::cout;

int connect()
{
    return exe_dispatcher_c::get_instance().start();
}

void receive(const cmd_vector_t &commands)
{
    exe_dispatcher_c::get_instance().receive( commands );
}

void disconnect()
{
    exe_dispatcher_c::get_instance().finish();
}
