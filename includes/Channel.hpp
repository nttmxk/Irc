#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <map>
# include <vector>
# include <string>

class Client;

typedef enum {
	INVITE_ONLY = 'i',	// Set/remove Invite-only channel
	TOPIC = 't',		// Set/remove the restrictions of the TOPIC command to channel operators
	KEY = 'k',			// Set/remove the channel key (password)
	OPER = 'o',			// Give/take channel operator privilege
	USER_LIMIT = 'l',	// Set/remove the user limit to channel
	MODE_CNT = 6
} ChannelMode;

class Channel {
private:
	
	// 채널 이름
	std::string name;
	// 채널 키
	std::string key;
	// 채널 토픽
	std::string	topic;
	// 채널 모드
	std::string	modeString;

	// 참여가능한 멤버 수 제한
	int memberLimit;
	// 채널에 속해 있는 모든 멤버 리스트, key: nickname (fd로 수정할 수도...?)
	std::map<std::string, Client*>	members;
	// 운영자(operator) 리스트
	std::vector<std::string>	operators;
	// 운영자가 아닌 보통 멤버 리스트
	std::vector<std::string>	normalMembers;


public:
	Channel(std::string const &name);
	~Channel();

	/* Getter */
	std::string						getName();
	std::string						getKey();	
	std::string						getTopic();
	// std::string						getMode();
	const int						getMemberLimit();
	std::map <std::string, Client>	getMembers();
	std::vector<std::string>		getOperators();
	std::vector<std::string>		getNormalMembers();

	/* Setter */
	void	setTopic(std::string& newTopic);
	void	setKey(std::string& newKey);
	
	/* Member */
	bool	isInChannel(const std::string& clientNickname) const;
	void	addMember(Client& client);
	void	deleteMember(const std::string& clientNickname);
	bool	isFull();

	/* Operator */
	bool	isOperator(const std::string& clientNickname) const;
	void	addOperator(Client& client);
	void	deleteOperator(const std::string& clientNickname);

	/* Modes */
	void	addMode(const ChannelMode& mode);
	void	deleteMode(const ChannelMode& mode);
	bool	hasMode(const char mode);

	/* Message Sending */
	void	sendTo(std::string message, Client& from, std::vector<Client&> to);
	void	sendToAll(std::string message, Client& from);

};

#endif