#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstring>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <poll.h>

# define BUF_LEN 1024
# define USER_MAX 128

class Server {
public:
	Server(int portNum, std::string pwd);
	Server();
	~Server();
	Server(const Server &orig);
	Server &operator=(const Server &orig);

	int				getServerFd(void) const;
	struct pollfd	*getPollFds(void) const;
	void			addClient(void);
	void			readMessage(int clientFd);
	void			runCommand(int clientFd);

private:
	int					serverFd;
	struct sockaddr_in	serverAddress;
	char				buffer[BUF_LEN];
	struct pollfd		pollFds[USER_MAX + 4];
	std::string			_pwd;

	void				setPoll(int index, int fd, short events, short revents);
	//std::list<Channel>
};

#endif