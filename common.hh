#ifndef COMMON_HH
#define COMMON_HH

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/strand.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/config.hpp>
#include <json/json.h>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <map>
#include <regex>

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>
// namespace json = boost::property_tree;  // from <boost/property_tree/ptree.hpp> bug, out format is string.

#define WST_VERSION "wst-mbs:server-1.0"
#define WST_API_VERSION "V1.0"

#define WST_PATH "/api/v1/storage/record"
#define WST_CREATE_RECORD "/api/v1/storage/record"
#define WST_QUERY_RECORD "/api/v1/storage/record/:rid"
#define WST_QUERY_ALL_RECORD "/api/v1/storage/record"
#define WST_UPDATE_RECORD "/api/v1/storage/record/:rid"
#define WST_DELETE_RECORD "/api/v1/storage/record:rid"
#define WST_DELETE_ALL_RECORD "/api/v1/storage/record"

enum ErrCode {
    Ok = 0,
    JsonParseErr = 3000,
    CreateRecordErr = 4000,
};

typedef struct FileInfo_ 
{
    std::string name;
    std::string bucket;
    std::string object;
    std::string type;
}FileInfo;

#endif // COMMON_HH