#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

/* PASS <password>
 * used to set a 'connection password'
 * Numeric Replies: ERR_NEEDMOREPARAMS (461)
 * 					ERR_ALREADYREGISTRED (462)
 * 					ERR_PASSWDMISMATCH (464)
 * 서버에서 예상한 비밀번호가 아니면, (464)를 보낸 후 ERROR와 함께 연결 닫기
 */
void Command::pass(Client& client, const std::string password) const {
	if (password == "") {
		// ERR_NEEDMOREPARAMS (461)
	} else if (client.isAuthorized()) {
		// ERR_ALREADYREGISTRED (462)
	} else if (client.getPassword() != password) {
		// ERR_PASSWDMISMATCH (464)
	} else {
		client.setPassword(password);
	}
	
} 