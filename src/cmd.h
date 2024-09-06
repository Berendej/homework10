#pragma once 

#include <string>
#include <vector>
#include <memory>
#include <list>

using cmd_list_t = std::list<std::string>;
using i_cmd_list_t = cmd_list_t::iterator;

using cmd_vector_t = std::vector<std::string>;
using i_cmd_vector_t = cmd_vector_t::iterator;
