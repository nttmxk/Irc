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
	std::clog << "ServerFD : " << serverFd << " is created\n";
	for (int i = 0; i < USER_MAX + 4; ++i) {
		setPoll(i, -1, 0, 0);
	}
	time_t t;
	time(&t);
	startTime = ctime(&t);
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

void Server::deleteClient(int clientFd) {
	setPoll(clientFd, -1, 0, 0);
	message.erase(clientFd);
	clients.erase(clientFd);
	close(clientFd);
}

void Server::readMessage(int clientFd) {
	int readSize = recv(clientFd, buffer, BUF_LEN - 1, MSG_DONTWAIT);

	if (readSize < 0) {
		deleteClient(clientFd);
		throw std::runtime_error("Error\nMessage read size < 0\n");
	}
	buffer[readSize] = 0;
//	std::clog << "Message from " << clientFd << " with rSize: " << readSize << '\n' << buffer << '\n';
	if (readSize == 0)
	{
		std::clog << "[Log] rSize=0 " << clientFd << ": Quit\n";
		message[clientFd] = "QUIT :disconnected\r\n";
		runCommand(clientFd);
		deleteClient(clientFd);
	}
	else if (readSize > 1 && buffer[readSize - 2] == '\r' && buffer[readSize - 1] == '\n')
	{
		message[clientFd].append(buffer);
		std::clog << "[Log] " << clientFd << ":New message\n" << message[clientFd] << '\n';
		runCommand(clientFd);
		message[clientFd].clear();
	}
	else
		message[clientFd].append(buffer);
}

void Server::runCommand(int clientFd) {
	Command command(clients[clientFd], message[clientFd]);
	int 	type;
	bool	noCommand = false;

	while (!command.isTokenEnd()) {
		type = command.getCommandType();
		switch (type) {
			case (CAP):
				command.passCommand();
				break;
			case (PASS):
				command.pass(_pwd);
				break;
			case (NICK):
				command.nick(clients);
				break;
			case (USER):
				command.user(startTime);
				break;
			case (cOPER):
				command.oper(clients, _pwd); // param fix needed
				break;
			case (JOIN):
				command.join(channels);
				break;
			case (PART):
				command.part(channels);
				break;
			case (INVITE):
				command.invite(channels);
				break;
			case (KICK):
				command.kick(channels);
				break;
			case (QUIT):
				command.quit(channels);
				break;
			case (cTOPIC):
				command.topic(channels);
				break;
			case (MODE):
				command.mode(channels);
				break;
			case (NOTICE):
				command.passCommand();
//				command.notice();
				break;
			case (PRIVMSG):
				command.passCommand();
//				command.privmsg();
				break;
			case (PING):
				command.ping();
				break;
			case (WHO):
				command.who(channels);
				break;
			default :
				noCommand = true;
		}
		if (noCommand) {
			std::clog << "[Log] :No Command!\n";
			command.passCommand();
			noCommand = false;
		}
		if (command.isConnectEnd)
			deleteClient(clientFd);
	}
}

Server::~Server() {
	std::map<int, Client*>::iterator it = clients.begin();
	while (it != clients.end())
	{
		close(it->first);
		delete it->second;
		it->second = nullptr;
		++it;
	}
	std::map<std::string, Channel*>::iterator it2 = channels.begin();
	while (it2 != channels.end())
	{
		delete it2->second;
		it2->second = nullptr;
		++it2;
	}
	close(serverFd);
}