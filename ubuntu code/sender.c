#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
//minimum arguments required for running server
#define MIN_ARGS 2

/*
 * Prints the error to the standard error stream and exits the program
 */
void signal_error(char *err_msg) {
	fprintf(stderr, err_msg);
	fprintf(stderr, "shutting down");
	exit(1);
}
int main(int argc, char *argv[]) {
	if (!argc < MIN_ARGS) {
		int bind_status, socket_file_descr, port;
		struct sockaddr_in server;

		port = atoi(argv[1]);
		socket_file_descr = socket(AF_INET, SOCK_DGRAM, 0);
		if (socket_file_descr == -1) {
			signal_error("Failed creating a socket for the server");
		}
		memset(&server, 0, sizeof(server));
		//populate the server address details
		server.sin_family = AF_INET;
		//assigning the network byte order equivalent of port no
		server.sin_port = htons(port);
		server.sin_addr.s_addr = INADDR_ANY;
		//bind socket
		bind_status = bind(socket_file_descr, (struct sockaddr *) &server,
				sizeof(server));
		if (bind_status == -1) {
			signal_error("Socket binding failed");
		}
			printf("\nWaiting for messages\n");
		while (1) {
			char buffer[400] = "";
			struct sockaddr_in client;
			socklen_t client_addr_size = sizeof client;
			int charRcvd = 0;
			charRcvd = recvfrom(socket_file_descr, buffer, 400, 0,
					(struct sockaddr *) &client, &client_addr_size);
			if (charRcvd < 0) {
				signal_error("Error in reading message");
			}
			printf("\nmsg rcvd:%s",buffer);

			int charWritten = 0;
			char ack[5] = "rcvd";
			charWritten = sendto(socket_file_descr, ack, 5, 0,
					(struct sockaddr *) &client, client_addr_size);
			printf("\nDEBUG:char written:%d",charWritten);		
		}
	} else {
		signal_error(
				"insufficient arguments. Port # is required for server boot up.");
	}
}



