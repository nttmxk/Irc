#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstring>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <poll.h>
# include <fcntl.h>

# include "Client.hpp"
# include "Channel.hpp"

# define BUF_LEN 1024
# define MSG_LEN 2048
# define USER_MAX 128

class Server {
public:
	Server(int portNum, std::string pwd);
	Server();
	~Server();
	Server(const Server &orig);
	Server &operator=(const Server &orig);

	int				getServerFd(void) const;
	struct pollfd	*getPollFds(void);
	void			addClient(void);
	void			readMessage(int clientFd);
	void			runCommand(int clientFd);

private:
	int								serverFd;
	struct sockaddr_in				serverAddress;
	struct pollfd					pollFds[USER_MAX + 4];
	char							buffer[BUF_LEN];
	std::map<int, std::string>		message;
	std::map<int, Client>			clients;
	std::map<std::string, Channel>	channels;
	std::string						_pwd;

	void				setPoll(int index, int fd, short events, short revents);
	//std::list<Channel>
};

#endif