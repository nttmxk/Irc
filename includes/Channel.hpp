#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <map>
# include <vector>
# include <string>

class Client;

typedef enum ChannelMode {
	INVITE_ONLY = 'i',	// Set/remove Invite-only channel
	TOPIC = 't',		// Set/remove the restrictions of the TOPIC command to channel operators
	KEY = 'k',			// Set/remove the channel key (password)
	OPER = 'o',			// Give/take channel operator privilege
	USER_LIMIT = 'l',	// Set/remove the user limit to channel
	MODE_CNT = 6
};

class Channel {
private:
	
	// 채널 이름
	std::string name;
	// 채널 토픽
	std::string	topic;
	// 채널 모드
	std::string	mode;

	// 채널에 속해 있는 모든 멤버 리스트, key: nickname (fd로 수정할 수도...?)
	std::map<std::string, Client&>	members;
	// 운영자(operator) 리스트
	std::vector<std::string>	operators;


public:
	Channel(std::string const &name);
	~Channel();

	/* Getter */
	std::string&					getName();
	std::string&					getTopic();
	std::string&					getMode();
	std::map <std::string, Client>&	getMembers();
	std::vector<std::string>&		getOperators();

	/* Setter */
	void	setTopic(std::string& newTopic);
	
	/* Member */
	bool	isInChannel(const std::string& clientNickname) const;
	void	addMember(Client& client);
	void	deleteMember(Client& client);

	/* Operator */
	bool	isOperator(const std::string& clientNickname) const;
	void	addOperator(Client& client);
	void	deleteOperator(Client& client);

	/* Modes */
	void	addMode(ChannelMode& const mode);
	void	deleteMode(ChannelMode& const mode);

	/* Message Sending */
	void	sendTo(std::string message, Client& from, std::vector<Client&> to);
	void	sendToAll(std::string message, Client& from);

};

#endif