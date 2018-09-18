#include "handler.hh"
#include "manager.hh"

Handler::Handler()
{

}

Handler::~Handler()
{
    
}

Handler & Handler::Instance()
{
    static Handler instance;
    return instance;
}

bool Handler::CreateRecord(RecordInfo info)
{
    Record recorder;
    std::string channelKey;
    std::string appid = info.appid;
    std::string channelId = info.channel;
    std::string name = info.name;
    std::string type = info.type;
    std::string decryptionMode;
    std::string secret;
    std::string mixResolution = info.resolution;
    std::string recordPath("/record");
    std::cout << "appid: " << appid << std::endl;
    int channelProfile = 1;
    int streamType = 0;
    int triggerMode = 0;
    bool isMix = info.isMix;
    unsigned int uid = 0;
    agora::recording::RecordingConfig config;
    // record type
    if (type.compare("video") == 0)
    {
        config.isVideoOnly = true;
        config.isAudioOnly = false;
    }
    else if (type.compare("audio") == 0)
    {
        config.isVideoOnly = false;
        config.isAudioOnly = true;
    }
    else if (type.compare("all"))
    {
        config.isVideoOnly = false;
        config.isAudioOnly = false;
    }
    
    
    
    config.isMixingEnabled = isMix;
    config.mixedVideoAudio = isMix;
    config.mixResolution = isMix ? const_cast<char*>(mixResolution.c_str()):NULL;

    config.decryptionMode = decryptionMode.empty() ? NULL : const_cast<char*>(decryptionMode.c_str());
    config.secret = secret.empty() ? NULL : const_cast<char*>(secret.c_str());

    config.appliteDir = const_cast<char*>(APPLITEDIR.c_str());
    config.recordFileRootDir = const_cast<char*>(recordPath.c_str());
    config.cfgFilePath = NULL;

    config.decodeVideo = agora::linuxsdk::VIDEO_FORMAT_H264_FRAME_TYPE;
    config.decodeAudio = agora::linuxsdk::AUDIO_FORMAT_AAC_FRAME_TYPE; 
    
    config.lowUdpPort = 0;
    config.highUdpPort = 0;  

    config.idleLimitSec = 300;
    config.captureInterval = 5;
    config.channelProfile = static_cast<agora::linuxsdk::CHANNEL_PROFILE_TYPE>(channelProfile);
    config.streamType = static_cast<agora::linuxsdk::REMOTE_VIDEO_STREAM_TYPE>(streamType);
    config.triggerMode = static_cast<agora::linuxsdk::TRIGGER_MODE_TYPE>(triggerMode);
    config.lang = agora::linuxsdk::CPP_LANG;
    config.proxyServer = NULL;

    return recorder.CreateChannel(appid, channelKey, channelId, uid, config);
}
    
void Handler::LeaveRecord()
{
    recorder.LeaveChannel();
}

Json::Value Handler::common()
{
    // try 
    // {
    //     json::ptree pt;
    //     pt.put("version",WST_API_VERSION);
    //     pt.put("seqnum", 1);
    //     pt.put("from", "wst-mbs");
    //     pt.put("to", "client");
    //     pt.put("type", "mbs");
    //     pt.put("number", "0108935265");
    //     pt.put("message", "");
    //     pt.put("code", 0);
    //     return pt;
    // }
    // catch (json::ptree_error ec)
    // {
    //     ec.what();
    //     return json::ptree();
    // }

    Json::Value root;
    root["version"] = WST_API_VERSION;
    root["seqnum"] = 1;
    root["from"] = "mbs";
    root["to"] = "client";
    root["type"] = "mbs";
    root["number"] = "XXXX-XXXX-XXXX-XXXX";
    root["message"] = "";
    root["code"] = 0;
    return root;
}

std::string Handler::resCreateRecord()
{
    Json::Value out = common();
    Json::Value data;
    data["id"] = "dddd-dddd-dddd-dddd";
    data["name"] = "sample.mp4";
    data["type"] = "mp4";
    data["path"] = "/root/path";
    data["url"] = "http://localhost:8080/sample.mp4";
    data["size"] = 1024;
    out["data"] = data;
    Manager mgr;
    mgr.GetURLOfUpload("llvision", "sample30", "image/jpeg");
    return out.toStyledString();    
}

std::string Handler::resCreateRecordErr()
{
    Json::Value root;
    root["version"] = WST_API_VERSION;
    root["seqnum"] = 1;
    root["from"] = "mbs";
    root["to"] = "client";
    root["type"] = "mbs";
    root["number"] = "XXXX-XXXX-XXXX-XXXX";
    root["message"] = "Failed to create the record channel.";
    root["code"] = CreateRecordErr;

    return root.toStyledString();
}

std::string Handler::resQueryRecordById()
{
    Json::Value out = common();
    Json::Value data;
    data["id"] = "dddd-dddd-dddd-dddd";
    data["type"] = "mp4";
    data["path"] = "/root/path";
    data["url"] = "http://localhost:8080/sample.mp4";
    data["size"] = 1024;
    data["description"] = "sample file.";
    data["progress"] = 20;
    out["data"] = data;
    
    return out.toStyledString();
}

std::string Handler::resQueryAllRecord()
{
    Json::Value out = common();

    return out.toStyledString();
}

std::string Handler::resUpdateRecord()
{
    Json::Value out = common();

    return out.toStyledString();
}

std::string Handler::resDeleteRecordById()
{
    Json::Value out = common();

    return out.toStyledString();
}

std::string Handler::resDeleteAllRecord()
{
    Json::Value out = common();

    return out.toStyledString();
}

std::string Handler::parseurl(std::string target)
{
    std::size_t size = target.find("?");
    if (size == 0)
    {
        return target;
    }
    else
    {
        return target.substr(0, size);
    }
}

std::string Handler::resParseErr()
{
    Json::Value root;
    root["version"] = WST_API_VERSION;
    root["seqnum"] = 1;
    root["from"] = "mbs";
    root["to"] = "client";
    root["type"] = "mbs";
    root["number"] = "XXXX-XXXX-XXXX-XXXX";
    root["message"] = "Failed to parse json format.";
    root["code"] = JsonParseErr;

    return root.toStyledString();
}