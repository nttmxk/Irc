#include "../../includes/Command.hpp"

/* NOTICE <target>{,<target>} <text to be sent>
	* 사용자 간에 또는 채널에 노티스를 보냄. 
	* - <target>{,<target>}: 노티스를 보낼 대상. <target>은 클라이언트의 닉네임이거나 채널의 이름, 여러 대상에게 가능
	* - <text to be sent>: 전송할 텍스트 노티스
	* 
	* - NOTICE 메시지는 PRIVMSG와 유사하게 사용된다. 
	* 	 NOTICE와 PRIVMSG의 차이점은 NOTICE 메시지에 대한 자동 응답을 절대로 보내면 안 된다는 것이다. 
	* 	 이 규칙은 서버에도 적용되며, NOTICE 명령을 수신할 때 클라이언트에게 에러를 보내면 안 된다. 
	* 	 이것의 의도는 클라이언트가 수신한 것에 대한 자동 응답을 보내는 것을 피하기 위한 것이다.
	* 	 이것은 일반적으로 '봇'(프로그램이 제어하는 사용자가 아닌 클라이언트) 및 서버 메시지에서 클라이언트로의 루프를 피하기 위한 것이다.
	* - 봇 작성자들에게 유의해야 할 점은 NOTICE 메시지가 여러 클라이언트에서 다르게 해석될 수 있다는 것이다. 
	* 	 일부 클라이언트는 채널에 보내진 NOTICE를 자신의 닉네임과 함께 PRIVMSG로 처리하는 것처럼 강조하거나 해석할 수 있다. 
	* 	 이로 인해 사용자는 클라이언트 또는 봇이 PRIVMSG 메시지 대신 NOTICE 메시지를 사용하면 계속해서 귀찮아질 수 있으며, 
	* 	 이러한 이유로 NOTICE 메시지는 일반적으로 클라이언트 봇에서 사용되지 않는다.
	* 
	* Numeric Replies: 없음 
	* Command Examples:
	*	- NOTICE Angel :This is a notice!			; Angel에게 노티스를 보내는 명령어
	*	- NOTICE %#bunny :Hi! This is a notice!		; 반반 오프와 채널 오프에게 노티스를 보내는 명령어
	*	- NOTICE @%#bunny :Hi! This is a notice!	; 반반 오프와 채널 오프에게 노티스를 보내는 명령어. 이 명령어는 기능적으로 위의 명령어와 동일하다.
	* Message Examples:
	*	- :Angel NOTICE Wiz :Hello, this is a notice!							; Angel에서 Wiz로의 노티스
	*	- :dan!~h@localhost NOTICE #coolpeople :Hi everyone, this is a notice!	; dan에서 채널 #coolpeople로의 노티스
	*/

// target과 같은 이름인 channel 찾아서 리턴하는 함수
static Channel* findChannel(const std::string target, std::map<std::string, Channel*> channels) {
    std::map<std::string, Channel*>::const_iterator it = channels.begin();
    for( ; it != channels.end(); it++) {
        if (it->second->getName() == target) {
            return (it->second);
        }
    }
    return NULL;
}

// target과 같은 이름인 client 찾아서 리턴하는 함수 
static Client* findClient(const std::string target, const std::map<int, Client*> clients) {
	std::map<int, Client*>::const_iterator it = clients.begin();	
	for( ; it != clients.end(); it++) {
		if (it->second->getNickname() == target) {
			return (it->second);
		}
	}
	return NULL;
}

void Command::sendToNotice(std::string nick, std::string message, Channel* channel) {
	std::string servername = "irc.local";
	std::string targetNick;

	std::map<std::string, Client*> members = channel->getMembers();
	std::map<std::string, Client*>::iterator it = members.begin();
	for ( ; it != members.end(); it++)
	{
		targetNick = it->second->getNickname();
		if (targetNick == nick)
			continue;
		if (send(it->second->getClientFd(), message.c_str(), message.length(), 0) < 0)
			exit(-1);
	}
}

void Command::notice(std::map<int, Client*> clients, std::map<std::string, Channel*> channels) {
	std::string servername = "irc.local";
    int numParam = getNumParameter();
	std::string nick = client->getNickname();

    if (numParam == 3) {
        std::string target = tokens[messageIndex + 1];
        std::string message = tokens[messageIndex + 2];
		messageIndex += numParam + 1;

        if (target[0] == '#')
        {
            Channel* targetChannel = findChannel(target, channels);
            if (!targetChannel) {
                this->sendReply(ERR_CANNOTSENDTOCHAN(servername, nick, target));
                return;
            }
			std::string response = RPL_NOTICE(nick, nick, servername, target, message);
            sendToChannel(nick, response, targetChannel);
        }
        else
        {
		    std::vector<std::string> targetClientList = splitByComma(target);
			std::vector<std::string>::iterator it = targetClientList.begin();
			for ( ; it != targetClientList.end(); it++) {
				std::string ItTarget = *it;
            	Client* targetClient = findClient(ItTarget, clients);
            	if (!targetClient) {
					this->sendReply(ERR_NOSUCHNICK(servername, ItTarget));
				} else {
					std::string targetNick = targetClient->getNickname();
					std::string response = RPL_NOTICE(nick, nick, servername, targetNick, message);
					if (send(targetClient->getClientFd(), response.c_str(), response.length(), 0) < 0)
							exit(-1);
				}
			}
        }
    }
    else {
		messageIndex += numParam + 1;
		if (numParam == 2) {
			this->sendReply(ERR_NOTEXTTOSEND(servername, nick));
			return;
		}
        return;
    }
}