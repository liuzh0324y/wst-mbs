#ifndef HTTP_LISTENER_HH
#define HTTP_LISTENER_HH

#include "common.hh"

class HTTPListenser : public std::enable_shared_from_this<HTTPListenser>
{
public:
    HTTPListenser(boost::asio::io_context& ioc, tcp::endpoint endpoint);

    void Run();

    void DoAccept();

    void OnAccept(boost::system::error_code ec);

private:
    tcp::acceptor acceptor_;
    tcp::socket socket_;
};

#endif // HTTP_LISTENER_HH