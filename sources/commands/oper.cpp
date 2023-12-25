#include "../../includes/Command.hpp"

// OPER 파라미터로 받아온 비밀번호가 유효한지 확인
// OPER를 실행한 클라이언트가 서버운영자이고, 
// 입력받은 비밀번호가 해당 클라이언트의 비밀번호와 일치할 경우 유효
static bool isValidPassword(const Client *client, const std::string password, const std::string pwd) {
	if (client == NULL)
		return false;

	// 클라이언트가 서버 운영자인지 확인
	// 클라이언트의 유저 모드에 'O'가 있으면 운영자
	if (!client->isServerOper())
		return false;

	// 클라이언트가 서버 운영자 중 한 명이면 password 유효한지 확인
	return password != pwd;
}

/* OPER <name> <password>
 * used by a normal user to obtain IRC operator privileges
 * Numeric Replies: ERR_NEEDMOREPARAMS (461)
					ERR_PASSWDMISMATCH (464)
					ERR_NOOPERHOST (491)
					RPL_YOUREOPER (381)
 * password 오류 시, 464 보낸 후 fail 처리
 * not connecting from a valid host for the given name, 491 보낸 후 fail 처리
 * 성공 시(name, password가 correct, 유효한 host에서 연결 중), 381을 user에게 보냄
 */
void Command::oper(std::map<int, Client *> &clientsInServer, const std::string pwd) {
	int numParam = getNumParameter();
	if (client->getFlag() != _connect) {
		messageIndex += numParam + 1;
		return;
	}
	std::string servername = "irc.local";
	std::string nick = client->getNickname();

	if (numParam < 3) {
		messageIndex += numParam + 1;
		this->sendReply(ERR_NEEDMOREPARAMS(servername, nick, "OPER"));
		return;
	}

	std::string targetNick = tokens[messageIndex + 1];
	std::string password = tokens[messageIndex + 2];
	messageIndex += numParam + 1;
	Client *target = findClientByNick(clientsInServer, targetNick);

	if (target == nullptr) {
		this->sendReply(ERR_NOOPERHOST(servername, nick));
		return;
	}

	if (!isValidPassword(client, password, pwd)) {
		this->sendReply(ERR_PASSWDMISMATCH(servername, nick));
		return;
	}

	// 타켓 유저를 서버 운영자로 만들기
	target->setServerOper(true);
	sendByFD(target->getClientFd(), RPL_YOUREOPER(servername, targetNick));
}