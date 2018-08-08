#ifndef AGORA_RECORD_HH
#define AGORA_RECORD_HH

#include "IAgoraRecordingEngine.h"

class Record : public agora::recording::IRecordingEngineEventHandler
{
public:
    Record();
    virtual ~Record();

    bool CreateChannel(const std::string &appid, const std::string &channelKey, const std::string &name,  agora::linuxsdk::uid_t uid,
                agora::recording::RecordingConfig &config);

    bool LeaveChannel();
    
protected:
    /**
     *  Callback when an error occurred during the runtime of recording engine
     *
     *
     *  @param error        Error code
     *  @param stat_code    state code
     *
     */
    virtual void onError(int error, agora::linuxsdk::STAT_CODE_TYPE stat_code);

    /**
     *  Callback when an warning occurred during the runtime of recording engine
     *
     *
     *  @param warn         warning code
     *
     */
    virtual void onWarning(int warn);
   
    /**
     *  Callback when the user hase successfully joined the specified channel
     *
     *
     *  @param channelID    channel ID 
     *  @param uid          User ID
     *
     */
    virtual void onJoinChannelSuccess(const char * channelId, uid_t uid);
   
    /**
     *  Callback when recording application successfully left the channel
     *
     *
     *  @param code        leave path code
     *
     */
    virtual void onLeaveChannel(agora::linuxsdk::LEAVE_PATH_CODE code);

    /**
     *  Callback when another user successfully joined the channel
     *
     *
     *  @param uid          user ID
     *  @param infos        user join information    
     *
     */
    virtual void onUserJoined(uid_t uid, agora::linuxsdk::UserJoinInfos &infos);
   
    /**
     *  Callback when a user left the channel or gone offline
     *
     *
     *  @param uid          user ID
     *  @param reason       offline reason    
     *
     */
    virtual void onUserOffline(uid_t uid, agora::linuxsdk::USER_OFFLINE_REASON_TYPE reason);

    /**
     *  Callback when received a audio frame
     *
     *
     *  @param uid          user ID
     *  @param frame        pointer to received audio frame    
     *
     */
    virtual void audioFrameReceived(unsigned int uid, const agora::linuxsdk::AudioFrame *frame) const;

    /**
     *  Callback when received a video frame
     *
     *
     *  @param uid          user ID
     *  @param frame        pointer to received video frame    
     *
     */
    virtual void videoFrameReceived(unsigned int uid, const agora::linuxsdk::VideoFrame *frame) const;

private:
    agora::recording::IRecordingEngine *engine_;

    agora::recording::RecordingConfig config_;
};

#endif // AGORA_RECORD_HH