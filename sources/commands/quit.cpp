#include "../../includes/Command.hpp"

/* QUIT [<reason>]
 * A client session is terminated with a quit message.  
 * The server acknowledges this by sending an ERROR message to the client.
 * Example: QUIT :Gone to have lunch 
 *		:syrk!kalt@millennium.stealth.net QUIT :Gone to have lunch 
 *		; User syrk has quit IRC to have lunch. ( --> Preferred message format )
 * client가 서버 접속을 끊음, client가 접속해 있는 모든 채널에서 나감
 */
void Command::quit(std::map<std::string, Channel*> &channelsInServer) {
	int numParam = getNumParameter();
	if (client->getFlag() != _connect)
	{
		messageIndex += numParam + 1;
		return;
	}
	std::vector<std::string> joinedChannels = client->getJoinedChannels();
	std::string nick = client->getNickname();
	std::string quitMsg = "Quit: ";
	quitMsg += (numParam == 1) ? "leaving" : tokens[messageIndex + 1];
	quitMsg += CRLF;

	messageIndex += numParam + 1;
	std::vector<std::string>::iterator it = joinedChannels.begin();
	for ( ; it != joinedChannels.end(); it++) {
		std::string targetChannel = *it;

		Channel* channelPtr = isChannelExist(channelsInServer, targetChannel);
		if (channelPtr == NULL || !(channelPtr->isInChannel(nick)))
			continue;
		sendToChannel(nick, quitMsg, channelPtr);
		channelPtr->deleteMember(nick);
		if (channelPtr->getMemberNum() == 0)
		{
			delete channelPtr;
			channelsInServer.erase(targetChannel);
		}
	}
	// We're going to delete the Client so we don't have to manage joinedChannels?
	isConnectEnd = true;
}
