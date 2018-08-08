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

json::ptree common()
{
    try 
    {
        json::ptree pt;
        pt.put("version",WST_API_VERSION);
        pt.put("seqnum", 1);
        pt.put("from", "wst-mbs");
        pt.put("to", "client");
        pt.put("type", "mbs");
        pt.put("number", "0108935265");
        pt.put("message", "");
        pt.put("code", 0);
        return pt;
    }
    catch (json::ptree_error ec)
    {
        ec.what();
        return json::ptree();
    }
}

std::string createRecord()
{
    std::stringstream ss;
    try
    {
        json::ptree pt = common();
        json::ptree data;
        data.put("id", "dddd-dddd-dddd-dddd");
        data.put("name","sample.mp4");
        data.put("type", "mp4");
        data.put("path", "/root/path");
        data.put("url", "http://localhost:8080/sample.mp4");
        data.put("size", 1024);
        pt.add_child("data", data);
        json::write_json(ss, pt);
        std::cout << "pt.data(): " << ss.str()<< std::endl;
        return ss.str();
    }
    catch (json::ptree_error ec)
    {
        ec.what();
        return std::string();
    }
}

std::string queryRecordById()
{
    std::stringstream ss;
    try
    {
        json::ptree pt = common();
        json::ptree data;
        data.put("id", "dddd-dddd-dddd-dddd");
        data.put("type", "mp4");
        data.put("path", "/root/path");
        data.put("url", "http://localhost:8080/sample.mp4");
        data.put("size", 1024);
        data.put("description", "sample file.");
        data.put("progress", 20);
        pt.add_child("data", data);
        json::write_json(ss, pt);
        return ss.str();
    }
    catch (json::ptree_error ec)
    {
        ec.what();
        return std::string();
    }
}

std::string queryAllRecord()
{
    try
    {
        std::stringstream ss;
        json::ptree pt = common();
        // json::ptree data;
        // data.put
        json::write_json(ss, pt);
        return ss.str();
    }
    catch (json::ptree_error ec)
    {
        ec.what();
        return std::string();
    }
}

std::string UpdateRecord()
{
    try
    {
        std::stringstream ss;
        json::ptree pt = common();

        json::write_json(ss, pt);
        return ss.str();
    }
    catch (json::ptree_error ec)
    {
        ec.what();
        return std::string();
    }
}

std::string DeleteRecordById()
{
    try
    {
        std::stringstream ss;
        json::ptree pt = common();

        json::write_json(ss, pt);
        return ss.str();
    }
    catch (json::ptree_error ec)
    {
        ec.what();
        return std::string();
    }
}

std::string DeleteAllRecord()
{
    try
    {
        std::stringstream ss;
        json::ptree pt = common();

        json::write_json(ss, pt);
        return ss.str();
    }
    catch (json::ptree_error ec)
    {
        ec.what();
        return std::string();
    }
}

std::string parseurl(std::string target)
{
    std::size_t size = target.find("?");
    if (size == 0)
    {
        return target;
    }
    else
    {
        return target.substr(0, size);
    }
}

std::map<std::string, std::string> parseParam(std::string uri)
{
    // std::regex reg
}

template<class Body, class Allocator, class Send>
void handleRequest(boost::beast::string_view doc_root, http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send)
{
    // Returns a bad request response
    auto const bad_request =
    [&req](boost::beast::string_view why)
    {
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.set(http::field::server, WST_VERSION);
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
        res.set(http::field::server, WST_VERSION);
        res.set(http::field::content_type, "text/html");
        res.set(http::field::connection, "close");
        res.body() = "nothing";
        res.prepare_payload();
        return res;
    };

    // Returns a server error response
    auto const server_error =
    [&req](boost::beast::string_view what)
    {
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::server, WST_VERSION);
        res.set(http::field::content_type, "text/html");
        res.set(http::field::connection, "close");
        res.body() = "An error occurred: '" + what.to_string() + "'";
        res.prepare_payload();
        return res;
    };

    auto const create_record =
    [&req]()
    {
        http::response<http::string_body> res{http::status::ok, req.version()}; 
        res.set(http::field::server, WST_VERSION);
        res.set(http::field::content_type, "application/json");
        res.set(http::field::connection, "close");
        res.body() = createRecord();
        res.prepare_payload();
        return res;
    };

    auto const query_record_by_id =
    [&req]()
    {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, WST_VERSION);
        res.set(http::field::content_type, "application/json");
        res.set(http::field::connection, "close");
        res.body() = queryRecordById();
        res.prepare_payload();
        return res;
    };

    auto const query_all_record =
    [&req]()
    {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, WST_VERSION);
        res.set(http::field::content_type, "application/json");
        res.set(http::field::connection, "close");
        res.body() = queryAllRecord();
        res.prepare_payload();
        return res;
    };

    auto const update_record =
    [&req]()
    {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, WST_VERSION);
        res.set(http::field::content_type, "application/json");
        res.set(http::field::connection, "close");
        res.body() = UpdateRecord();
        res.prepare_payload();
        return res;
    };

    auto const delete_record =
    [&req]()
    {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, WST_VERSION);
        res.set(http::field::content_type, "application/json");
        res.set(http::field::connection, "close");
        res.body() = DeleteRecordById();
        res.prepare_payload();
        return res;
    };

    auto const delete_all_record =
    [&req]()
    {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, WST_VERSION);
        res.set(http::field::content_type, "application/json");
        res.set(http::field::connection, "close");
        res.body() = DeleteAllRecord();
        res.prepare_payload();
        return res;
    };

    std::string url = parseurl(req.target().to_string());
    std::cout << " -- url: " << parseurl(req.target().to_string()) << std::endl;
    // Parse uri and request method
    std::string uri = req.target().to_string();

    // Create the record
    if ((url.compare(WST_PATH) == 0) && (req.method() == http::verb::put))
    {
        std::cout << " ** create the record." << std::endl;
        return send(create_record());
    }

    // Query the record by id
    else if ((url.compare(WST_PATH) > 0) && (req.method() == http::verb::get))
    {
        std::cout << " ** query the record by id." << std::endl;
        return send(query_record_by_id());
    }

    // Query all record
    else if ((url.compare(WST_PATH) == 0) && (req.method() == http::verb::get))
    {
        std::cout << " ** query all record." << std::endl;
        return send(query_all_record());
    }

    // Update the record
    else if ((url.compare(WST_PATH) > 0) && (req.method() == http::verb::post))
    {
        std::cout << " ** update the record." << std::endl;
        return send(update_record());
    }

    // Delete the record by id
    else if ((url.compare(WST_PATH) > 0) && (req.method() == http::verb::delete_))
    {
        std::cout << " ** delete the record" << std::endl;
        return send(delete_record());
    }

    // Delete all record
    else if ((url.compare(WST_PATH) == 0) && (req.method() == http::verb::delete_))
    {
        std::cout << " ** delete all record." << std::endl;
        return send(delete_all_record());
    }

    // 
    std::cout << "handler requst." << std::endl;
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
    std::cout << "session do read." << std::endl;
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
    std::cout << "session.read" << std::endl;
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