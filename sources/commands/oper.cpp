#include "../../includes/Command.hpp"

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
void Command::oper() {
		
}