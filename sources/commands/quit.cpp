#include "../../includes/Command.hpp"

/* QUIT [<reason>]
 * A client session is terminated with a quit message.  
 * The server acknowledges this by sending an ERROR message to the client.
 * Example: QUIT :Gone to have lunch 
 *		:syrk!kalt@millennium.stealth.net QUIT :Gone to have lunch 
 *		; User syrk has quit IRC to have lunch. ( --> Preferred message format )
 * client가 서버 접속을 끊음, client가 접속해 있는 모든 채널에서 나감
 */
void Command::quit() {

	std::string quitMsg = "Quit: ";
	quitMsg += (tokens.size() == 1) ? "leaving" : tokens[1];

	// 참여한 채널에 quitMsg 보내고 나가기

	// 서버에서 나가기
}
