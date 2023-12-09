#include "Server.hpp"

Server::Server(int portNum, std::string pwd) {
	_pwd = pwd;
	if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		exit(1); // should define how to handle the error
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNum);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
	{
		close(serverFd);
		exit(1);
	}
	if (listen(serverFd, USER_MAX) == -1)
	{
		close(serverFd);
		exit(1);
	}
	setPoll(serverFd, POLLIN, 0);
	// set the clientFds
}

int Server::getServerFd() const {
	return serverFd;
}

int Server::getClientNum() const {
	return _clientNum;
}

struct pollfd* Server::getPollFds() const {
	return pollFds;
}

void Server::setPoll(int fd, short events, short revents) {
	pollFds[_clientNum].fd = fd;
	pollFds[_clientNum].events = events;
	pollFds[_clientNum].revents = revents;
}

Server::~Server() {}