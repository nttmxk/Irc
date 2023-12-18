#include "../includes/Server.hpp"

Server::Server(int portNum, std::string pwd) {
	_pwd = pwd;
	if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		exit(1); // should define how to handle the error
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNum);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverFd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
		close(serverFd);
		exit(1);
	}
	if (listen(serverFd, USER_MAX) == -1) {
		close(serverFd);
		exit(1);
	}
	/*log*/ std::clog << "ServerFD : " << serverFd << '\n';
	for (int i = 0; i < USER_MAX + 4; ++i) {
		setPoll(i, -1, 0, 0);
	}
	setPoll(serverFd, serverFd, POLLIN, 0);
}

int Server::getServerFd() const {
	return serverFd;
}

struct pollfd *Server::getPollFds() {
	return pollFds;
}

void Server::setPoll(int index, int fd, short events, short revents) {
	pollFds[index].fd = fd;
	pollFds[index].events = events;
	pollFds[index].revents = revents;
}

void Server::addClient(void) {
	struct sockaddr_in	clientAddress;
	int					clientFd;
	socklen_t			clientAddressSize = sizeof(clientAddress);

	if ((clientFd = accept(serverFd, (struct sockaddr *) &clientAddress,
						   &clientAddressSize)) == -1)
		throw std::runtime_error("Error\nCannot open client socket\n");

	if (clientFd >= USER_MAX + 4) {
		close(clientFd);
		throw std::runtime_error("Error\nReached USER_MAX\n");
	}

	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) {
		close(clientFd);
		throw std::runtime_error("Error\nfcntl\n");
	}

	setPoll(clientFd, clientFd, POLLIN | POLLHUP, 0);
}

void Server::readMessage(int clientFd) {
	int messageSize = read(clientFd, buffer, BUF_LEN);

	if (messageSize < 0) {
		setPoll(clientFd, -1, 0, 0);
		close(clientFd);
		throw std::runtime_error("Error\nmessageSize < 0\n");
	}
	if (messageSize == 0) // when connection is closed, this is not an error right?
	{
//		runCommand(clientFd);
		setPoll(clientFd, -1, 0, 0);
		close(clientFd);
	} else // Ctrl+D handling? saving buffer?
	{
		runCommand(clientFd);
	}
}

void Server::runCommand(int clientFd) {
	(void)clientFd;
}


Server::~Server() {}