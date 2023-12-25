#include "../../includes/Command.hpp"

void Command::who(std::map<std::string, Channel *> &channelsInServer) {
	int numParam = getNumParameter();
	if (client->getFlag() != _connect) {
		messageIndex += numParam + 1;
		return;
	}
	std::string servername = "irc.local";
	std::string nick = client->getNickname();

	std::string channelName = tokens[messageIndex + 1];
	messageIndex += numParam + 1;
	if (numParam < 2) {
		sendReply(ERR_NEEDMOREPARAMS(servername, nick, "WHO"));
		return;
	}
	Channel *channelPtr = isChannelExist(channelsInServer, channelName);
	if (channelPtr == NULL) {
		sendReply(ERR_NOSUCHCHANNEL(servername, nick, channelName));
		return;
	}
	std::string memberListStr = channelPtr->getMemberStr();
	sendReply(RPL_NAMREPLY(servername, nick, channelPtr->getName(), memberListStr));
	sendReply(RPL_ENDOFNAMES(servername, nick, channelPtr->getName()));
}