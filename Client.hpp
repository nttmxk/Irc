#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

class Client {
private:
    std::string realName;
    std::string nickname;
    int         client_fd;

public:
    Client();
    Client(std::string nickname);
    ~Client();

    int         getClientFd() const;
    std::string getRealName() const;
    std::string getNickname() const;
    
    void    setNickname(std::string nickname);
    void    setRealName(std::string realName);
};

#endif