#include "../includes/Command.hpp"
#include <iostream> // cout
#include <sys/socket.h> // send

Command::Command()
	: client(NULL), message("") {}
Command::Command(const Command& abj) { (void)abj; }
Command::~Command() {}


Command::Command(Client * _client, const std::string _message) 
	: client(_client), message(_message) {
	
	this->parseMessage();
	this->execute(tokens[0]);
}

void Command::parseMessage() {
	size_t        prev = 0;
    size_t        next;

    while (this->message[prev])
    {
        next = this->message.find_first_of(" ,", prev);
        if (next == std::string::npos || this->message[prev] == ':')
        {
            this->tokens.push_back(this->message.substr(prev));
            break;
        }
        this->tokens.push_back(this->message.substr(prev, next - prev));
        prev = next + 1;
    }
}

void Command::execute(std::string cmd) {
	int type = 0;
	for ( ; type<4; type++) {
		if (commandTypeStr[type] == cmd) break;
	}

	switch (type) {
		case (PASS):
			this->pass(); break;
		case (NICK):
			this->nick(); break;
		case (USER):
			this->user(); break;
		default :
			return;
	}	
}

void Command::sendReply(std::string replyMsg) {

	std::string tmp = replyMsg[replyMsg.length() - 1] == '\n' ? replyMsg : replyMsg + "\n";
	std::cout << ">> Sending message to client: " << tmp;
	if (send(client->getClientFd(), tmp.c_str(), tmp.length(), 0) == -1)
		throw std::runtime_error("Error sending message");
}