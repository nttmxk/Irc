#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "../includes/NumericReply.hpp"
# include "../includes/Client.hpp"
# include "../includes/Channel.hpp"

# include <string>
# include <vector>
# include <map>
# include <iostream> // for testing
# define CRLF "\r\n"

class Channel;

# define COMMAND_CNT 16

enum CommandType {
	CAP = 0,
	PASS,
	NICK,
	USER,
	cOPER,
	JOIN,
	PART,
	INVITE,
	KICK,
	QUIT,
	cTOPIC,
	MODE,
	NOTICE,
	PRIVMSG,
	PING,
	WHO
};

static const std::string commandTypeStr[COMMAND_CNT] = {"CAP","PASS", "NICK", "USER", "OPER", "JOIN", "PART",
											  "INVITE", "KICK", "QUIT", "TOPIC", "MODE", "NOTICE", "PRIVMSG", "PING", "WHO"};

class Command {
private:
	Client*						client;
	std::string					message;
	std::vector<std::string>	tokens;
	int 						messageIndex;
	int 						tokenSize;

//	Command();
//	Command(const Command& abj);
//	~Command();

public:
	Command(Client * _client, const std::string _message);
	int		getCommandType();
	bool	isTokenEnd();
	bool	isConnectEnd;
	int		getNumParameter();
	void	passCommand();

private:
	void parseMessage();
	void sendReply(std::string);
	int	getParamsCnt();
	std::vector<std::string> splitByComma(std::string str);
	Channel* isChannelExist(std::map<std::string,Channel*> &channelsInServer, std::string channelName);

public:

/* Connection Registration */
	void pass(const std::string pwd);
	void nick(std::map<int, Client*> &clients);
	void user(std::string time);
	void oper(std::map<int, Client*> &clientsInServer, const std::string pwd);
	void quit(std::map<std::string, Channel*> &channelsInServer);
	void mode(std::map<std::string, Channel*> &channelsInServer);
	void ping();

/* Channel Operations */
	void join(std::map<std::string,Channel*> &channelsInServer);
	void who(std::map<std::string,Channel*> &channelsInServer);
	void part(std::map<std::string, Channel*> &channelsInServer);
	void topic(std::map<std::string, Channel*> &channelsInServer);
	void invite(std::map<std::string,Channel*> &channelsInServer);
	void kick(std::map<int, Client*> &clientsInServer, std::map<std::string, Channel*> &channelsInServer);

/* Server Queries and Commands */
	// https://modern.ircdocs.horse/#mode-message 내용이 너무 많아서... 링크 달아둘게요
	/* MODE <target> [<modestring> [<mode arguments>...]]
	 * 주어진 대상에 대한 옵션(또는 모드)을 설정하거나 제거
	 * - <target>: 모드를 설정 또는 제거할 대상
	 * - [<modestring> [<mode arguments>...]]: 모드 문자열이 주어지면 제공된 모드가 적용되고, 사용자에게 변경된 모드를 포함하는 MODE 메시지가 전송된다. 
	 * 
	 * modestring은 플러스('+', 0x2B) 또는 마이너스('-', 0x2D) 문자로 시작하고 다음 문자로 구성된다.
	 *	- '+': 다음 모드(들)를 추가한다.
	 *	- '-': 다음 모드(들)를 제거한다.
	 *	- 'a-zA-Z': 모드 문자로, 추가/제거할 모드를 나타낸다.
	 * 
	 * Numeric Replies:
	 * 	- ERR_NOSUCHNICK (401): 주어진 닉네임이 네트워크에 존재하지 않을 때 반환된다.
	 * 	- ERR_USERSDONTMATCH (502): <target>이 다른 사용자의 닉네임일 때 명령을 보낸 사용자와 일치하지 않을 때 반환된다.
	 * 	- ERR_NOSUCHCHANNEL (403): 주어진 채널이 네트워크에 존재하지 않을 때 반환된다.
	 * 	- RPL_UMODEIS (221): <modestring>이 주어지지 않았을 때, 현재 대상 사용자의 모드를 나타내는 응답으로 반환된다.
	 *  - RPL_CHANNELMODEIS (324): <modestring>이 주어지지 않았을 때, 현재 대상 채널의 모드를 나타내는 응답으로 반환된다.
	 * 	- ERR_CHANOPRIVSNEEDED (482): 사용자가 대상 채널에서 모드를 변경할 적절한 채널 권한이 없을 때 반환된다.
	 * 	- ERR_UMODEUNKNOWNFLAG (501): <modestring>에 포함된 하나 이상의 모드가 서버에서 구현되지 않았을 때, 서버는 구현된 모드를 적용한 다음 ERR_UMODEUNKNOWNFLAG (501)을 응답으로 반환한다.
	 * 	- RPL_CREATIONTIME (329): <modestring>이 주어지지 않았을 때, 서버는 RPL_CHANNELMODEIS에 이어서 채널의 생성 시간을 반환할 수 있다.
	 * 
	 * Command Examples:
	 * 	- MODE dan +i                     ; dan에게 "invisible" 사용자 모드 설정
	 * 	- MODE #foobar +mb *@127.0.0.1    ; "#foobar" 채널에 "moderated" 채널 모드 설정 및 ban 목록에 "*@127.0.0.1" 마스크 추가
	 * Message Examples:
	 * 	- :dan!~h@localhost MODE #foobar -bl+i *@192.168.0.1
	 *                                   ; dan은 "*@192.168.0.1" 마스크를 ban 해제하고, 채널 "#foobar"의 클라이언트 제한을 제거하고, 채널을 초대 전용으로 설정했다.
	 * 	- :irc.example.com MODE #foobar +o bunny
	 *                                   ; irc.example.com 서버가 "#foobar" 채널에 bunny에게 채널 오퍼레이터 권한을 부여했다.
	 * Requesting modes for a channel:
	 * 	- MODE #foobar
	 * Getting modes for a channel (and channel creation time):
	 * 	- :irc.example.com 324 dan #foobar +nrt
	 * 	- :irc.example.com 329 dan #foobar 1620807422
	 */

/* Sending Messages */

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
	 *	- ERR_NOSUCHSERVER (402): 대상 서버가 존재하지 않아 실패함.
	 *	- ERR_CANNOTSENDTOCHAN (404): 채널에 메시지를 전달할 수 없어 실패함.
	 *	- ERR_TOOMANYTARGETS (407): 메시지를 전송할 대상이 너무 많아 실패함.
	 *	- ERR_NORECIPIENT (411): 메시지가 없어 실패함.
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

	void privmsg(std::vector<Client&> targets, std::string message) const;
	void privmsg(std::vector<Channel&> targets, std::string message) const;

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
	void notice(std::vector<Client&> targets, std::string message) const;
	void notice(std::vector<Channel&> targets, std::string message) const;

};

#endif