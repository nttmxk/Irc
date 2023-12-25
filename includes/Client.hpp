#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <vector>

enum UserMode {
	OPERATOR = 'O'
};


class Client {
private:
	int         client_fd;
 	std::string nickname;   
	std::string	userName;
	std::string realName;
//	std::string	password;
//	std::string modeStr;

	bool		_authorized;

	std::vector<std::string> joinedChannels;

	Client();

public:
    Client(int _fd);
    // Client(int _fd, std::string _nickname);
    ~Client();

    int         getClientFd() const;
 	std::string getNickname() const;   
	std::string getUserName() const;
	std::string getRealName() const;
//	std::string	getPassword() const;
//	std::string getUserMode() const;
	std::vector<std::string> getJoinedChannels() const;

    void    setNickname(const std::string nickname);
	void	setUserName(const std::string userName);
    void    setRealName(const std::string realName);
	void	setAuthorization();

	void	joinChannel(std::string channelName);
	void	quitChannel(std::string channelName);
	void	quitAllChannels();

	bool	isAuthorized() const;

	bool	isServerOper() const;
	void	setServerOper();
};

#endif