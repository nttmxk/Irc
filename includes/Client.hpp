#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

class Client {
private:
	int         client_fd;
 	std::string nickname;   
	std::string	userName;
	std::string realName;

	bool		_authorized;

	Client();

public:
    Client(int _fd);
    // Client(int _fd, std::string _nickname);
    ~Client();

    int         getClientFd() const;
 	std::string getNickname() const;   
	std::string getUserName() const;
	std::string getRealName() const;

    void    setNickname(const std::string nickname);
	void	setUserName(const std::string userName);
    void    setRealName(const std::string realName);

	bool	isAuthorized() const;
	void	checkAuthorization();
};

#endif