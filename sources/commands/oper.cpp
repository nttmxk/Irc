#include "../../includes/Command.hpp"
#include <map>

// 서버 클라이언트 리스트에서 타겟 유저 찾아서 객체 포인터를 리턴, 없으면 NULL 리턴
static Client* searchByNickname(std::string nickname, std::map<int, Client*> clients) {
	std::map<int, Client*>::iterator it = clients.begin();
	for( ; it != clients.end(); it++) {
		if (it->second->getNickname() == nickname) {
			return it->second;
		}
	}
	return NULL;
}

// OPER 파라미터로 받아온 타겟 닉네임이 유효한지 확인
// 서버 클라이언트 리스트에 속해 있으면, 유효한 호스트에 연결되어 있음을 의미
static bool isValidName(const Client* target) {
	return target == NULL ? false : true;
}

// OPER 파라미터로 받아온 비밀번호가 유효한지 확인
// OPER를 실행한 클라이언트가 서버운영자이고, 
// 입력받은 비밀번호가 해당 클라이언트의 비밀번호와 일치할 경우 유효
static bool isValidPassword(const Client* client, const std::string password, const std::string pwd) {
	if (client == NULL)
		return false;

	// 클라이언트가 서버 운영자인지 확인
	// 클라이언트의 유저 모드에 'O'가 있으면 운영자
	if (client->isServerOper() == false)
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
void Command::oper(std::map<int, Client*> &clientsInServer, const std::string pwd) {
	int numParam = getNumParameter();
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
	Client* target = searchByNickname(targetNick, clientsInServer);

	if (isValidName(target) == false) {
		this->sendReply(ERR_NOOPERHOST(servername, nick));
		return;
	}
	
	if (isValidPassword(client, password, pwd) == false) {
		this->sendReply(ERR_PASSWDMISMATCH(servername, nick));
		return;
	}

	// 타켓 유저를 서버 운영자로 만들기
	target->setServerOper();
	this->sendReply(RPL_YOUREOPER(servername, nick));

}