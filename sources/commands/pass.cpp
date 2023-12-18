#include "../../includes/Command.hpp"

/* PASS <password>
 * used to set a 'connection password'
 * 비밀번호가 설정되어 있다면 연결을 등록하기 전에 반드시 해당 비밀번호를 제공해야 함
 * 클라이언트 연결 과정에서 PASS 명령어가 NICK/USER 명령어보다 선행되어야 함
 * Numeric Replies: ERR_NEEDMOREPARAMS (461) : PASS 명령어에 필요한 매개변수가 누락되었음
 * 					ERR_ALREADYREGISTRED (462) : 클라이언트가 이미 등록되어 있는 상태
 * 					ERR_PASSWDMISMATCH (464) : 제공된 비밀번호와 서버에서 기대하는 비밀번호 간의 불일치
 * 서버에서 예상한 비밀번호가 아니면, (464)를 보낸 후 ERROR와 함께 연결 닫기
 */
void Command::pass() {

	const std::string servername = "irc.local";
	const std::string nick = this->client->getNickname();

	if (this->tokens.size() < 2) {
		this->sendReply(ERR_NEEDMOREPARAMS(servername, nick, "PASS"));
		return;
	}

	std::string password = this->tokens[1];
	if (this->client->isAuthorized()) {
		this->sendReply(ERR_ALREADYREGISTRED(servername, nick));
		return;
	} else if (this->client->getPassword() != password) {
		this->sendReply(ERR_PASSWDMISMATCH(servername, nick));
		return;
	} else {
		this->client->setPassword(password);
	}
	
} 