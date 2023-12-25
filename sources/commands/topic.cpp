#include "../../includes/Command.hpp"
#include <ctime> // time

/* TOPIC <channel> [<topic>]
* 주어진 채널의 토픽을 변경하거나 확인 
* - <channel>: 토픽을 변경하거나 확인할 채널의 이름이다.
* - [<topic>]: 변경할 새로운 토픽 내용이다. 생략 가능, 빈 문자열 가능
* topic이 없으면 (332) 또는 (331) 중 하나가 반환 		ex) TOPIC #test  
* topic이 있으면 channel의 토픽을 topic으로 변경		ex)	TOPIC #test :another topic      
* topic이 있으나 빈 문자열이면, channel의 토픽을 제거	  ex) TOPIC #test :
* 
* - 만약 이 명령을 보내는 클라이언트가 (332) 을 받는 경우, 해당 클라이언트에게 (333) 도 보내야 함
* - 채널의 토픽이 변경되거나 지워지면 해당 채널에 있는 모든 클라이언트(토픽 변경자 포함)는 새로운 토픽을 인수로 하는 TOPIC 명령을 수신하게 된다(또는 토픽이 지워진 경우 빈 인수). 
* 	 이를 통해 토픽이 어떻게 변경되었는지 알릴 수 있다.
* - 나중에 채널에 참여하는 클라이언트는 (332) 숫자를 받게 되며 이에 따라 토픽이 표시된다(또는 토픽이 없는 경우 표시되지 않음).
* 
* Numeric Replies:
*	- ERR_NEEDMOREPARAMS (461): 필요한 매개변수가 부족하여 명령어를 처리할 수 없음.
*	- ERR_NOSUCHCHANNEL (403): 지정된 채널이 존재하지 않아 실패함.
*	- ERR_NOTONCHANNEL (442): 클라이언트가 채널에 참여하지 않아 실패함.
*	- ERR_CHANOPRIVSNEEDED (482): 채널에 보호된 토픽 모드가 설정되어 있고 클라이언트가 적절한 채널 권한이 없어 실패함.
*	- RPL_NOTOPIC (331): 해당 채널에 토픽이 없어 실패함(주어진 경우).
*	- RPL_TOPIC (332): 채널의 토픽이 성공적으로 전송됨(주어진 경우).
*	- RPL_TOPICWHOTIME (333): 채널 토픽의 변경자와 변경 시간을 성공적으로 전송함(주어진 경우).
* 커멘드 사용 예시 
*	- TOPIC #test :New topic          ; "#test" 채널의 토픽을 "New topic"으로 설정.
*	- TOPIC #test :                   ; "#test" 채널의 토픽을 지움.
*	- TOPIC #test                     ; "#test" 채널의 토픽을 확인.
*/
void Command::topic(std::map<std::string, Channel*> &channelsInServer) {
    int numParam = getNumParameter();
	std::string servername = "irc.local";
	std::string nick = client->getNickname();

	if (numParam < 2) {
		messageIndex += numParam + 1;
		sendReply(ERR_NEEDMOREPARAMS(servername, nick, "TOPIC"));
		return;
	}

    std::string targetChannel = tokens[messageIndex + 1];
    std::string newTopic = numParam >= 3 ? tokens[messageIndex + 2] : "";
    messageIndex += numParam + 1;

    // 타겟 채널이 서버에 존재하는지 확인
    Channel* channelPtr = isChannelExist(channelsInServer, targetChannel);
    if (channelPtr == NULL) {
        sendReply(ERR_NOSUCHCHANNEL(servername, nick, targetChannel));
		return;
    }

    // 클라이언트가 타겟 채널에 참여하고 있는지 확인
    if (channelPtr->isInChannel(nick) == false) {
        sendReply(ERR_NOTONCHANNEL(servername, nick, targetChannel));
		return;
    }

    // 채널이 초대 전용 모드일 경우, 클라이언트가 초대권한이 있는지 확인
    if (channelPtr->isModeOn('t') && channelPtr->isOperator(nick) == false) {
        sendReply(ERR_CHANOPRIVSNEEDED(servername, nick, targetChannel));
		return;
    }

    channelPtr->setTopic(newTopic);
    std::string setTimeStr = std::to_string(std::time(nullptr));
    sendReply(RPL_TOPIC(servername, nick, targetChannel, newTopic));
    sendReply(RPL_TOPICWHOTIME(servername, nick, targetChannel, 
                USER_ADDR(nick, client->getUserName(), "irc.local"), setTimeStr));
}