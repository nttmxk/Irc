#include "../../includes/Command.hpp"


void Command::ping() {
	int numParam = getNumParameter();
	const std::string servername = "irc.local";
	const std::string nick = this->client->getNickname();

	std::string ip = this->tokens[messageIndex + 1];
	messageIndex += numParam + 1;

	if (numParam < 2) {
		this->sendReply(ERR_NEEDMOREPARAMS(servername, nick, "PING"));
		return;
	}

	if (this->client->isAuthorized())
		this->sendReply(RPL_PONG(servername, ip));
} 