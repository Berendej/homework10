#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include "server_connection.h"
#include "async.h"

cmd_vector_t server_connection_c::s_common_queue;
std::mutex server_connection_c::s_common_queue_mx;

server_connection_c::server_connection_c(boost::asio::ip::tcp::socket sock, int n) : 
    m_socket{std::move(sock)}, m_n(n), m_depth(0)
{
#ifdef VERBOSE
    std::cout << "server_connection_c " << m_n << std::endl;
#endif
}

void server_connection_c::start_read()
{
#ifdef VERBOSE
    std::cout << "start read " << m_socket.is_open() << std::endl;
#endif
    boost::asio::async_read_until( m_socket, m_buffer, '\n',
        boost::bind(&server_connection_c::handle_read, 
                     shared_from_this(),
                     boost::asio::placeholders::error,
                     boost::asio::placeholders::bytes_transferred));
#ifdef VERBOSE
    std::cout << "start read p2" << std::endl;
#endif
}

void server_connection_c::handle_read(const boost::system::error_code error,
                  const std::size_t length) 
{
#ifdef VERBOSE
    std::cout << "handle read " << m_socket.is_open() << std::endl;
#endif
    if (error or 0 == length ) 
    {
#ifdef VERBOSE
        std::cout << "handle read error " << error << " << len " << length << std::endl;
#endif
        // zero length also means connection is closed
        // connection is closed, we do not continue 
        // async calling so that smart pointer will do its work
        // and connection_c will be deleted
        return;
    }
#ifdef VERBOSE
    std::cout << "handle read ok" << std::endl;
#endif
    process_cmd(length);
    m_buffer.consume(length);
    start_read();
}

void server_connection_c::down()
{
    // Размер блока можно изменить динамически, 
    // если перед началом блока и сразу после дать команды `{`
    // Предыдущий пакет при этом принудительно завершается. !!!
    std::unique_lock lck(s_common_queue_mx);
    if ( 0 == m_depth && s_common_queue.size() > 0 )
    {
        // Предыдущий пакет при этом принудительно завершается.
        cmd_vector_t v;
        for( auto s : s_common_queue )
        {
            v.push_back(s);
        }
        s_common_queue.clear();
        receive(v);
    }

    m_depth--;
}

void server_connection_c::up()
{
    m_depth++;
    if ( 0 == m_depth )
    {
        dump_private_queue();
    }
}

void server_connection_c::process_cmd(int length)
{
    std::string s;
    std::istream(&m_buffer) >> s; 
#ifdef VERBOSE
    std::cout << "got command " << s << std::endl;
#endif
    if ( 0 == s.compare("{"))
    {
        down();
        return;
    }
    if ( 0 == s.compare("}"))
    {
        up();
        return;
    }
    if ( 0 == m_depth )
    {
        put_in_common_queue(s);
        return;
    }
    // put in our own queue
    put_in_private_queue(s);
}

void server_connection_c::put_in_private_queue(std::string &s)
{
    m_queue.push_back(s);
}

void server_connection_c::put_in_common_queue(std::string &s)
{
    cmd_vector_t v;
    bool need_to_receive = false;
    { // <-  to release lck before calling receive()
        std::unique_lock lck(s_common_queue_mx);
        s_common_queue.push_back(s);
        if ( s_common_queue.size() >= m_n )
        {
            need_to_receive = true;
            for( auto s : s_common_queue )
            {
                v.push_back(s);
            }
            s_common_queue.clear();
        }
    }
    if ( need_to_receive )
    {
        receive(v);
    }
}

void server_connection_c::dump_private_queue()
{
#ifdef VERBOSE
    std::cout << "dump_private_queue " << m_queue.size() << std::endl;
#endif
    cmd_vector_t v;
    for( auto s : m_queue )
    {
        v.push_back(s);
    }
    m_queue.clear();
    receive(v);
}

server_connection_c::~server_connection_c()
{
#ifdef THIS_IS_WRONG
    /* Если данные закончились внутри динамического блока, весь динамический блок игнорируется.*/
    if ( m_queue.size() > 0 )
    {
        dump_private_queue();
    }
#endif
#ifdef VERBOSE
    std::cout << "~server_connection_c " << m_socket.is_open() << std::endl;
#endif
}
