#ifndef COMMON_HH
#define COMMON_HH

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/config.hpp>
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
namespace json = boost::property_tree;  // from <boost/property_tree/ptree.hpp>

#define WST_VERSION "wst-mbs:server-1.0"
#define WST_API_VERSION "V1.0"

#define WST_PATH "/api/v1/storage/record"
#define WST_CREATE_RECORD "/api/v1/storage/record"
#define WST_QUERY_RECORD "/api/v1/storage/record/:rid"
#define WST_QUERY_ALL_RECORD "/api/v1/storage/record"
#define WST_UPDATE_RECORD "/api/v1/storage/record/:rid"
#define WST_DELETE_RECORD "/api/v1/storage/record:rid"
#define WST_DELETE_ALL_RECORD "/api/v1/storage/record"

#endif // COMMON_HH