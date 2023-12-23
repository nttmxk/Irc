#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"

Channel::Channel() : name(""), key(""), topic(""), memberLimit(-1) {
	// 채널 모드
	mode[INVITE_ONLY] = false;
	mode[TOPIC] = true;
	mode[KEY] = false;
	mode[OPER] = false;
	mode[USER_LIMIT] = false;
}

Channel::Channel(std::string const name, Client* creator) 
	: name(name), key(""), topic(""), memberLimit(-1) {
	// 채널 모드
	mode[INVITE_ONLY] = false;
	mode[TOPIC] = true;
	mode[KEY] = false;
	mode[OPER] = false;
	mode[USER_LIMIT] = false;

	
	addMember(creator, true);
}

Channel::~Channel() {}

/* Getter */
std::string Channel::getName() {
	return this->name;
}
std::string Channel::getKey() {
	return this->key;
}

std::string Channel::getTopic() {
	return this->topic;
}


/* Setter */
void	Channel::setTopic(std::string newTopic) {
	this->topic = newTopic;
}

void	Channel::setKey(std::string newKey) {
	this->key = newKey;
}
	
/* Member */
bool Channel::isInChannel(const std::string targetNick) const {
	std::map<std::string, Client*>::const_iterator ret = this->members.find(targetNick);
	return ret != this->members.end();
}

void Channel::addMember(Client* client, bool isOper) {
	std::string nickname = client->getNickname();
	if (isInChannel(nickname) == false)
		this->members.insert(std::make_pair(nickname, client));
	isOper ? addOperator(nickname) : addNormalMember(nickname);
}

void Channel::deleteMember(const std::string targetNick) {
	if (isInChannel(targetNick))
		this->members.erase(targetNick);
	normalMembers.erase(std::remove(normalMembers.begin(), normalMembers.end(), targetNick), normalMembers.end());
	
}

bool Channel::isFull() {
	return members.size() >= memberLimit;
}

bool Channel::isInvitedMember(const std::string targetNick) {
	std::vector<std::string>::iterator ret = std::find(invitedUsers.begin(), invitedUsers.end(), targetNick);
	return ret != this->invitedUsers.end();
}

void Channel::addInvitedMember(Client* client) {
	std::string nickname = client->getNickname();
	if (isInvitedMember(nickname))
		addMember(client, false);
	invitedUsers.erase(std::remove(invitedUsers.begin(), invitedUsers.end(), nickname), invitedUsers.end());
	deleteOperator(nickname);
}

std::string Channel::getMemberStr() {
	std::string ret = "";
	std::vector<std::string>::iterator it; 
	std::vector<std::string>::iterator end_it; 

	it = this->operators.begin();
	end_it = this->operators.end();
	for ( ; it != end_it; it++) 
		ret += ("@" + *it + " ");

	it = this->normalMembers.begin(); 
	end_it = this->normalMembers.end();
	for ( ; it != end_it; it++) 
		ret += (*it + " ");
	
	return ret;
}

/* Operator */
bool Channel::isOperator(const std::string targetNick) const {
	std::vector<std::string>::const_iterator ret = std::find(operators.begin(), operators.end(), targetNick);
	return ret != this->operators.end();
}

void Channel::addOperator(const std::string targetNick) {
	if (isOperator(targetNick) == false) {
		deleteNormalMember(targetNick);
		this->operators.push_back(targetNick);
	}
}

void Channel::deleteOperator(const std::string targetNick) {
	operators.erase(std::remove(operators.begin(), operators.end(), targetNick), operators.end());
}

/* Modes */
void Channel::onMode(const int modeIdx) {
	mode[modeIdx] = true;
}

void Channel::offMode(const int modeIdx) {
	mode[modeIdx] = false;
}

bool Channel::isModeOn(const char modeChar) {
	int idx = 0;
	for ( ; idx < ChannelModeCnt; idx++) {
		if (modeChar == channelModeChar[idx])
			break;
	}

	return idx < ChannelModeCnt ? mode[idx] : false;
}

/* Normal Member */
void Channel::addNormalMember(const std::string targetNick) {
	if (isOperator(targetNick)) {
		deleteOperator(targetNick);
		this->normalMembers.push_back(targetNick);
	}
}

void Channel::deleteNormalMember(const std::string targetNick) {
	normalMembers.erase(std::remove(normalMembers.begin(), normalMembers.end(), targetNick), normalMembers.end());
}
