#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

/* USER <username> 0 * <realname>
 * used at the beginning of connection to specify the username and realname of a new user.
 * 클라이언트의 정보를 서버에 알려줌
 * Numeric Replies: ERR_NEEDMOREPARAMS (461)
 * 					ERR_ALREADYREGISTRED (462)
 * realname에는 공백문자가 포함될 수 있고, realname 바로 앞에 ':'을 붙여야 함
 * username의 최대길이는 USERLEN RPL_ISUPPORT 파라미터로 정할 수 있고, 최대길이가 정해지면 그 길이로 자동으로 잘려야 함
 * username의 최소길이는 1, 빈문자열일 경우, 461 보낸 후 기본값 사용...?
 * 이미 서버에 등록되어 있는 경우, 462 보낸 후, fail 시도...?
 * Example: USER amy 0 * :Amy Ponds
 */
void Command::user() {
	std::string servername = "irc.local";
	std::string nick = client->getNickname();

	if (tokens.size() < 5) {
		this->sendReply(ERR_NEEDMOREPARAMS(servername, nick, "USER"));
		return;
	}

	std::string userName = tokens[2];
	std::string realName = tokens[4];

	if (userName.length() < 1 || realName.length() < 1) {
		this->sendReply(ERR_NEEDMOREPARAMS(servername, nick, "USER"));
		return;
	} 
	if (client->isAuthorized()) {
		this->sendReply(ERR_ALREADYREGISTRED(servername, nick));
		return;
	}

	client->setUserName(userName);
	client->setRealName(realName);
	client->checkAuthorization();
}