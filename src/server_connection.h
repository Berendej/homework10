#pragma once

#include <list>
#include <string>
#include <memory>

#include "async.h"
#include "cmd.h"

class server_connection_c : public std::enable_shared_from_this<server_connection_c>
{
    static cmd_vector_t s_common_queue;
    static std::mutex s_common_queue_mx;

    cmd_vector_t m_queue;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::streambuf m_buffer;
    std::list<std::string> m_priv_queue; // < keep dynamic commands here
    int m_n;
    int m_depth;
    void process_cmd(int length);
    void handle_read(const boost::system::error_code error,
                     const std::size_t length);
    void up();
    void down();
    void put_in_common_queue(std::string &s);
    void put_in_private_queue(std::string &s);

    void dump_private_queue();
public:
    server_connection_c(boost::asio::ip::tcp::socket sock, int n);
    virtual ~server_connection_c();
    void start_read();
};
using sp_server_connection_c = std::shared_ptr<server_connection_c>;
