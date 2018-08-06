#include "httputils.hh"

boost::beast::string_view mime_type(boost::beast::string_view path)
{
    using boost::beast::iequals;
    auto const ext = [&path]
    {
        auto const pos = path.rfind(".");
        if (pos == boost::beast::string_view::npos)
        {
            return boost::beast::string_view{};
        }
        return path.substr(pos);
    }();
    if (iequals(ext, ".json")) return "application/json";

    return "application/text";
}

void fail(boost::system::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}