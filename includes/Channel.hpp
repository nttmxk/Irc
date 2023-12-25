#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <map>
# include <vector>
# include <string>

class Client;

# define ChannelModeCnt 5
enum {
	INVITE_ONLY = 0,	// Set/remove Invite-only channel
	TOPIC,				// Set/remove the restrictions of the TOPIC command to channel operators
	KEY,				// Set/remove the channel key (password)
	OPER,				// Give/take channel operator privilege
	USER_LIMIT			// Set/remove the user limit to channel
};

static const char channelModeChar[ChannelModeCnt] = {'i', 't', 'k', 'o', 'l'};

class Channel {
private:
	// 채널 이름
	std::string name;
	// 채널 키
	std::string key;
	// 채널 토픽
	std::string topic;
	// 채널 모드
	bool mode[ChannelModeCnt];

	// 참여가능한 멤버 수 제한
	unsigned long memberLimit;
	// 채널에 속해 있는 모든 멤버 리스트, key: nickname
	std::map<std::string, Client *> members;
	// 운영자(operator) 리스트
	std::vector <std::string> operators;
	// 운영자가 아닌 보통 멤버 리스트
	std::vector <std::string> normalMembers;
	std::vector <std::string> invitedUsers;

	Channel();

public:
	Channel(std::string const name, Client *creator);
	~Channel();

	/* Getter */
	std::string	getName();
	std::string	getKey();
	std::string	getTopic();
	std::string	getChannelMode();
	int			getMemberNum();
	std::map<std::string, Client *>	&getMembers();

	/* Setter */
	void setTopic(std::string newTopic);
	void setKey(std::string newKey);
	void setMemberLimit(int n);

	/* Member */
	bool isInChannel(const std::string targetNick) const;
	void addMember(Client *client, bool isOper);
	void addInvitedList(std::string targetNick);
	void addInvitedMember(Client *client);
	void deleteMember(const std::string targetNick); // 멤버가 operator인 경우 deleteOperator 실행해줘야함.

	bool isInvitedMember(const std::string targetNick);
	bool isFull();

	std::string getMemberStr();

	/* Operator */
	bool isOperator(const std::string targetNick) const;
	void addOperator(const std::string targetNick);
	void deleteOperator(const std::string targetNick);

	/* Modes */
	void onMode(const int modeIdx);
	void offMode(const int modeIdx);
	bool isModeOn(const char modeChar);

private:
	/* Normal Member */
	void addNormalMember(const std::string targetNick);
	void deleteNormalMember(const std::string targetNick);

};

#endif