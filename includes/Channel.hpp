#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <map>
# include <vector>
# include <string>

class Client;

# define ChannelModeCnt 5
typedef enum {
	INVITE_ONLY,	// Set/remove Invite-only channel
	TOPIC,			// Set/remove the restrictions of the TOPIC command to channel operators
	KEY,			// Set/remove the channel key (password)
	OPER,			// Give/take channel operator privilege
	USER_LIMIT		// Set/remove the user limit to channel
} ChannelMode;

static const char channelModeChar[ChannelModeCnt] = {'i', 't', 'k', 'o', 'l'};

class Channel {
private:
	
	// 채널 이름
	std::string name;
	// 채널 키
	std::string key;
	// 채널 토픽
	std::string	topic;
	// 채널 모드
	bool		mode[ChannelModeCnt];

	// 참여가능한 멤버 수 제한
	int memberLimit;
	// 채널에 속해 있는 모든 멤버 리스트, key: nickname (fd로 수정할 수도...?)
	std::map<std::string, Client&>	members;
	// 운영자(operator) 리스트
	std::vector<std::string>	operators;
	// 운영자가 아닌 보통 멤버 리스트
	std::vector<std::string>	normalMembers;
	std::vector<std::string>	invitedUsers;

	Channel();

public:
	Channel(std::string const &name, Client& creator);
	~Channel();

	/* Getter */
	std::string						getName();
	std::string						getKey();	
	std::string						getTopic();
	// const int						getMemberLimit();
	// std::map <std::string, Client&>	getMembers();
	// std::vector<std::string>		getOperators();
	// std::vector<std::string>		getNormalMembers();

	/* Setter */
	void	setTopic(std::string newTopic);
	void	setKey(std::string newKey);
	
	/* Member */
	bool	isInChannel(const std::string targetNick) const;
	void	addMember(Client& client, bool isOper);
	void	deleteMember(const std::string targetNick); // 멤버가 operator인 경우 deleteOperator 실행해줘야함.
	bool	isFull();

	bool	isInvitedMember(const std::string targetNick);
	void	addInvitedMember(Client& client);

	std::string getMemberStr();

	/* Operator */
	bool	isOperator(const std::string targetNick) const;
	void	addOperator(const std::string targetNick);
	void	deleteOperator(const std::string targetNick);

	/* Modes */
	void	onMode(const ChannelMode& mode);
	void	offMode(const ChannelMode& mode);
	bool	isModeOn(const char mode);

	/* Message Sending */
	void	sendTo(std::string message, Client& from, std::vector<Client&> to);
	void	sendToAll(std::string message, Client& from);

private:
	/* Normal Member */
	void	addNormalMember(const std::string targetNick);
	void	deleteNormalMember(const std::string targetNick);

};

#endif