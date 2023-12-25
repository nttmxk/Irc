#include "../../includes/Command.hpp"

/* INVITE <nickname> <channel>
* 사용자를 채널로 초대 
* - <nickname>: 채널로 초대할 사용자의 닉네임
* - <channel>: 사용자를 초대할 대상 채널의 이름
* 
* 초대가 성공하면 서버는 명령을 실행한 사용자에게 (341) 숫자를 보내고, 
* 대상 사용자에게는 <source>로 명령을 실행한 사용자가 있는 INVITE 메시지를 보내야 한다. 다른 채널 멤버에게는 알림을 보내면 안 된다.
* 
* Numeric Replies:
*	- RPL_INVITING (341): 초대가 성공적으로 전송됨.
*	- ERR_NEEDMOREPARAMS (461): 필요한 매개변수가 부족하여 명령어를 처리할 수 없음.
*	- ERR_NOSUCHCHANNEL (403): 대상 채널이 존재하지 않아 실패함. 채널에 적어도 한 명의 클라이언트가 있어야 함
*	- ERR_NOTONCHANNEL (442): 초대를 보낸 클라이언트가 채널에 참여하지 않아 실패함.
*	- ERR_CHANOPRIVSNEEDED (482): 채널이 초대 전용 모드로 설정되어 있고 클라이언트가 적절한 채널 권한이 없어 실패함.
*	- ERR_USERONCHANNEL (443): 초대받은 사용자가 이미 대상 채널에 있어 실패함.
* 커멘드 사용 예시 
*	- INVITE Wiz #foo_bar    ; Wiz를 #foo_bar 채널로 초대.
* Message Examples:
*	- :dan-!d@localhost INVITE Wiz #test    ; dan-이 Wiz를 #test 채널로 초대함.
*/
void Command::invite(std::map<int, Client*> &clientsInServer, std::map<std::string, Channel*> &channelsInServer) {
	if (client->getFlag() != _connect)
		return;
    int numParam = getNumParameter();
	std::string servername = "irc.local";
	std::string nick = client->getNickname();

	if (numParam < 3) {
		messageIndex += numParam + 1;
		sendReply(ERR_NEEDMOREPARAMS(servername, nick, "INVITE"));
		return;
	}

    std::string targetNick = tokens[messageIndex + 1];
    std::string targetChannel = tokens[messageIndex + 2];
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

    // 채널이 초대 전용 모드일 경우, 클라이언트가 초대권한이 있는지 확인
    if (channelPtr->isModeOn('i') && !channelPtr->isOperator(nick)) {
        sendReply(ERR_CHANOPRIVSNEEDED(servername, nick, targetChannel));
		return;
    }

    // 타겟 유저가 채널에 이미 참여하고 있는지 확인
    if (channelPtr->isInChannel(targetNick)) {
        sendReply(ERR_USERONCHANNEL(servername, nick, targetNick, targetChannel));
		return;
    }

    sendReply(RPL_INVITING(servername, nick, targetChannel, targetNick));
    std::string inviteMsg = USER_ADDR(nick, client->getUserName(), "irc.local") \
                                + " invite :" + targetChannel = "\r\n";

	Client* clientPtr = findClientByNick(clientsInServer, targetNick);
	if (clientPtr == nullptr)
		return;
//	sendByFd(clientPtr->getClientFd(), inviteMsg);
}
