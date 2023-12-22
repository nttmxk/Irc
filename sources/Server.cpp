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
		std::cerr << "Socket bind err\n";
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

	clients[clientFd] = new Client(clientFd);
//	std::clog << "[Log] " << clientFd << " is connected\n";
	setPoll(clientFd, clientFd, POLLIN | POLLHUP, 0);
}

void Server::readMessage(int clientFd) {
	int readSize = recv(clientFd, buffer, BUF_LEN - 1, MSG_DONTWAIT);

	if (readSize < 0) {
		setPoll(clientFd, -1, 0, 0);
		close(clientFd);
		throw std::runtime_error("Error\nMessage read size < 0\n");
	}
	buffer[readSize] = 0;
//	std::clog << "Message from " << clientFd << " with rSize: " << readSize << '\n' << buffer << '\n';
	if (readSize == 0) // when connection is closed, this is not an error right?
	{
		std::clog << "[Log] " << clientFd << ": Quit\n";
		message[clientFd] = "QUIT :<reason>\r\n"; // reason
		runCommand(clientFd);
		setPoll(clientFd, -1, 0, 0);
		close(clientFd);
	}
	else if (readSize > 1 && buffer[readSize - 2] == '\r' && buffer[readSize - 1] == '\n')
	{
		message[clientFd].append(buffer);
		std::clog << "[Log] " << clientFd << ": \n" << message[clientFd] << '\n';
		runCommand(clientFd);
		message[clientFd].clear();
		// when runCommand executes <QUIT>... then setPoll and close should be called here too?
		// message flush
	}
	else // Ctrl + D handling
		message[clientFd].append(buffer);
}

void Server::runCommand(int clientFd) {
	Command command(clients[clientFd], message[clientFd]);
	int 	type;

	while (!command.isTokenEnd()) {
		type = command.getCommandType();
		switch (type) {
			case (PASS):
				command.pass(_pwd);
				break;
			case (NICK):
				command.nick();
				break;
			case (USER):
				command.user();
				break;
			default :
				break;
		}
		if (command.isConnectEnd)
		{
			// disconnect routine
		}
	}
}


Server::~Server() {}