#ifndef HTTP_SERVER_HH
#define HTTP_SERVER_HH

#include <boost/config.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

class HTTPServer 
{
public:
    HTTPServer();
    virtual ~HTTPServer();

    void Run();

protected:
    ///
    /// Reture a reasonable mime type based on the extension of a file.
    ///
    boost::beast::string_view  mimeType(boost::beast::string_view path);

    ///
    /// Append an HTTP rel-path to a local filesystem path.
    /// The returned path is normalized for the platform.
    ///
    std::string catPath(boost::beast::string_view base, boost::beast::string_view path);

    ///
    /// This function produces an HTTP response for the given request.
    /// The type of the response object depends on the contents of the request,
    /// so the interface requires the caller to pass a generic lambda for receiving the response.
    ///
    template<class Body, class Allocator, class Send>
    void handleRequest(boost::beast::string_view doc_root, boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>&& req, Send&& send);

    ///
    /// Report a failure
    ///
    void fail(boost::system::error_code ec, char const* what);

    ///
    /// Handles an HTTP server connection
    ///
    void doSession(boost::asio::ip::tcp::socket& socket);

}; /// HTTPServer

#endif /// HTTP_SERVER_HH