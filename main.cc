#include "servers/httpserver.hh"
#include "handler/manager.hh"

int main(int argc, char **argv)
{
    HTTPServer http;

    http.Run();
    // Manager manager;
    // manager.PutFileToServer();
}