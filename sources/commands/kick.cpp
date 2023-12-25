#include "../../includes/Command.hpp"

// channel에서 client를 강제퇴장
/* KICK <channel> <user> *( "," <user> ) [<comment>]
* <user>를 <channel>에서 강제제거 (강제퇴장). 
* 만약 주석이 주어지지 않으면 서버는 대신 기본 메시지를 사용해야 한다.
* - <channel>: 사용자를 강제로 제거할 채널
* - <user>: 강제로 제거될 사용자의 닉네임
* - *( "," <user> ): 추가적인 사용자를 나열하는데 사용됨
* - [<comment>]: 강제로 제거의 이유를 설명하는 주석. 생략가능
* 
* - 서버는 KICK 메시지를 여러 사용자에게 보내서는 안 된다. 이는 기존 클라이언트 소프트웨어와의 하위 호환성을 유지하기 위해 필요하다.
* - 서버는 RPL_ISUPPORT 의 TARGMAX 매개변수를 통해 KICK 명령당 대상 사용자 수를 제한할 수 있으며, 대상 수가 제한을 초과하는 경우 무시해야 한다.
* 
* Numeric Replies:
*	- ERR_NEEDMOREPARAMS (461): 필요한 매개변수가 부족하여 명령어를 처리할 수 없음.
*	- ERR_NOSUCHCHANNEL (403): 대상 채널이 존재하지 않아 실패함.
*	- ERR_CHANOPRIVSNEEDED (482): 클라이언트가 적절한 채널 권한이 없어 실패함.
*	- ERR_USERNOTINCHANNEL (441): 대상 사용자가 채널에 존재하지 않아 실패함.
*	- ERR_NOTONCHANNEL (442): 클라이언트가 지정된 채널에 참여하지 않아 실패함.
* Deprecated Numeric Reply:
*	- ERR_BADCHANMASK (476): 사용되지 않는 Deprecated 숫자 응답.
* 
* Examples:
*	- KICK #Finnish Matthew        			; #Finnish에서 Matthew를 강제로 제거하는 명령어
*	- KICK &Melbourne Matthew     		    ; &Melbourne에서 Matthew를 강제로 제거하는 명령어
*	- KICK #Finnish John :Speaking English	; #Finnish에서 John을 "Speaking English"을 이유(주석)로 강제로 제거하는 명령어
* Message Examples:
*	- :WiZ!jto@tolsun.oulu.fi KICK #Finnish John
*									; WiZ가 채널 #Finnish에서 John을 제거하도록 하는 KICK 메시지
*/

void Command::kick(std::map<int, Client*> &clientsInServer, std::map<std::string, Channel*> &channelsInServer) {
    int numParam = getNumParameter();
	std::string servername = "irc.local";
	std::string nick = client->getNickname();

	if (numParam < 3) {
		messageIndex += numParam + 1;
		sendReply(ERR_NEEDMOREPARAMS(servername, nick, "KICK"));
		return;
	}

    std::string targetChannel = tokens[messageIndex + 1];
    std::string targetNicks = tokens[messageIndex + 2];
    std::string comment = (numParam > 3) ? tokens[messageIndex + 3] : "";
    messageIndex += numParam + 1;

    // 타겟 채널이 서버에 존재하는지 확인
    Channel* channelPtr = isChannelExist(channelsInServer, targetChannel);
    if (channelPtr == NULL) {
        sendReply(ERR_NOSUCHCHANNEL(servername, nick, targetChannel));
		return;
    }

    // 클라이언트가 타겟 채널에 참여하고 있는지 확인
    if (!channelPtr->isInChannel(nick)) {
        sendReply(ERR_NOTONCHANNEL(servername, nick, targetChannel));
		return;
    }

    // 클라이언트가 타겟 채널 kick 권한이 있는지 확인
    if (!channelPtr->isOperator(nick)) {
        sendReply(ERR_CHANOPRIVSNEEDED(servername, nick, targetChannel));
		return;
    }

    std::vector<std::string> targetNickList = splitByComma(targetNicks);
    std::vector<std::string>::iterator it = targetNickList.begin();
    for ( ; it != targetNickList.end(); it++) {
        std::string targetNick = *it;

        // 타겟 유저가 채널에 참여하고 있는지 확인
        if (!channelPtr->isInChannel(targetNick)) {
            sendReply(ERR_USERNOTINCHANNEL(servername, nick, targetNick, targetChannel));
            return;
        }

        std::string kickMsg = USER_ADDR(nick, client->getUserName(), "irc.local") \
                                + " KICK :" + targetChannel + " " + targetNick + "\r\n";

		Client* clientPtr = findClientByNick(clientsInServer, targetNick);
		if (clientPtr == nullptr)
			return;
		clientPtr->quitChannel(targetChannel);
//		sendToChannel(kickMsg, channelPtr);
        channelPtr->deleteMember(targetNick);
    }
}
