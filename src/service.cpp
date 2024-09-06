#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <memory>

#include "async.h"
#include "server_connection.h"

using tcp = boost::asio::ip::tcp;

void start_accept(tcp::acceptor& acceptor, int &n)
{
    acceptor.async_accept(
        [&acceptor, &n](const boost::system::error_code error, tcp::socket socket)
        {
            sp_server_connection_c sp_conn( new server_connection_c(std::move(socket), n));
            // start_read() will put shared_from_this in io_service's slot so it will be alive
            // even when sp_conn will go out of scope
            sp_conn->start_read(); // it'll die itself when there will be no work for it.
            start_accept(acceptor, n);
        });
}

bool run_server(int n, int port)
{
    boost::asio::io_service io_service;
    if ( not connect() )// calling async lib
    {
        return false;
    }
    tcp::acceptor acceptor{io_service, tcp::endpoint(tcp::v4(), port)};
    start_accept(acceptor, n);
    io_service.run();
    disconnect(); // <- from async lib 
#ifdef VERBOSE
    std::cout << "run end" << std::endl;
#endif
    return true;
}
