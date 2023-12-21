#include "../../includes/Command.hpp"
#include <map>

/*
Servers MUST allow at least all alphanumerical characters, square and curly brackets ([]{}), 
backslashes (\), and pipe (|) characters in nicknames, and MAY disallow digits as the first character. 
Servers MAY allow extra characters, as long as they do not introduce ambiguity in other commands, 
including:
	no leading # character or other character advertized in CHANTYPES -> 어디에 오든 에러남
	no leading colon (:) -> 파싱 시에 첫번째 ':'는 허용하는 것 같음, 그 외 ':'는 어디에 오든 에러남 
	no ASCII space -> 파싱 시에 스페이스 기준으로 잘라서 첫 번째 값만 닉네임으로 들어오게 됨
*/
static bool isValidNickname(const std::string nickname) {
	// 영어 대소문자, 숫자, [], {}, 백슬래쉬, 파이프
	std::string validNicknameChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789[]{}\\|";

	// 첫문자가 숫자인지 체크
	if (std::isdigit(nickname[0])) {
        return false;
    }

	// 사용 불가능한 문자가 있는지 체크
	for (std::string::const_iterator it = nickname.begin(); it != nickname.end(); ++it) {
        if (validNicknameChars.find(*it) == std::string::npos) {
            return false;  // Character not allowed
        }
    }
	return true;
}

// 닉네임 중복 확인
static bool isNicknameAlreadyInUse(const int client_fd, const std::string nickname, const std::map<int, Client> clients) {
	std::map<int, Client>::const_iterator it = clients.begin();	
	for( ; it != clients.end(); it++) {
		if (it->first == client_fd)
			continue;
		if (it->second.getNickname() == nickname) {
			return true;
		}
	}
	return false;
}

/* NICK <nickname>
 * used to give user a nickname or change the existing one.
 * Numeric Replies: ERR_NONICKNAMEGIVEN (431)         
 *					ERR_ERRONEUSNICKNAME (432)  
 *					ERR_NICKNAMEINUSE (433)       
 *					ERR_NICKCOLLISION (436)
 *    				ERR_UNAVAILRESOURCE             
 *					ERR_RESTRICTED
 * 유효하지 않은 닉네임일 경우, 432 보낸 후 커멘드 무시
 * 닉네임 중복이 있을 경우, 433 보낸 후 커멘드 무시
 * 닉네임 파라미터 못받았을 경우, 431 보낸 후 커멘드 무시
 * 닉네임 변경 성공 시, <old nickname>!<user>@localhost NICK <new nickname> 출력 ex) oldhio!root@127.0.0.1 NICK newhio
 */
void Command::nick() {
	std::string servername = "irc.local";
	std::string nick = client->getNickname();

	if (tokens.size() < 2) {
		this->sendReply(ERR_NONICKNAMEGIVEN(servername, nick));
		return;
	}

	std::string newNickname = tokens[1];
	const std::map<int, Client> clients; // = client->server.clientList

	if (newNickname == "") {
		this->sendReply(ERR_NONICKNAMEGIVEN(servername, nick));
		return;
	} 
	// 유효한 비밀번호인지 확인
	if (!isValidNickname(newNickname)) { 
		this->sendReply(ERR_ERRONEUSNICKNAME(servername, nick, newNickname));
		return;
	} 
	// 중복확인
	if (isNicknameAlreadyInUse(client->getClientFd(), newNickname, clients)) {
		this->sendReply(ERR_NICKNAMEINUSE(servername, nick, newNickname));
		return;
	} 
	
	client->setNickname(newNickname);

	// ERR_NICKCOLLISION (436) - 변경 중 서버와 충돌이 발생했을 때...?
}
