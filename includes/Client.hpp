#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <vector>

enum UserMode {
	OPERATOR = 'O'
};

enum UserFlag {
	_init = 0,
	_pass,
	_nick,
	_connect
};

class Client {
private:
	int			client_fd;
	std::string	nickname;
	std::string	userName;
	std::string realName;
	std::string	modeStr;
	int			_flag;

	std::vector <std::string>	joinedChannels;

public:
	Client(int _fd);
	~Client();

	int getClientFd() const;
	int getFlag() const;
	std::string getNickname() const;
	std::string getUserName() const;
	std::string getRealName() const;
	std::vector <std::string> getJoinedChannels() const;

	void setNickname(const std::string nickname);
	void setUserName(const std::string userName);
	void setRealName(const std::string realName);
	void setFlag(int flag);

	void joinChannel(std::string channelName);
	void quitChannel(std::string channelName);
	void quitAllChannels();

	bool isAuthorized() const;

	bool isServerOper() const;
	void setServerOper(bool op);
};

#endif