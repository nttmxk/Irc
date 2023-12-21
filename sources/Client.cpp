#include "../includes/Client.hpp"


Client::Client(int _fd) 
	: client_fd(_fd), 
	  nickname(""), userName(""), realName(""), 
	  password(""),
	  _authorized(false) {}

Client::~Client() {}

int	Client::getClientFd() const {
	return this->client_fd;
}

std::string Client::getNickname() const {
	return this->nickname;
}

std::string Client::getUserName() const {
	return this->userName;
}

std::string Client::getRealName() const {
	return this->realName;
}

std::string	Client::getPassword() const {
	return this->password;
}
    
void Client::setNickname(const std::string nickname) {
	this->nickname = nickname;
}

void Client::setUserName(const std::string userName) {
	this->userName = userName;
}

void Client::setRealName(const std::string realName) {
	this->realName = realName;
}

void Client::setPassword(const std::string password) {
	this->password = password;
}

bool Client::isAuthorized() const {
	return this->_authorized;
}

void Client::checkAuthorization() {
	this->_authorized = true;
}

bool Client::isServerOper() const {
	return modeStr.find(OPERATOR) != std::string::npos;
}

void Client::setServerOper() {
	if (this->isServerOper())
		return;
	this->modeStr += 'O';
}