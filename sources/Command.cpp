#include "../includes/Command.hpp"
#include <iostream> // cout

Command::Command(Client * _client, const std::string _message) 
	: client(_client), message(_message), messageIndex(0), isConnectEnd(false) {
	
	this->parseMessage();
}

void Command::parseMessage() {
	size_t		prev = 0;
    size_t		next;
	size_t		crlf;

    while (this->message[prev])
    {
		crlf = this->message.find(CRLF, prev);
		next = this->message.find(' ', prev);
		if (this->message[prev] == ':' || crlf < next)
		{
            this->tokens.push_back(this->message.substr(prev, crlf - prev));
			this->tokens.push_back(CRLF);
			prev = crlf + 2;
		}
		else
		{
			this->tokens.push_back(this->message.substr(prev, next - prev));
			prev = next + 1;
		}
    }
	tokenSize = tokens.size();
}

int Command::getCommandType()
{
	std::string cmd = tokens[messageIndex];
	int type = 0;

	for ( ; type < COMMAND_CNT; type++) {
		if (commandTypeStr[type] == cmd)
			return type;
	}
	return -1;
}

void Command::sendReply(std::string replyMsg) {
	std::string tmp = replyMsg[replyMsg.length() - 1] == '\n' ? replyMsg : replyMsg + "\n";
	std::cout << ">> Sending message to client: " << tmp;
	if (send(client->getClientFd(), tmp.c_str(), tmp.length(), 0) == -1)
		exit(1);
}

bool Command::isTokenEnd()
{
	if (messageIndex < tokenSize)
		return false;
	return true;
}

void Command::passCommand()
{
	messageIndex += getNumParameter() + 1;
}

int Command::getNumParameter()
{
	int	num = 0;

	while (tokens[messageIndex + num] != CRLF)
		num++;

	return num;
}


// ',' 기준으로 채널 split
std::vector<std::string> Command::splitByComma(std::string str) {
	std::vector<std::string> ret;

	std::size_t pos = 0;
	while (pos != std::string::npos) {
		std::size_t tmpPos = str.find(',', pos);
		int count = (tmpPos == std::string::npos) ? str.size() - pos : tmpPos - pos;
		ret.push_back(str.substr(pos, count));
		pos = (tmpPos == std::string::npos) ? tmpPos : tmpPos + 1;
	}
	
	return ret;
}


Channel* Command::isChannelExist(std::map<std::string,Channel*> &channelsInServer, std::string channelName) {
	std::map<std::string, Channel*>::iterator channel;

	channel = channelsInServer.find(channelName);
	if (channel == channelsInServer.end())
		return NULL;
	return channel->second;
}

Client* Command::findClientByNick(std::map<int, Client*> &clientsInServer, std::string nick) {
	std::map<int, Client*>::iterator it = clientsInServer.begin();
	for (; it != clientsInServer.end() ; ++it) {
		if (it->second->getNickname() == nick)
			return it->second;
	}
	return nullptr;
}

int	Command::sendByFD(int fd, std::string message) {
	return send(fd, message.c_str(), message.length(), 0);
}