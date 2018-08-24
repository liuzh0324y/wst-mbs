#include "manager.hh"

template<class Ext, class Int = boost::any>
struct veriant_translator
{
    typedef Ext external_type;
    typedef Int internal_type;

    external_type get_value(const internal_type& value) const
    {
        return boost::any_cast<external_type>(value);
    }

    internal_type put_value(const external_type& value) const
    {
        return value;
    }
};

void Manager::PutFileToServer()
{
    auto const target = "/api/v1/cloundstorage/file";
    
    Json::Value out = common();
    Json::Value data;
    data["name"] = "sample.mp4";
    data["type"] = "mbs";
    data["url"] = "http://localhost:8080/sample.mp4";
    data["key"] = "XXXX-XXXX-XXXX-XXXX";
    data["secret"] = "SSSS-SSSS-SSSS-SSSS";
    data["bucket"] = "llvision";
    data["object"] = "sample.mp4";
    data["region"] = "beijing";
    data["endpoint"] = "www.llvision.com";
    data["description"] = "sample description";
    out["data"] = data;
    
    sendToServer(target, out.toStyledString());
}

Json::Value Manager::common()
{
    Json::Value root;
    root["version"] = WST_API_VERSION;
    root["seqnum"] = 1;
    root["from"] = "mbs";
    root["to"] = "client";
    root["type"] = "mbs";
    root["number"] = "XXXX-XXXX-XXXX-XXXX";
    root["uid"] = "XXXX-XXXX-XXXX-XXXX";
    return root;
}

void Manager::sendToServer(std::string target, std::string body)
{
    try
    {
        auto const host = "10.33.48.20";
        auto const port = "8083";
        boost::asio::io_context ioc;

        tcp::resolver resolver{ioc};
        tcp::socket socket{ioc};

        auto const results = resolver.resolve(host, port);

        boost::asio::connect(socket, results.begin(), results.end());

        // Set up an HTTP PUT request message
        http::request<http::string_body> req;
        req.target(target);
        req.method(http::verb::put);
        req.set(http::field::host, host);
        req.set(http::field::user_agent, "mbs:v1.0");
        req.set(http::field::accept, "application/json");
        req.set(http::field::content_type, "application/json");
        req.set(http::field::connection, "close");
        req.body() = body.c_str();

        http::write(socket, req);

        std::cout << "write: {" << '\n' << req << "}" << std::endl;

        boost::beast::flat_buffer buffer;

        http::response<http::dynamic_body> res;

        http::read(socket, buffer, res);

        std::cout << "read: {" << '\n' << res << "}" << std::endl;

        // Gracefully close the socket
        boost::system::error_code ec;
        socket.shutdown(tcp::socket::shutdown_both, ec);

        // not_connected happends sometimes
        // so don't bother reporting it.
        if (ec && ec != boost::system::errc::not_connected)
            throw boost::system::system_error{ec};
    }
    catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}