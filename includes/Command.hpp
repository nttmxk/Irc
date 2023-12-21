#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "../includes/NumericReply.hpp"
# include "../includes/Client.hpp"

# include <string>
# include <vector>

class Channel;
// typedef enum ChannelMode;

enum CommandType {
	PASS = 0,
	NICK,
	USER,
	COMMAND_CNT
};

static const std::string commandTypeStr[3] = {"PASS", "NICK", "USER"};

class Command {
private:
	Client*						client;
	std::string					message;
	std::vector<std::string>	tokens;

	Command();
	Command(const Command& abj);
	~Command();

public:
	Command(Client * _client, const std::string _message);

private:
	void parseMessage();
	void execute(std::string cmd);
	void sendReply(std::string);

/* Connection Registration  */
	void pass();
	void nick();
	void user();
	void oper();

	/* QUIT [<reason>]
	 * A client session is terminated with a quit message.  
	 * The server acknowledges this by sending an ERROR message to the client.
	 * Example: QUIT :Gone to have lunch 
				:syrk!kalt@millennium.stealth.net QUIT :Gone to have lunch 
				; User syrk has quit IRC to have lunch. ( --> Preferred message format )
	 * client가 서버 접속을 끊음, client가 접속해 있는 모든 채널에서 나감
	 */
	void quit(Client& Client, std::string message);


/* Channel Operations */

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
	 *	- ERR_TOOMANYCHANNELS (405): 클라이언트가 지정된 수 이상의 채널에 참여할 수 없음.
	 *	- ERR_BADCHANNELKEY (475): 채널에 설정된 키가 올바르지 않음.
	 *	- ERR_BANNEDFROMCHAN (474): 클라이언트가 채널에 참여할 권한이 없음 (밴 당함).
	 *	- ERR_CHANNELISFULL (471): 채널이 가득 찼음.
	 *	- ERR_INVITEONLYCHAN (473): 초대 전용 채널에 클라이언트가 참여하려고 시도함.
	 *	- ERR_BADCHANMASK (476): 잘못된 채널 마스크가 사용됨.
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
	void join(Client& client, std::vector<Channel&> channel);

	/* PART <channel>{,<channel>} [<reason>]
	 * client가 channel을 나감, 주어진 channel의 활성멤버 목록에서 client를 지움
	 * - <channel>: 클라이언트를 제거할 채널. 여러 채널에서 동시에 나갈 수 있으며 콤마(,)로 구분
	 * - [<reason>]: 클라이언트가 채널에서 나간 이유. 생략 가능. 
	 * 
	 * 성공적인 PART 명령을 전송하면 사용자는 제거된 각 채널에 대해 서버로부터 PART 메시지를 받게 된다. <reason>은 클라이언트가 채널에서 나간 이유를 나타낸다.
	 * This message may be sent from a server to a client to notify the client that someone has been removed from a channel. 
	 * In this case, the message <source> will be the client who is being removed, 
	 * and <channel> will be the channel which that client has been removed from. 
	 * Servers SHOULD NOT send multiple channels in this message to clients, 
	 * and SHOULD distribute these multiple-channel PART messages as a series of messages with a single channel name on each. 
	 * If a PART message is distributed in this way, <reason> (if it exists) should be on each of these messages.
 	 * 
	 * Numeric Replies:
	 *	- ERR_NEEDMOREPARAMS (461): 필요한 매개변수가 부족하여 명령어를 처리할 수 없음.
	 *	- ERR_NOSUCHCHANNEL (403): 지정된 채널이 존재하지 않아 실패함.
	 *	- ERR_NOTONCHANNEL (442): 클라이언트가 채널에 참여하지 않아 실패함. (채널은 존재함)
	 * 
	 * 커멘드 사용 예시 
	 *	- PART #twilight_zone             ; 채널 "#twilight_zone"을 나감.
	 *	- PART #oz-ops,&group5            ; 채널 "&group5"와 "#oz-ops"를 동시에 나감.
	 * Message Examples:
	 *	- :dan-!d@localhost PART #test    ; dan-이 채널 #test를 나감.
	*/
	void part(Client& client, Channel& channel, std::vector<std::string> reasons) const;

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
	void topic(Channel& channel, std::string newTopic, bool hasTopic) const;

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
	void invite(Channel& channel, Client& target) const;

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

	void kick(Channel& channel, std::vector<Client&> users) const;

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
	void mode(Channel& channel, std::string modesString) const;

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