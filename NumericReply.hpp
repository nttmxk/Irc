#ifndef NUMERIC_REPLY_HPP
# define NUMERIC_REPLY_HPP

/*
RPL_UMODEIS (221)			// mode
RPL_AWAY (301)				// privmsg
RPL_CHANNELMODEIS (324)		// mode
RPL_CREATIONTIME (329)		// mode
RPL_NOTOPIC (331)			// topic
RPL_TOPIC (332)				// join, topic
RPL_TOPICWHOTIME (333)		// join, topic
RPL_INVITING (341)			// invite
RPL_NAMREPLY (353)			// join
RPL_ENDOFNAMES (366)		// join
RPL_YOUREOPER (381)			// oper

ERR_NOSUCHNICK (401)		// mode, privmsg
ERR_NOSUCHSERVER (402)		// privmsg
ERR_NOSUCHCHANNEL (403)		// join, part, topic, invite, kick, mode
ERR_CANNOTSENDTOCHAN (404)	// privmsg
ERR_TOOMANYCHANNELS (405)	// join
ERR_TOOMANYTARGETS (407)	// privmsg
ERR_NORECIPIENT (411)		// privmsg
ERR_NOTEXTTOSEND (412)		// privmsg
ERR_NOTOPLEVEL (413)		// privmsg
ERR_WILDTOPLEVEL (414)		// privmsg

ERR_NONICKNAMEGIVEN (431)	// nick
ERR_ERRONEUSNICKNAME (432)	// nick
ERR_NICKNAMEINUSE (433)		// nick
ERR_NICKCOLLISION (436)		// nick

ERR_USERNOTINCHANNEL (441)	// kick
ERR_NOTONCHANNEL (442)		// part, topic, invite, kick
ERR_USERONCHANNEL (443)		// invite

ERR_NEEDMOREPARAMS (461)	// pass, user, oper, join, part, topic, invite, kick
ERR_ALREADYREGISTRED (462)	// pass, user
ERR_PASSWDMISMATCH (464)	// pass, oper

ERR_CHANNELISFULL (471)		// join
ERR_INVITEONLYCHAN (473)	// join
ERR_BANNEDFROMCHAN (474)	// join
ERR_BADCHANNELKEY (475)		// join
ERR_BADCHANMASK (476)		// join, kick

ERR_CHANOPRIVSNEEDED (482)	// topic, invite, kick, mode

ERR_NOOPERHOST (491)		// oper

ERR_UMODEUNKNOWNFLAG (501)	// mode
ERR_USERSDONTMATCH (502)	// mode
*/


#endif