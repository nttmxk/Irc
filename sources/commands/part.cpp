#include "../../includes/Command.hpp"

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
void Command::part() {

}