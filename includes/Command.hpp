#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "../includes/NumericReply.hpp"
# include "../includes/Client.hpp"
# include "../includes/Channel.hpp"

# include <sys/socket.h>
# include <string>
# include <vector>
# include <map>
# include <iostream>

# define CRLF "\r\n"

class Channel;

# define COMMAND_CNT 16

enum CommandType {
	CAP = 0,
	PASS,
	NICK,
	USER,
	cOPER,
	JOIN,
	PART,
	INVITE,
	KICK,
	QUIT,
	cTOPIC,
	MODE,
	NOTICE,
	PRIVMSG,
	PING,
	WHO
};

static const std::string commandTypeStr[COMMAND_CNT] = {"CAP", "PASS", "NICK", "USER", "OPER", "JOIN", "PART",
														"INVITE", "KICK", "QUIT", "TOPIC", "MODE", "NOTICE", "PRIVMSG",
														"PING", "WHO"};

class Command {
private:
	Client						*client;
	std::string					message;
	std::vector <std::string>	tokens;
	int							messageIndex;
	int							tokenSize;

public:
	Command(Client *_client, const std::string _message);
	int		getCommandType();
	bool	isTokenEnd();
	bool	isConnectEnd;
	int		getNumParameter();
	void	passCommand();

private:
	void	parseMessage();
	void	sendReply(std::string);
	int		sendByFD(int fd, std::string message);
	int		getParamsCnt();

	std::vector <std::string>	splitByComma(std::string str);
	Channel	*isChannelExist(std::map<std::string, Channel *> &channelsInServer, std::string channelName);
	Client	*findClientByNick(std::map<int, Client *> &clientsInServer, std::string nick);

public:

/* Connection Registration */
	void pass(const std::string pwd);
	void nick(std::map<int, Client *> &clients, std::map<std::string, Channel *> &channelsInServer);
	void user(std::string time);
	void oper(std::map<int, Client *> &clientsInServer, const std::string pwd);
	void quit(std::map<std::string, Channel *> &channelsInServer);
	void mode(std::map<std::string, Channel *> &channelsInServer);
	void ping();

/* Channel Operations */
	void join(std::map<std::string, Channel *> &channelsInServer);
	void who(std::map<std::string, Channel *> &channelsInServer);
	void part(std::map<std::string, Channel *> &channelsInServer);
	void topic(std::map<std::string, Channel *> &channelsInServer);
	void invite(std::map<int, Client *> &clientsInServer, std::map<std::string, Channel *> &channelsInServer);
	void kick(std::map<int, Client *> &clientsInServer, std::map<std::string, Channel *> &channelsInServer);

	void sendToChannel(std::string nick, std::string message, Channel *channel);
	void privmsg(std::map<int, Client *> clients, std::map<std::string, Channel *> channels);
	void notice(std::vector<Client &> targets, std::string message) const;
	void notice(std::vector<Channel &> targets, std::string message) const;
};

#endif