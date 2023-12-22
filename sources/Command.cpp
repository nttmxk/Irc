#include "../includes/Command.hpp"
#include <iostream> // cout
#include <sys/socket.h> // send

//Command::Command()
//	: client(NULL), message("") {}
//Command::Command(const Command& abj) { (void)abj; }
//Command::~Command() {}


Command::Command(Client * _client, const std::string _message) 
	: client(_client), message(_message), messageIndex(0) {
	
	this->parseMessage();
}

void Command::parseMessage() {
	size_t		prev = 0;
    size_t		next;
	size_t		crlf;

    while (this->message[prev])
    {
		crlf = this->message.find("\r\n", prev);
		next = this->message.find(' ', prev);
		if (this->message[prev] == ':' || crlf < next)
		{
            this->tokens.push_back(this->message.substr(prev, crlf - prev));
			prev = crlf + 2;
		}
		else
		{
			this->tokens.push_back(this->message.substr(prev, next - prev));
			prev = next + 1;
		}
    }
	messageSize = tokens.size();
}

int Command::getCommandType()
{
	std::string cmd = tokens[messageIndex++];
	int type = 0;

	for ( ; type<4; type++) {
		if (commandTypeStr[type] == cmd)
			return type;
	}
	return -1;
}

void Command::sendReply(std::string replyMsg) {

	std::string tmp = replyMsg[replyMsg.length() - 1] == '\n' ? replyMsg : replyMsg + "\n";
	std::cout << ">> Sending message to client: " << tmp;
	if (send(client->getClientFd(), tmp.c_str(), tmp.length(), 0) == -1)
		throw std::runtime_error("Error sending message");
}

bool Command::isEnd()
{
	if (messageIndex < messageSize)
		return false;
	return true;
}
