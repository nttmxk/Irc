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
	Server& operator=(const Server &orig);

	int				getServerFd(void) const;
	int			 	getClientNum(void) const;
	struct pollfd*	getPollFds(void) const;
	int				addClient(void);
	int 			readMessage(int clientFd);
	void			runCommand(int clientFd);

private:
	int						serverFd;
	struct sockaddr_in		serverAddress;
//	int 					clientFds[USER_MAX];
	char 					buffer[BUF_LEN];
	struct pollfd			pollFds[USER_MAX + 1];
	int 					_clientNum;
	std::string 			_pwd;
	void					setPoll(int fd, short events, short revents);
	// vector / list / map?
	//std::list<Channel>
	//std::list<Client>

};

#endif