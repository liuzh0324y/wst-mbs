#include "httpsession.hh"
#include "httputils.hh"
#include <iostream>

// template<class Stream>
// struct send_lambda
// {
//     Stream& stream_;
//     bool& close_;
//     boost::system::error_code& ec_;

//     explicit
//     send_lambda(
//         Stream& stream,
//         bool& close,
//         boost::system::error_code& ec)
//         : stream_(stream)
//         , close_(close)
//         , ec_(ec)
//     {
//     }

//     template<bool isRequest, class Body, class Fields>
//     void operator()(boost::beast::http::message<isRequest, Body, Fields>&& msg) const
//     {
//         // Determine if we should close the connection after
//         close_ = msg.need_eof();

//         // We need the serializer here because the serializer requires
//         // a non-const file_body, and the message oriented version of
//         // http::write only works with const messages.
//         boost::beast::http::serializer<isRequest, Body, Fields> sr{msg};
//         boost::beast::http::write(stream_, sr, ec_);
//     }
// };

template<class Body, class Allocator, class Send>
void handleRequest(boost::beast::string_view doc_root, http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send)
{
    // Returns a bad request response
    auto const bad_request =
    [&req](boost::beast::string_view why)
    {
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.set(http::field::connection, "close");
        res.body() = why.to_string();
        res.prepare_payload();
        return res;
    };

    // Returns a not found response
    auto const not_found =
    [&req](boost::beast::string_view target)
    {
        http::response<http::string_body> res{http::status::not_found, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.set(http::field::connection, "close");
        res.body() = target.to_string();
        res.prepare_payload();
        return res;
    };

    return send(not_found(req.target()));
}

HTTPSession::HTTPSession(tcp::socket socket, std::shared_ptr<std::string const> const& doc_root)
    : socket_(std::move(socket)),
      strand_(socket_.get_executor()),
      doc_root_(doc_root),
      lambda_(*this)
{

}

HTTPSession::~HTTPSession()
{

}

void HTTPSession::Run()
{
    DoRead();
}

void HTTPSession::DoRead()
{
    req_ = {};

    // Read a request
    http::async_read(socket_, buffer_, req_,
        boost::asio::bind_executor(
            strand_,
            std::bind(&HTTPSession::OnRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2)
        ));
}

void HTTPSession::OnRead(boost::system::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    // This means they closed the connection
    if (ec == http::error::end_of_stream)
    {
        return DoClose();
    }

    if (ec)
    {
        return fail(ec, "read");
    }

    // Send the response
    handleRequest(*doc_root_, std::move(req_), lambda_);
}

void HTTPSession::OnWrite(boost::system::error_code ec, std::size_t bytes_transferred, bool close)
{
    boost::ignore_unused(bytes_transferred);

    if (ec)
    {
        return fail(ec, "write");
    }

    if (close)
    {
        return DoClose();
    }

    res_ = nullptr;

    DoRead();
}

void HTTPSession::DoClose()
{
    boost::system::error_code ec;
    socket_.shutdown(tcp::socket::shutdown_send, ec);
}

// void HTTPSession::doSession(boost::asio::ip::tcp::socket& socket, std::string const& doc_root)
// {
//     bool close = false;
//     boost::system::error_code ec;

//     // This buffer is required to persist across reads
//     boost::beast::flat_buffer buffer;

//     // This lambda is used to send messages
//     send_lambda<boost::asio::ip::tcp::socket> lambda{socket, close, ec};

//     // for(;;)
//     // {
//     //     // Read a request
//     //     boost::beast::http::request<boost::beast::http::string_body> req;
//     //     boost::beast::http::read(socket, buffer, req, ec);
//     //     if(ec == boost::beast::http::error::end_of_stream)
//     //         break;
//     //     if(ec)
//     //         return fail(ec, "read");

//     //     // Send the response
//     //     handleRequest(doc_root, std::move(req), lambda);
//     //     if(ec)
//     //         return fail(ec, "write");
//     //     if(close)
//     //     {
//     //         // This means we should close the connection, usually because
//     //         // the response indicated the "Connection: close" semantic.
//     //         break;
//     //     }
//     // }

//     // Send a TCP shutdown
//     socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);

//     // At this point the connection is closed gracefully
// }

// template<class Body, class Allocator, class Send>
// void HTTPSession::handleRequest(boost::beast::string_view doc_root, boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>&& req, Send&& send)
// {
//     // Returns a bad request response
//     auto const bad_request =
//     [&req](boost::beast::string_view why)
//     {
//         boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::bad_request, req.version()};
//         res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
//         res.set(boost::beast::http::field::content_type, "text/html");
//         res.keep_alive(req.keep_alive());
//         res.body() = why.to_string();
//         res.prepare_payload();
//         return res;
//     };

//     // Returns a not found response
//     auto const not_found =
//     [&req](boost::beast::string_view target)
//     {
//         boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::not_found, req.version()};
//         res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
//         res.set(boost::beast::http::field::content_type, "text/html");
//         res.keep_alive(req.keep_alive());
//         res.body() = "The resource '" + target.to_string() + "' was not found.";
//         res.prepare_payload();
//         return res;
//     };

//     // Returns a server error response
//     auto const server_error =
//     [&req](boost::beast::string_view what)
//     {
//         boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::internal_server_error, req.version()};
//         res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
//         res.set(boost::beast::http::field::content_type, "text/html");
//         res.keep_alive(req.keep_alive());
//         res.body() = "An error occurred: '" + what.to_string() + "'";
//         res.prepare_payload();
//         return res;
//     };

//     // Make sure we can handle the method
//     if( req.method() != boost::beast::http::verb::get &&
//         req.method() != boost::beast::http::verb::head)
//         return send(bad_request("Unknown HTTP-method"));

//     // Request path must be absolute and not contain "..".
//     if( req.target().empty() ||
//         req.target()[0] != '/' ||
//         req.target().find("..") != boost::beast::string_view::npos)
//         return send(bad_request("Illegal request-target"));

//     // Build the path to the requested file
//     std::string path = catPath(doc_root, req.target());
//     if(req.target().back() == '/')
//         path.append("index.html");

//     // Attempt to open the file
//     boost::beast::error_code ec;
//     boost::beast::http::file_body::value_type body;
//     body.open(path.c_str(), boost::beast::file_mode::scan, ec);

//     // Handle the case where the file doesn't exist
//     if(ec == boost::system::errc::no_such_file_or_directory)
//         return send(not_found(req.target()));

//     // Handle an unknown error
//     if(ec)
//         return send(server_error(ec.message()));

//     // Cache the size since we need it after the move
//     auto const size = body.size();

//     // Respond to HEAD request
//     if(req.method() == boost::beast::http::verb::head)
//     {
//         boost::beast::http::response<boost::beast::http::empty_body> res{boost::beast::http::status::ok, req.version()};
//         res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
//         res.set(boost::beast::http::field::content_type, mimeType(path));
//         res.content_length(size);
//         res.keep_alive(req.keep_alive());
//         return send(std::move(res));
//     }

//     // Respond to GET request
//     boost::beast::http::response<boost::beast::http::file_body> res{
//         std::piecewise_construct,
//         std::make_tuple(std::move(body)),
//         std::make_tuple(boost::beast::http::status::ok, req.version())};
//     res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
//     res.set(boost::beast::http::field::content_type, mimeType(path));
//     res.content_length(size);
//     res.keep_alive(req.keep_alive());
//     return send(std::move(res));
// }

// void HTTPSession::fail(boost::system::error_code ec, char const* what)
// {
//     std::cerr << what << ": " << ec.message() << "\n";
// }