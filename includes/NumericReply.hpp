#ifndef NUMERIC_REPLY_HPP
# define NUMERIC_REPLY_HPP

# define USER_ADDR(nickname, username, hostname) \
	nickname + "!" + username + "@" + hostname
	
# define RPL_WELCOME(servername, nick) \
	":" + servername + " 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "\r\n"
# define RPL_YOURHOST(servername, nick, version) \
	":" + servername + " 002 " + nick + " :Your host is " + servername + ", running version " + version + "\r\n"
# define RPL_CREATED(servername, nick, creationDate) \
	":" + servername + " 003 " + nick + " :This server was created " + creationDate + "\r\n"
# define RPL_MYINFO(servername, nick, version, userModes, chanModes, chanParamsModes) \
	":" + servername + " 004 " + nick + " " + servername + " " + version + " " + userModes + " " + chanModes + " " + chanParamsModes + "\r\n"
# define RPL_ISUPPORT(servername, nick, tokens) \
	":" + servername + " 005 " + nick + " " + tokens + " :are supported by this server\r\n"

// RPL_CHANNELMODEIS (324)		// mode
# define RPL_CHANNELMODEIS(servername, nick, channel, modes) \
	":" + servername + " 324 " + nick + " #" + channel + " :+" + modes + "\r\n"
// RPL_NOTOPIC (331)			// topic
# define RPL_NOTOPIC(servername, nick, channel) \
	":" + servername + " 331 " + nick + " #" + channel + " :No topic is set\r\n"
// RPL_TOPIC (332)				// join, topic
# define RPL_TOPIC(servername, nick, channel, topic) \
	":" + servername + " 332 " + nick + " #" + channel + " :" + topic + "\r\n"
// RPL_TOPICWHOTIME (333)		// join, topic
# define RPL_TOPICWHOTIME(servername, nick, channel, userAddr, time) \
	":" + servername + " 333 " + nick + " #" + channel + " " + userAddr + " :" + time + "\r\n"
// RPL_INVITING (341)			// invite
# define RPL_INVITING(servername, nick, channel, targetUser) \
	":" + servername + " 341 " + nick + " " + targetUser + " :#" + channel + "\r\n"
// RPL_NAMREPLY (353)			// join
# define RPL_NAMREPLY(servername, nick, symbol, channel, users) \
	":" + servername + " 353 " + nick + " = #" + channel + " :" + users + "\r\n"
// RPL_ENDOFNAMES (366)			// join
# define RPL_ENDOFNAMES(servername, nick, channel) \
	":" + servername + " 366 " + nick + " #" + channel + " :End of /NAMES list\r\n"
// ERR_NOSUCHNICK (401)			// mode, privmsg
#define ERR_NOSUCHNICK(servername, nick) \
	":" + servername + " 401 " + nick + " :No such nick\r\n"
// ERR_NOSUCHCHANNEL (403)		// join, part, topic, invite, kick, mode
# define ERR_NOSUCHCHANNEL(servername, nick, channel) \
	":" + servername + " 403 " + nick + " #" + channel + " :No such channel\r\n"
// ERR_NONICKNAMEGIVEN (431)	// nick
# define ERR_NONICKNAMEGIVEN(servername, nick) \
	":" + servername + " 432 " + nick + " :No nickname given\r\n"
// ERR_ERRONEUSNICKNAME (432)	// nick
# define ERR_ERRONEUSNICKNAME(servername, nick, newNickname) \
	":" + servername + " 432 " + nick + " " + newNickname + " :Erroneous Nickname\r\n"
// ERR_NICKNAMEINUSE (433)		// nick
# define ERR_NICKNAMEINUSE(servername, nick, newNickname) \
	":" + servername + " 433 " + nick + " " + newNickname + " :Nickname is already in use.\r\n"
// ERR_USERNOTINCHANNEL (441)	// kick
# define ERR_USERNOTINCHANNEL(servername, nick, targetUser, channel) \
	":" + servername + " 441 " + nick + " " + targetUser + " #" + channel + " :They are not on that channel\r\n"
