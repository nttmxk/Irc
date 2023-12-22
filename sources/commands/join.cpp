#include "../../includes/Command.hpp"

/* JOIN <channel>{,<channel>} [<key>{,<key>}]
* 클라이언트가 특정 채널에 참여하고자 할 때 사용, 채널마다 지정된 키를 사용하여 참여 가능
* - <channel>: 클라이언트가 참여하려는 채널의 이름. 여러 채널에 동시에 참여할 수 있으며 콤마(,)로 구분
* - [<key>{,<key>}]: 채널에 참여할 때 사용할 키를 지정. 채널마다 키를 설정할 수 있으며, 각 채널의 키는 콤마(,)로 구분. 생략 가능
* 
* 성공적으로 채널에 참여하면 서버는 순서대로 다음을 전송해야 함함
*	1. JOIN 메시지: 클라이언트를 메시지 <source>로, 참여한 채널을 메시지의 첫 번째 매개변수로 보낸다.
*	2. topic이 있다면 332 및 333(선택적)을 보냄, topic 없으면 안보냄
*	3. 현재 채널에 참여한 사용자 목록: 하나 이상의 353 과 하나의 366을 전송. 서버에서 전송된 353 메시지는 요청한 클라이언트를 포함해야 함
*
* 참여 가능 여부 확인
* 	- 채널의 키, 클라이언트 제한, 밴 예외, 초대 전용 예외 및 기타 채널 모드에 영향을 받음
* 	- 서버는 클라이언트가 한 번에 참여할 수 있는 채널 수를 제한할 수 있다. 이 제한은 CHANLIMIT RPL_ISUPPORT 매개변수에 정의되어야 함
*	  클라이언트가 이 제한을 초과하여 채널에 참여할 수 없는 경우, 405 응답을 받게 되며, 커멘드는 fail 처리
*
* 클라이언트가 채널에 참여하게 되면..!
* 	- 클라이언트가 채널에 참여하면 JOIN, PART, KICK, MODE 메시지 등 해당 채널에 영향을 미치는 모든 정보를 받게 됨
*	- 채널로 전송된 PRIVMSG와 NOTICE 메시지를 수신하게 되며, 동시에 해당 채널에 참여한 다른 클라이언트의 QUIT 메시지도 받음
* 
* Numeric Replies:
*	- ERR_NEEDMOREPARAMS (461): 필요한 매개변수가 부족하여 명령어를 처리할 수 없음.
*	- ERR_NOSUCHCHANNEL (403): 해당 채널이 존재하지 않음.
*	- ERR_TOOMANYCHANNELS (405): 클라이언트가 지정된 수 이상의 채널에 참여할 수 없음. (X - 클라이언트 참여가능 채널 수 제한 없음)
*	- ERR_BADCHANNELKEY (475): 채널에 설정된 키가 올바르지 않음.
*	- ERR_BANNEDFROMCHAN (474): 클라이언트가 채널에 참여할 권한이 없음 (밴 당함). (X - 벤 기능 없음)
*	- ERR_CHANNELISFULL (471): 채널이 가득 찼음.
*	- ERR_INVITEONLYCHAN (473): 초대 전용 채널에 클라이언트가 참여하려고 시도함. 
*	- ERR_BADCHANMASK (476): 잘못된 채널 마스크가 사용됨. (...?)
*	- RPL_TOPIC (332): 채널의 주제를 전송.
*	- RPL_TOPICWHOTIME (333): 채널 주제의 설정자와 설정 시간을 전송.
*	- RPL_NAMREPLY (353): 현재 채널에 참여한 사용자 목록을 전송.
*	- RPL_ENDOFNAMES (366): 사용자 목록 전송의 끝을 알림.
* 커멘드 사용 예시 
*	- JOIN #foobar                    ; channel #foobar에 참여.
*	- JOIN &foo fubar                 ; channel &foo에 키 "fubar"를 사용하여 참여.
*	- JOIN #foo,&bar fubar            ; channel #foo에 키 "fubar", channel &bar에는 키를 사용하지 않고 참여.
*	- JOIN #foo,#bar fubar,foobar     ; channel #foo에 키 "fubar", channel #bar에 키 "foobar"를 사용하여 참여.
*	- JOIN #foo,#bar                  ; channel #foo와 #bar에 참여.
* Message Examples:
*	- :WiZ JOIN #Twilight_zone        ; WiZ가 채널 #Twilight_zone에 참여.
*	- :dan-!d@localhost JOIN #test    ; dan-이 채널 #test에 참여.
*/
void Command::join(std::vector<Channel> channelsInServer) {
	
	if (tokens.size() < 2) {
		// ERR_NEEDMOREPARAMS
		return;
	}
	
	std::vector<std::string> channels;
	std::vector<std::string> keys;
	bool hasKeys = tokens.size() >= 3;
	
	channels = splitByComma(tokens[1]);
	if (hasKeys)
		keys = splitByComma(tokens[2]);

	// 벡터 순회하면서 채널에 들어가기
	for (int i=0; i<channels.size(); i++) {
		// 채널이 서버에 존재하는지 확인, 없으면 새로 생성
		Channel* channelPtr = isChannelExist(channelsInServer, channels[i]);
		if (channelPtr == NULL) {
			// create channel
		}

		// 채널 key 가 맞는지 확인
		std::string keyOfChannel = channelPtr->getKey();
		if (keyOfChannel != "") {
			if (i >= keys.size()) {
				// error reply 475
			}
			else if (keys[i] != keyOfChannel) {
				// error reply 475
			}
		}

		
	}
	/* 로직
		- is valid channel name
			- if no -> loop break.. ? or continue
		- is channel full
			- if yes -> error reply 471
		- is invite-only channel
			- if yes -> error reply 473
		- join channel
	*/
	/* 예외 상황
		- 해당 채널이 없는 경우 => 채널 만들어서 입장
		- 채널 키가 있는데 입력받은 키와 맞지 않은 경우 => ERR_BADCHANNELKEY (475)
		- 채널에 참여가능한 user limit을 초과한 경우 => ERR_CHANNELISFULL (471)
		- 초대 전용 채널에 참여하려고 한 경우 => ERR_INVITEONLYCHAN (473)
	*/
	
	// 성공한 경우
	// 1. Join Msg -> 채널로 보내는 건가..?
	// 2. 참여한 채널에 토픽이 잇으면 RPL_TOPIC (332)
	// 3. 현재 채널에 참여한 사용자 목록 - RPL_NAMREPLY (353) * 채널 참여자 수 + RPL_ENDOFNAMES (366)

}

// ',' 기준으로 채널 split
static std::vector<std::string> splitByComma(std::string str) {
	std::vector<std::string> ret;

	std::size_t pos = 0;
	while (pos != std::string::npos) {
		std::size_t tmpPos = str.find(',', pos);
		int count = (tmpPos == std::string::npos) ? str.size() - pos : tmpPos - pos;
		ret.push_back(str.substr(pos, count));
		pos = (tmpPos == std::string::npos) ? tmpPos : tmpPos + 1;
	}
	
	return ret;
}


static std::string getValidChannelName(std::string channel) {
	return channel;
}

static Channel* isChannelExist(std::vector<Channel> channelsInServer, std::string channelName) {
	std::vector<Channel>::iterator it = channelsInServer.begin();
	for ( ; it != channelsInServer.end(); it++) {
		if (channelName == it->getName())
			return &(*it);
	}
	return NULL;
}







