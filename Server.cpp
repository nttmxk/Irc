#include "Server.hpp"

int main(int argc, char *argv[]) {
	int port_number;
	char buffer[BUF_LEN];
	struct socketaddr_in server_addr;
	int client_fd;
	int msg_size;
	int i;

	port_number = atoi(argv[1]);
	// 1. Socket create
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << "Client: Can't open stream socket\n";
		exit(0);
	}
	// 2. Connecting
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_number);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (connect(client_fd, (struct socketaddr *) &server_addr, sizeof(server_addr)) < 0) {
		std::cerr << "Connection failed\n";
		exit(0);
	} else {
		// 3. Receiving packets
		std::cout << "Connection Success\n";
		i = 0;
		std::cout << "port_number: " << port_number << " starts reading\n";
		msg_size = 1;
		while ((msg_size = read(client_fd, buffer, BUF_LEN)) > 0) {
			i++;
			if (msg_size != BUF_LEN) buffer[msg_size] = '\0'; // Buffer is not flushed, so garbage value can be printed.
			std::cout << buffer << '\n';
			if (i > 4) // Limit the number of packets received.
				break;
		}
		close(client_fd);
	}

}