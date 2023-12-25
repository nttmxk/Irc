#include "../includes/Client.hpp"


Client::Client(int _fd) 
	: client_fd(_fd), 
	  nickname(""), userName(""), realName(""), 
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

std::vector<std::string> Client::getJoinedChannels() const {
	return joinedChannels;
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

void Client::setAuthorization() {
	this->_authorized = true;
}

void Client::joinChannel(std::string channelName) {
	joinedChannels.push_back(channelName);
}

void Client::quitChannel(std::string channelName) {
	joinedChannels.erase(std::remove(joinedChannels.begin(), joinedChannels.end(), channelName), joinedChannels.end());
}

void Client::quitAllChannels() {
	joinedChannels.clear();
}

bool Client::isAuthorized() const {
	return this->_authorized;
}

bool Client::isServerOper() const {
	return modeStr.find(OPERATOR) != std::string::npos;
}

void Client::setServerOper(bool op) {
	if (op)
		modeStr = 'O';
	else
		modeStr = "";
}