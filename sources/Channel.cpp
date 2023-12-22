#include "../includes/Channel.hpp"

Channel() : name(""), key(""),  {
    name = "";
    key = "";
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
	// 초대 보낸 유저 리스트
	std::vector<std::string>	invitedUsers;
}

Channel(std::string const &name) {

}
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
	void	addMember(Client& client);
	void	deleteMember(const std::string targetNick); // 멤버가 operator인 경우 deleteOperator 실행해줘야함.
	bool	isFull();

	bool	isInvitedMember(const std::string targetNick);
	void	addInvitedMember(Client& client);

	std::string getMemberStr();

	/* Operator */
	bool	isOperator(const std::string targetNick) const;
	void	addOperator(Client& client);
	void	deleteOperator(const std::string targetNick);

	/* Modes */
	void	onMode(const ChannelMode& mode);
	void	offMode(const ChannelMode& mode);
	bool	isModeOn(const char mode);