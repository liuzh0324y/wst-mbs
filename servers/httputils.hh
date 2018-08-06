#ifndef HTTP_UTILS_HH
#define HTTP_UTILS_HH

#include "common.hh"

boost::beast::string_view mime_type(boost::beast::string_view path);

void fail(boost::system::error_code ec, char const* what);

#endif // HTTP_UTILS_HH