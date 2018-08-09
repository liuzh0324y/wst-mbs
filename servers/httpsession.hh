#ifndef HTTP_SESSION_HH
#define HTTP_SESSION_HH

#include "common.hh"

class HTTPSession : public std::enable_shared_from_this<HTTPSession>
{
public:
    explicit HTTPSession(tcp::socket socket);
    virtual ~HTTPSession();

    void Run();

    void DoRead();

    void OnRead(boost::system::error_code ec, std::size_t bytes_transferred);

    void OnWrite(boost::system::error_code ec, std::size_t bytes_transferred, bool close);

    void DoClose();

protected:
    

private:
    struct send_lambda
    {
        HTTPSession& self_;

        explicit send_lambda(HTTPSession& self)
            : self_(self)
        {

        }

        template<bool isRequest, class Body, class Fields>
        void operator()(http::message<isRequest, Body, Fields>&& msg) const
        {
            auto sp = std::make_shared<http::message<isRequest, Body, Fields>>(std::move(msg));

            self_.res_ = sp;

            http::async_write(self_.socket_, *sp, boost::asio::bind_executor(
                self_.strand_,
                std::bind(&HTTPSession::OnWrite, self_.shared_from_this(), std::placeholders::_1, std::placeholders::_2, sp->need_eof())
            ));
        }
    };

    tcp::socket socket_;
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
    std::shared_ptr<void> res_;
    send_lambda lambda_;
};

#endif /// HTTP_SESSION_HH