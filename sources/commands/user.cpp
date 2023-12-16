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
 * Ident 프로토콜을 사용하여 이름을 이미 수신했다면, 명령 무시..?
 * Ident 프로토콜에서 사용자 이름이 없는 경우, 등록하고 username 앞에 ~ 추가..?
 * Example: USER amy 0 * :Amy Ponds
 */
void Command::user(Client& client, std::string userName, std::string realName) const {
	if (userName == "" || realName == "") {
		// ERR_NEEDMOREPARAMS (461)
	} else if (client.isAuthorized()) {
		// ERR_ALREADYREGISTRED (462)
	} else {
		client.setUserName(userName);
		client.setRealName(realName);
		client.checkAuthorization();
	}
}