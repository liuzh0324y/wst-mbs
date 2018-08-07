#include "httplistener.hh"
#include "httpsession.hh"
#include "httputils.hh"

HTTPListenser::HTTPListenser(boost::asio::io_context& ioc, tcp::endpoint endpoint, std::shared_ptr<std::string const> const& doc_root)
    : acceptor_(ioc),
      socket_(ioc),
      doc_root_(doc_root)
{
    boost::system::error_code ec;

    // Open the acceptor
    acceptor_.open(endpoint.protocol(), ec);
    if (ec)
    {
        fail(ec, "open");
        return;
    }

    // Allow address reuse
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec)
    {
        fail(ec, "set_option");
        return;
    }

    // Bind to the server address
    acceptor_.bind(endpoint, ec);
    if (ec)
    {
        fail(ec, "bind");
        return;
    }

    // Start listen connections
    acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
    if (ec)
    {
        fail(ec, "listen");
        return;
    }
}

void HTTPListenser::Run()
{
    if (!acceptor_.is_open())
    {
        std::cout << "???" << std::endl;
        return;
    }
    DoAccept();
}

void HTTPListenser::DoAccept()
{
    acceptor_.async_accept(socket_, std::bind(&HTTPListenser::OnAccept, shared_from_this(), std::placeholders::_1));
}

void HTTPListenser::OnAccept(boost::system::error_code ec)
{
    if (ec)
    {
        fail(ec, "accept");
    }
    else
    {
        std::cout << "listenser accept." << std::endl;
        std::make_shared<HTTPSession>(std::move(socket_), doc_root_)->Run();
    }

    DoAccept();
}