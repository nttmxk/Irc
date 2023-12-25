#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstring>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <poll.h>
# include <fcntl.h>
# include <ctime>

# include "Client.hpp"
# include "Channel.hpp"
# include "Command.hpp"

# define BUF_LEN 1024
# define MSG_LEN 2048
# define USER_MAX 128

class Server {
public:
	Server(int portNum, std::string pwd);
	~Server();

	std::string		startTime;

	int				getServerFd(void) const;
	struct pollfd	*getPollFds(void);
	void			addClient(void);
	void			readMessage(int clientFd);

private:
	int				serverFd;
	struct			sockaddr_in serverAddress;
	struct pollfd	pollFds[USER_MAX + 4];
	char			buffer[BUF_LEN];

	std::map<int, std::string>			message;
	std::map<int, Client *>				clients;
	std::map<std::string, Channel *>	channels;
	std::string							_pwd;

	void	setPoll(int index, int fd, short events, short revents);
	void	runCommand(int clientFd);
	void	deleteClient(int clientFd);
};

#endif