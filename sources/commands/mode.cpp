#include "../../includes/Command.hpp"

void Command::mode(std::map<std::string, Channel *> &channelsInServer) {
	int numParam = getNumParameter();
	if (client->getFlag() != _connect) {
		messageIndex += numParam + 1;
		return;
	}
	std::string channelName;
	std::string modeString;
	std::string modeArguments[3];
	std::string servername = "irc.local";
	std::string nick = client->getNickname();

	if (numParam < 2) {
		sendReply(ERR_NEEDMOREPARAMS(servername, nick, "MODE"));
		messageIndex += numParam + 1;
		return;
	}
	channelName = tokens[messageIndex + 1];
	modeString = tokens[messageIndex + 2];

//	std::clog << "[Log] mode:" << channelName << "," << modeString << '\n';

	if (numParam > 6) {
		sendReply(":irc.local " + nick + channelName + " :Too many parameters\r\n"); // Custom?
		messageIndex += numParam + 1;
		return;
	}
	if ((modeString.find_first_of("kol")) != std::string::npos) {
		for (int i = 0; i < numParam - 3; ++i)
			modeArguments[i] = tokens[messageIndex + 3 + i];
	}
	messageIndex += numParam + 1;

	if (channelName[0] != '#')
		return;
	if (channelsInServer.find(channelName) == channelsInServer.end()) {
		sendReply(ERR_NOSUCHCHANNEL(servername, nick, channelName));
		return;
	}
	Channel *channel = channelsInServer[channelName];
	int argIndex = 0;

	if (numParam == 2) {
		sendReply(RPL_CHANNELMODEIS(servername, nick, channelName, channel->getChannelMode()));
		return;
	}
	if (!channel->isOperator(nick)) {
		sendReply(ERR_CHANOPRIVSNEEDED(servername, nick, channelName));
		return;
	}

	if (modeString[0] == '+') {
		for (int i = 1; modeString[i]; ++i) {
			if (modeString[i] == 'i')
				channel->onMode(INVITE_ONLY);
			else if (modeString[i] == 't')
				channel->onMode(TOPIC);
			else if (modeString[i] == 'k') {
				if (modeArguments[argIndex] == "") {
					sendReply(ERR_NEEDMOREPARAMS(servername, nick, "MODE"));
					return;
				}
				if (channel->isOperator(nick)) {
					channel->onMode(KEY);
					channel->setKey(modeArguments[argIndex++]);
				}
			} else if (modeString[i] == 'o') {
				if (modeArguments[argIndex] == "") {
					sendReply(ERR_NEEDMOREPARAMS(servername, nick, "MODE"));
					return;
				}
				std::string targetName = modeArguments[argIndex++];
				channel->addOperator(targetName);
			} else if (modeString[i] == 'l') {
				if (modeArguments[argIndex] == "") {
					sendReply(ERR_NEEDMOREPARAMS(servername, nick, "MODE"));
					return;
				}
				int n = stoi(modeArguments[argIndex++]);
				channel->onMode(USER_LIMIT);
				channel->setMemberLimit(n);
			}
		}
	} else if (modeString[0] == '-') {
		for (int i = 1; modeString[i]; ++i) {
			if (modeString[i] == 'i')
				channel->offMode(INVITE_ONLY);
			else if (modeString[i] == 't') {
				channel->offMode(TOPIC);
				channel->setTopic("");
			}
			else if (modeString[i] == 'k') {
				if (channel->isOperator(nick)) {
					channel->offMode(KEY);
					channel->setKey("");
				}
			} else if (modeString[i] == 'o') {
				if (modeArguments[argIndex] == "") {
					sendReply(ERR_NEEDMOREPARAMS(servername, nick, "MODE"));
					return;
				}
				std::string targetName = modeArguments[argIndex++];
				channel->addNormalMember(targetName);
			} else if (modeString[i] == 'l') {
				channel->offMode(USER_LIMIT);
				channel->setMemberLimit(atoi("-1")); // memberMax
			}
		}
	}
}