// ERR_NOTONCHANNEL (442)		// part, topic, invite, kick
# define ERR_NOTONCHANNEL(servername, nick, channel) \
	":" + servername + " 442 " + nick + " #" + channel + " :You're not on that channel!\r\n"
// ERR_USERONCHANNEL (443)		// invite
# define ERR_USERONCHANNEL(servername, nick, targetUser, channel) \
	":" + servername + " 443 " + nick + " " + targetUser + " #" + channel + " :is already on channel\r\n"
// ERR_NEEDMOREPARAMS (461)		// pass, user, oper, join, part, topic, invite, kick
# define ERR_NEEDMOREPARAMS(servername, nick, command) \
	":" + servername + " 461 " + nick + " " + command + " :Not enough parameters.\r\n"
// ERR_ALREADYREGISTRED (462)	// pass, user
# define ERR_ALREADYREGISTRED(servername, nick) \
	":" + servername + " 462 " + nick + " :You may not reregister\r\n"
// ERR_PASSWDMISMATCH (464)		// pass, oper
# define ERR_PASSWDMISMATCH(servername, nick) \
	":" + servername + " 464 " + nick + " :Password incorrect\r\n"
// ERR_CHANOPRIVSNEEDED (482)	// topic, invite, kick, mode
# define ERR_CHANOPRIVSNEEDED(servername, nick, channel) \
	":" + servername + " 482 " + nick + " #" + channel + " :You must be a channel operator\r\n"

/*
RPL_UMODEIS (221)			// mode
RPL_AWAY (301)				// privmsg
RPL_CREATIONTIME (329)		// mode
RPL_YOUREOPER (381)			// oper
ERR_NOSUCHSERVER (402)		// privmsg
ERR_CANNOTSENDTOCHAN (404)	// privmsg
ERR_TOOMANYCHANNELS (405)	// join
ERR_TOOMANYTARGETS (407)	// privmsg
ERR_NORECIPIENT (411)		// privmsg
ERR_NOTEXTTOSEND (412)		// privmsg
ERR_NOTOPLEVEL (413)		// privmsg
ERR_WILDTOPLEVEL (414)		// privmsg

ERR_NICKCOLLISION (436)		// nick

// ERR_CHANNELISFULL (471)		// join
#define ERR_CHANNELISFULL(servername, nick, channel) \
	":" + servername + " 471 " + nick + " #" + channel + " :Cannot join channel , channel is full(+l)\n"
#define ERR_UNKNOWNMODE(servername, nick, mode) \
	":" + servername + " 472 " + nick + " " + mode + " :is unknown mode char to me for that channel\n"
// ERR_INVITEONLYCHAN (473)	// join
#define ERR_INVITEONLYCHAN(servername, nick, channel) \
	":" + servername + " 473 " + nick + " #" + channel + " :Cannot join channel (+i)\n"
// ERR_BANNEDFROMCHAN (474)	// join
#define ERR_BANNEDFROMCHAN(servername, nick, channel) \
	":" + servername + " 474 " + nick + " #" + channel + " :Cannot join channel (+b)\n"
// ERR_BADCHANNELKEY (475)		// join
#define ERR_BADCHANNELKEY(servername, nick, channel) \
	":" + servername + " 475 " + nick + " #" + channel + " :Bad channel key\n"
// ERR_BADCHANMASK (476)		// join, kick
#define ERR_BADCHANMASK(servername, nick) \
	":" + servername + " 476 * " + nick + " :Bad Channel Mask\n"

ERR_NOOPERHOST (491)		// oper
ERR_UMODEUNKNOWNFLAG (501)	// mode
// ERR_USERSDONTMATCH (502)	// mode
#define ERR_USERSDONTMATCH(servername, nick) \
	":" + servername + " 502 " + nick + " :Cant change mode for other users\n"
*/


#endif