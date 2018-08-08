#include "agorarecord.hh"

Record::Record()
{

}

Record::~Record()
{

}

bool Record::CreateChannel(const std::string &appid, const std::string &channelKey, const std::string &name,  agora::linuxsdk::uid_t uid,
                agora::recording::RecordingConfig &config)
{
    if ((engine_ = agora::recording::IRecordingEngine::createAgoraRecordingEngine(appid.c_str(), this)) == NULL)
    {
        return false;
    }

    if(agora::linuxsdk::ERR_OK != engine_->joinChannel(channelKey.c_str(), name.c_str(), uid, config))
    {
        return false;
    }

    config_ = config;

    return true;
}

bool Record::LeaveChannel()
{
    if (engine_)
    {
        engine_->leaveChannel();
    }
}
/**
 *  Callback when an error occurred during the runtime of recording engine
 *
 *
 *  @param error        Error code
 *  @param stat_code    state code
 *
 */
void Record::onError(int error, agora::linuxsdk::STAT_CODE_TYPE stat_code)
{

}

/**
 *  Callback when an warning occurred during the runtime of recording engine
 *
 *
 *  @param warn         warning code
 *
 */
void Record::onWarning(int warn)
{

}

/**
 *  Callback when the user hase successfully joined the specified channel
 *
 *
 *  @param channelID    channel ID 
 *  @param uid          User ID
 *
 */
void Record::onJoinChannelSuccess(const char * channelId, uid_t uid)
{

}

/**
 *  Callback when recording application successfully left the channel
 *
 *
 *  @param code        leave path code
 *
 */
void Record::onLeaveChannel(agora::linuxsdk::LEAVE_PATH_CODE code)
{

}

/**
 *  Callback when another user successfully joined the channel
 *
 *
 *  @param uid          user ID
 *  @param infos        user join information    
 *
 */
void Record::onUserJoined(uid_t uid, agora::linuxsdk::UserJoinInfos &infos)
{

}

/**
 *  Callback when a user left the channel or gone offline
 *
 *
 *  @param uid          user ID
 *  @param reason       offline reason    
 *
 */
void Record::onUserOffline(uid_t uid, agora::linuxsdk::USER_OFFLINE_REASON_TYPE reason)
{

}

/**
 *  Callback when received a audio frame
 *
 *
 *  @param uid          user ID
 *  @param frame        pointer to received audio frame    
 *
 */
void Record::audioFrameReceived(unsigned int uid, const agora::linuxsdk::AudioFrame *frame) const
{

}

/**
 *  Callback when received a video frame
 *
 *
 *  @param uid          user ID
 *  @param frame        pointer to received video frame    
 *
 */
void Record::videoFrameReceived(unsigned int uid, const agora::linuxsdk::VideoFrame *frame) const
{

}