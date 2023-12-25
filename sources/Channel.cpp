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

Channel::Channel(std::string const name, Client *creator)
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

std::string Channel::getChannelMode() {
	std::string modes;
	if (mode[INVITE_ONLY])
		modes += 'i';
	if (mode[TOPIC])
		modes += 't';
	if (mode[KEY])
		modes += 'k';
	if (mode[USER_LIMIT])
		modes += 'l';
	return modes;
}

int Channel::getMemberNum() {
	return members.size();
}


/* Setter */
void Channel::setTopic(std::string newTopic) {
	this->topic = newTopic;
}

void Channel::setKey(std::string newKey) {
	this->key = newKey;
}

void Channel::setMemberLimit(int n) {
	if (n < 0)
		n = -1;
	this->memberLimit = n;
}

/* Member */
bool Channel::isInChannel(const std::string targetNick) const {
	std::map<std::string, Client *>::const_iterator ret = this->members.find(targetNick);
	return ret != this->members.end();
}

void Channel::addMember(Client *client, bool isOper) {
	std::string nickname = client->getNickname();
	if (!isInChannel(nickname))
		this->members.insert(std::make_pair(nickname, client));
	isOper ? addOperator(nickname) : addNormalMember(nickname);
}

void Channel::deleteMember(const std::string targetNick) {
//	std::clog << "[Log] deleteMember\n";
	if (!isInChannel(targetNick))
		return;
	this->members.erase(targetNick);
	deleteOperator(targetNick);
	deleteNormalMember(targetNick);
}

bool Channel::isFull() {
	return members.size() >= memberLimit;
}

bool Channel::isInvitedMember(const std::string targetNick) {
	std::vector<std::string>::iterator ret = std::find(invitedUsers.begin(), invitedUsers.end(), targetNick);
	return ret != this->invitedUsers.end();
}

void Channel::addInvitedMember(Client *client) {
	std::string nickname = client->getNickname();
	if (isInvitedMember(nickname))
		addMember(client, false);
	invitedUsers.erase(std::remove(invitedUsers.begin(), invitedUsers.end(), nickname), invitedUsers.end());
}

std::string Channel::getMemberStr() {
	std::string ret = "";
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator end_it;

	it = this->operators.begin();
	end_it = this->operators.end();
	for (; it != end_it; it++)
		ret += ("@" + *it + " ");

	it = this->normalMembers.begin();
	end_it = this->normalMembers.end();
	for (; it != end_it; it++)
		ret += (*it + " ");

	return ret;
}

std::map<std::string, Client *> &Channel::getMembers() {
	return members;
}

/* Operator */
bool Channel::isOperator(const std::string targetNick) const {
//	std::clog << "[Log] isOperator\n";
	std::vector<std::string>::const_iterator ret = std::find(operators.begin(), operators.end(), targetNick);
	return ret != this->operators.end();
}

void Channel::addOperator(const std::string targetNick) {
//	std::clog << "[Log] addOperator\n";
	if (!isOperator(targetNick)) {
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
	for (; idx < ChannelModeCnt; idx++) {
		if (modeChar == channelModeChar[idx])
			break;
	}
	if (idx < ChannelModeCnt) {
		return mode[idx];
	} else {
		return false;
	}
}

/* Normal Member */
void Channel::addNormalMember(const std::string targetNick) {
	if (isOperator(targetNick))
		deleteOperator(targetNick);
	this->normalMembers.push_back(targetNick);
}

void Channel::deleteNormalMember(const std::string targetNick) {
	normalMembers.erase(std::remove(normalMembers.begin(), normalMembers.end(), targetNick), normalMembers.end());
}
