#include "agorarecord.hh"
#include "manager.hh"

Record::Record()
{

}

Record::~Record()
{

}

bool Record::CreateChannel(const std::string &appid, const std::string &channelKey, const std::string &channelId,  agora::linuxsdk::uid_t uid,
                agora::recording::RecordingConfig &config)
{
    if ((engine_ = agora::recording::IRecordingEngine::createAgoraRecordingEngine(appid.c_str(), this)) == NULL)
    {
        return false;
    }

    if(agora::linuxsdk::ERR_OK != engine_->joinChannel(channelKey.c_str(), channelId.c_str(), uid, config))
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


	Manager().PutFileToServer();
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
	Manager manager;
	manager.PutFileToServer();
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
    // if(infos.storageDir)
    // {
    //     m_storage_dir = std::string(infos.storageDir);
    //     m_logdir = m_storage_dir;
    // }

    peers_.push_back(uid);
    setVideoMixLayout();
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
    peers_.erase(std::remove(peers_.begin(), peers_.end(), uid), peers_.end());
    
    //When the user is offline, we can re-layout the canvas
    setVideoMixLayout();
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

int Record::setVideoMixLayout()
{
	agora::linuxsdk::VideoMixingLayout layout;
	layout.canvasWidth = 1280;
	layout.canvasHeight = 720;
	layout.backgroundColor = "#000000";

	layout.regionCount = peers_.size();

	if (!peers_.empty())
	{
		agora::linuxsdk::VideoMixingLayout::Region *regionList = new agora::linuxsdk::VideoMixingLayout::Region[peers_.size()];
		
		regionList[0].uid = peers_[0];
		regionList[0].x = 0.f;		
		regionList[0].y = 0.f;
		regionList[0].width = 1280.0;
		regionList[0].height = 720.0;
		regionList[0].zOrder = 0;
		regionList[0].alpha = 1.f;
		regionList[0].renderMode = 2;

		float canvasWidth = 1280.0;
		float canvasHeight = 720.0;

		float viewVEdge = 0.3;
		float viewHEdge = 0.025;
		float viewHeight = viewVEdge * (canvasWidth / canvasHeight);
		float viewWidth = viewHEdge * (canvasWidth / canvasHeight);

		layout.regions = regionList;
	}
	else
	{
		layout.regions = NULL;
    }

	engine_->setVideoMixingLayout(layout);

	return 0;
}