#ifndef MANAGER_HH
#define MANAGER_HH

#include "common.hh"

class Manager 
{
public:

    void PutFileToServer();

    void GetURLOfUpload(std::string bucket, std::string object, std::string type);

private:
    Json::Value common();

    void sendToServer(std::string target, std::string body);
};

#endif // MANAGER_HH