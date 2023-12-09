#include "Server.hpp"
#include <iostream>

int	checkArg(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cerr << "Error\nUsage: ./ircserv <port> <password>\n";
		return (1);
	}
	if (std::strlen(argv[1]) == 0)
	{
		std::cerr << "Error\nPort number must be in numeric\n";
		return (1);
	}
	for (int i = 0; argv[1][i] != '\0'; ++i) {
		if (argv[1][i] < '0' || argv[1][i] > '9')
		{
			std::cerr << "Error\nPort number must be in numeric\n";
			return (1);
		}
	}
	int portNum = std::atoi(argv[1]);
	if (portNum < 0 || portNum > 66535)
	{
		std::cerr << "Error\nPort number must be in the range of 0 to 66535\n";
		return (1);
	}
	return (0);
}

int main(int argc, char *argv[]) {
	if (checkArg(argc, argv))
		return (1);
	Server server(std::atoi(argv[1]), argv[2]);
	int clientNum;
	int	retPoll;
	struct pollfd *pollFds = server.getPollFds();
	while (1)
	{
		clientNum = server.getClientNum();
		retPoll = poll(pollFds, clientNum + 1, 4242);
		if (retPoll < 0)
			exit(1);
		if (retPoll > 0)
		{
			if (pollFds[0].revents & POLLIN)
			{
				// accept the client
				retPoll--;
			}
			// retPoll != 0과 i < clientNum + 1이 맞나?
			for (int i = 1; retPoll != 0 && i < clientNum + 1; ++i) {
				if (pollFds[i].revents & POLLIN)
				{
					// read event
					retPoll--;
				}
				if (pollFds[i].revents & POLLHUP)
				{
					// disconnected
					retPoll--;
				}
			}
		}
	}
}