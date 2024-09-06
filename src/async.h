#pragma once
#include "cmd.h"

int connect();
void receive(const cmd_vector_t &commands);
void disconnect();

