#include "../../includes/Command.hpp"

void Command::mode(std::map<int, Channel> channelsInServer) {
	int numParam = getNumParameter();
	std::string	channelName;
	std::string modeString;
	std::string modeArguments[3];

	if (numParam < 3)
	{
		sendReply(ERR_NOSUCHCHANNEL("irc.local", client->getNickname(), channelName));
		messageIndex += numParam + 1;
		return;
	}
	channelName = tokens[messageIndex + 1];
	modeString = tokens[messageIndex + 2];
	if (numParam > 6)
	{
		sendReply(":irc.local " + client->getNickname() + channelName + " :Too many parameters\r\n"); // Custom?
		messageIndex += numParam + 1;
		return;
	}
	if ((modeString.find_first_of("kol")) != std::string::npos)
	{
		for (int i = 0; i < numParam - 3; ++i)
			modeArguments[i] = tokens[messageIndex + 3 + i];
	}
	messageIndex += numParam + 1;

	if (channelName[0] != '#')
		return;
	if (channelsInServer.find(channelName) == channelsInServer.end())
	{
		sendReply(ERR_NOSUCHCHANNEL("irc.local", client->getNickname(), channelName));
		return;
	}
	Channel channel = channelsInServer[channelName];
	int argIndex = 0;

	if (modeString[0] == '+')
	{
		for (int i = 1; modeString[i]; ++i) {
			if (modeString[i] == 'i')
				channel->onMode(INVITE_ONLY);
			else if (modeString[i] == 't')
				channel->onMode(TOPIC);
			else if (modeString[i] == 'k')
			{
				if (channel->isOperator(client->getNickname()))
				{
					channel->onMode(KEY);
					channel->setKey(modeArguments[argIndex++]);
				}
			}
			else if (modeString[i] == 'o')
			{
				std::string targetName = modeArguments[argIndex++];
				if (channel->isOperator(client->getNickname()))
					channel->addOperator(targetName);
			}
			else if (modeString[i] == 'l')
			{
				int n = stoi(modeArguments[argIndex++]);
				if (channel->isOperator(client->getNickname()))
					channel->setMemberLimit(n);
			}
		}
	}
	else if (modeString[0] == '-') {
		for (int i = 1; modeString[i]; ++i) {
			if (modeString[i] == 'i')
				channel->offMode(INVITE_ONLY);
			else if (modeString[i] == 't')
				channel->offMode(TOPIC);
			else if (modeString[i] == 'k')
			{
				if (channel->isOperator(client->getNickname()))
				{
					channel->offMode(KEY);
					channel->setKey("");
				}
			}
			else if (modeString[i] == 'o')
			{
				std::string targetName = modeArguments[argIndex++];
				if (channel->isOperator(client->getNickname()))
					channel->deleteOperator(targetName);
			}
			else if (modeString[i] == 'l')
			{
				if (channel->isOperator(client->getNickname()))
					channel->setMemberLimit("4242"); // memberMax
			}
		}
	}
}