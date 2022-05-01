# include <stdio.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <string.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
# include <iostream>
# include <fstream>
using namespace std;
# define MAX 1088
# define PORT 9090

int main() {
	int server_fd, bindErr;
	struct sockaddr_in servaddr, cliaddr;
	char buff[MAX];
	int opt = 1;
	if( (server_fd = socket(AF_INET, SOCK_DGRAM, 0) ) == 0) {
		cerr << "Error: socket creation failed" << endl;
		exit(1);
	}
	
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		cerr << "Error: socket set failed" << endl;
		exit(3);
	}

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	
	if( (bindErr = bind(server_fd, (struct sockaddr*) &servaddr, sizeof(servaddr) ) ) != 0) {
		cerr << "Error: bind failed" << endl;
		exit(2);
	}
	
	ofstream logFile;
	logFile.open("echo.log", fstream::app);
	
	cout << "log_s up and ready" << endl;

	char* brk = "============================================================================================\n";

	logFile << brk << "New Log Session\n" << brk << flush;
	for( ; ; ) {
		bzero(buff, MAX);
		int childpid;
		socklen_t len = (unsigned) sizeof(cliaddr);
		recvfrom(server_fd, buff, sizeof(buff), 0, (struct sockaddr*) &cliaddr, &len);
		logFile << buff << endl;
	}
}
