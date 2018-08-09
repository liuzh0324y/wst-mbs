#ifndef HANDLER_HH
#define HANDLER_HH

#include "common.hh"
#include "agorarecord.hh"

static std::string APPLITEDIR = "/opt/wst-mbs/tools";

struct RecordInfo 
{
    std::string appid;
    std::string channel;
    std::string name;
    std::string sid;
    std::string uid;
    std::string type;
    bool isMix;
};

class Handler 
{
public:
    static Handler & Instance();


    template<class Body, class Allocator, class Send>
    void HandleRequest(http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send);

    bool CreateRecord(RecordInfo info);
    
    void LeaveRecord();

protected:
    static json::ptree common();

    static std::string resCreateRecord();

    static std::string resQueryRecordById();

    static std::string resQueryAllRecord();

    static std::string resUpdateRecord();

    static std::string resDeleteRecordById();

    static std::string resDeleteAllRecord();

    static std::string parseurl(std::string target);

private:
    Handler();
    virtual ~Handler();

    Record recorder;
};

template<class Body, class Allocator, class Send>
void Handler::HandleRequest(http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send)
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
        res.body() = Handler::resCreateRecord();
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
        res.body() = resQueryRecordById();
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
        res.body() = resQueryAllRecord();
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
        res.body() = resUpdateRecord();
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
        res.body() = resDeleteRecordById();
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
        res.body() = resDeleteAllRecord();
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
        RecordInfo info;
        if (CreateRecord(info) != true)
        {
            std::cout << "create record failed." << std::endl;
        }
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
        LeaveRecord();
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
#endif // HANDLER_HH