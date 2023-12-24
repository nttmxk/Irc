#include "../../includes/Command.hpp"

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

static void sendToChannel(std::string message, Channel* channel) {
	std::map<int, Client*> members = channel->getMembers();
	std::map<int, Client*>::iterator it = members.begin();
	while (it != members.end())
	{
		sendByFd(it->first, message);
		it++;
	}
}

void sendByFd(int fd, std::string message)
{
	if (send(fd, message.c_str(), message.length(), 0) < 0) {
		exit(-1);
	}
}

/* PRIVMSG <target>{,<target>} <text to be sent>
	 * 사용자 간에 개인 메시지를 보내거나 채널에 메시지를 보냄. 
	 * - <target>{,<target>}: 메시지를 보낼 대상, 클라이언트의 닉네임 또는 채널의 이름. 여러 대상 가능
	 * - <text to be sent>: 전송할 텍스트 메시지
	 * 
	 * - 만약 <target>이 채널 이름이고 클라이언트가 차단되어 있고 차단 예외로 처리되지 않은 경우, 메시지는 전달되지 않고 명령은 조용히 실패한다.
	 * - 적절한 채널 모드가 활성화된 채널은 특정 사용자의 메시지를 차단할 수 있다. 다른 채널 모드는 메시지의 전달이나 전달 전에 메시지를 수정할 수 있다. 이러한 모드는 서버 소프트웨어와 사용 중인 구성에 따라 정의된다.
	 * - 만약 메시지를 채널로 전달할 수 없는 경우, 서버는 ERR_CANNOTSENDTOCHAN (404) 숫자로 응답하여 이 메시지가 전달되지 못했음을 사용자에게 알려야 한다.
	 * - <target>이 채널 이름인 경우, 하나 이상의 채널 멤버십 접두사 문자(@, + 등)로 접두사를 붙일 수 있으며 이 메시지는 해당 채널의 멤버 중에서 지정된 또는 그보다 높은 상태를 가진 멤버에게만 전달된다. 이 기능을 지원하는 서버는 STATUSMSG RPL_ISUPPORT 파라미터에 지원되는 접두사를 나열하며 클라이언트가 이 토큰에서 접두사가 광고된 경우에만 시도해야 한다.
	 * - <target>이 사용자이고 해당 사용자가 자리를 비웠으면 서버는 RPL_AWAY (301) 숫자로 응답하고 명령은 계속 진행될 것이다.
	 * - PRIVMSG 메시지는 서버에서 클라이언트로 메시지를 전달하기 위한 것이다. 메시지의 <source>는 메시지를 보낸 사용자 또는 서버를 나타내고, <target>은 PRIVMSG의 대상을 나타낸다(클라이언트, 채널 등).
	 * - PRIVMSG 메시지가 서버에서 클라이언트로 전송되고 <target>이 달러 기호('$', 0x24)로 시작하면 해당 메시지는 하나 이상의 서버의 모든 클라이언트에게 브로드캐스트되는 메시지이다.
	 * 
	 * Numeric Replies:
	    *	- ERR_NOSUCHNICK (401): 대상 닉네임이 존재하지 않아 실패함.
	    *	- ERR_CANNOTSENDTOCHAN (404): 채널에 메시지를 전달할 수 없어 실패함.
	    *  	- ERR_NORECIPIENT (411): 메시지가 없어 실패함.
	    *	- ERR_NOTEXTTOSEND (412): 전송할 텍스트 메시지가 없어 실패함.
	 *	- ERR_NOTOPLEVEL (413): 대상이 최상위 도메인이 아니어서 실패함.
	 *	- ERR_WILDTOPLEVEL (414): 대상이 와일드카드 최상위 도메인이 아니어서 실패함.
	 *	- RPL_AWAY (301): 대상 사용자가 자리를 비웠음을 나타내는 숫자 응답.
	 * 
	 * Command Examples:
	 *	- PRIVMSG Angel :yes I'm receiving it !		; Angel에게 메시지를 보내는 명령어
	 *	- PRIVMSG %#bunny :Hi! I have a problem!	; 반반 오프와 채널 오프에게 메시지를 보내는 명령어
	 *	- PRIVMSG @%#bunny :Hi! I have a problem!	; 반반 오프와 채널 오프에게 메시지를 보내는 명령어. 이 명령어는 기능적으로 위의 명령어와 동일하다. 
	 * Message Examples:
	 *	- :Angel PRIVMSG Wiz :Hello are you receiving this message ?	; Angel에서 Wiz로의 메시지
	 *	- :dan!~h@localhost PRIVMSG #coolpeople :Hi everyone!			; dan에서 채널 #coolpeople로의 메시지
	 */

// PRIVMSG <targe> :<text to be sent> 
// target과 같은 (client or channel) 찾아서 message 보내기
void Command::privmsg(std::map<int, Client*> clients, std::map<std::string, Channel*> channels) {
	std::string servername = "irc.local";
    int numParam = getNumParameter();
	std::string nick = client->getNickname();
	// messageIndex += numParam + 1;
    
    if (numParam == 3) {
        std::string target = tokens[messageIndex + 1];
        std::string message = tokens[messageIndex + 2];
        if (target[0] == '#')
        {
            Channel* targetChannel = findChannel(target, channels);
            if (!targetChannel) {
                this->sendReply(ERR_CANNOTSENDTOCHAN(servername, nick, target));
                return;
            }
            // message를 <target>channels 안의 모든 client에게 전달
            sendToChannel(message, targetChannel);
        }
        else
        {
            // send target client
		    std::vector<std::string> targetClientList = splitByComma(target);
			std::vector<std::string>::iterator it = targetClientList.begin();
			for ( ; it != targetClientList.end(); it++) {
				std::string ItTarget = *it;
            	Client* targetClient = findClient(ItTarget, clients);
            	if (!targetClient) {
					this->sendReply(ERR_NOSUCHNICK(servername, ItTarget));
				} else {
					sendByFd(targetClient->getClientFd(), message);
				}
			}
        }
    }
    else {
        // PRIVMSG targetNick (message 없을 시)
		if (numParam == 2) {
			this->sendReply(ERR_NOTEXTTOSEND(servername, nick));
			return;
		}
        // PRIVMSG targetNick aaa aaa :msg (토큰 많이 들어 올 시)
        return;
    }
}

