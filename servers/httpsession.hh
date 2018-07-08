#ifndef HTTP_SESSION_HH
#define HTTP_SESSION_HH

class HTTPSession
{
public:
    HTTPSession();
    virtual ~HTTPSession();

    static void doSession();
};

#endif /// HTTP_SESSION_HH