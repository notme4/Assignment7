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
# include <ctime>
# include <sstream>
using namespace std;
# define MAX 1024
# define LOGPORT 9090

// socket descriptor for logSock,
int logSock;

int logMessage(char* buff, in_addr IPaddr, char* logmsg) {
	time_t now = time(0);
	char* date = ctime(&now);
	date[24] = '\t';
	date[25] = ' ';
	char* IP = (char*)malloc(16);
	bzero(&IP, 16);
	IP = inet_ntoa(IPaddr);
	char* msg = " was recieved from: ";

	int logmsgIndex = 0;
	for(int i = 0; i < 25; i++) {
		logmsg[logmsgIndex] = date[i];
		logmsgIndex++;
	}
	logmsg[logmsgIndex] = '\"';
	logmsgIndex++;
	for(int i = 0; i < MAX; i++) {
		if(buff[i] == '\n' || buff[i] == '\0') {
			continue;
		}
		logmsg[logmsgIndex] = buff[i];
		logmsgIndex++;
	}
	logmsg[logmsgIndex] = '\"';
	logmsgIndex++;
	for(int i = 0; i < 20; i++) {
		logmsg[logmsgIndex] = msg[i];
		logmsgIndex++;
	}
	for(int i = 0; IP[i] != '\0'; i++) {
		logmsg[logmsgIndex] = IP[i];
		logmsgIndex++;
	}

	return logmsgIndex;
}

void log(char* buff, int size, in_addr IPaddr) {
	struct sockaddr_in logaddr;

	memset(&logaddr, 0, sizeof(logaddr) );
	logaddr.sin_family = AF_INET;
	logaddr.sin_port = htons(LOGPORT);
	logaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	char* logmsg = (char*)malloc(25 + MAX + 20 + 15 + 2);
	int len = logMessage(buff, IPaddr, logmsg);
	sendto(logSock, logmsg, len, 0, (struct sockaddr*) &logaddr, sizeof(logaddr) );
	delete(logmsg);
}

void TCP_Chat(int connfd, sockaddr_in cliaddr) {
	cout << "tcp" << endl;

	char buff[MAX];
	for(;;) {
		int childpid;
		bzero(buff, MAX);
		read(connfd, buff, sizeof(buff) );
		if( (childpid = fork() ) == 0) {
			log(buff, sizeof(buff), cliaddr.sin_addr );
			exit(0);
		}
		if(strcmp(buff, "exit") == 0) {
			break;
		}
		write(connfd, buff, sizeof(buff) );
	}

}

void UDP_Chat(int connfd, sockaddr_in cliaddr, socklen_t len) {
	cout << "udp" << endl;

	int childpid;
	char buff[MAX];
	bzero(buff, MAX);
	recvfrom(connfd, buff, sizeof(buff), 0, (struct sockaddr*) &cliaddr, &len );
	if( (childpid = fork()) == 0) {
		log(buff, sizeof(buff), cliaddr.sin_addr);
		exit(0);
	}
	sendto(connfd, buff, sizeof(buff), 0, (struct sockaddr*) &cliaddr, len );
}

int main(int argc, char** argv) {
	if(argc <= 1) {
		cerr << "Error: no ports" << endl;
		exit(1);
	} if(argc > 4) {
		cerr << "Error: too many ports" << endl;
		exit(1);
	}
	fd_set fdvar;

	logSock = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in addr_in[argc];
	struct sockaddr_in cliaddr;
	int socketArray[(argc - 1) * 2];
	int maxFD = 0;
	for(int i = 0; i < argc - 1; i++) {
		stringstream ss(argv[i + 1]);
		unsigned int port;
		ss >> port;

		// create TCP socket
		if( (socketArray[i * 2] = socket(AF_INET, SOCK_STREAM, 0) ) < 0) {
			cerr << "Error: TCP socket creation failed" << endl;
			exit(3);
		}

		cout << "TCP socket succesfully created" << endl;
		
		if(socketArray[i * 2] >= maxFD) {
			maxFD = socketArray[i * 2] + 1;
		}

		addr_in[i].sin_family = AF_INET;
		addr_in[i].sin_addr.s_addr = htonl(INADDR_ANY);
		addr_in[i].sin_port = htons(port);

		// bind TCP socket
		if( bind(socketArray[i * 2], (struct sockaddr*) &addr_in[i], sizeof(addr_in[i]) ) != 0) {
			cerr << "Error: TCP bind failed at port: " << port << endl;
			exit(4);
		}

		cout << "TCP bind successful on port: " << port << endl;

		if(int num = listen(socketArray[i * 2], 10) != 0) {
			cerr << "Error: listen failed at port: " << port << endl; 
			exit(5);
		}
		
		// create UDP socket
		if( (socketArray[i * 2 + 1] = socket(AF_INET, SOCK_DGRAM, 0) ) < 0) {
			cerr << "Error: UDP socket creation failed" << endl;
			exit(3);
		}

		cout << "UDP socket succesfully created" << endl;

		if(socketArray[i * 2 + 1] >= maxFD) {
			maxFD = socketArray[i * 2 + 1] + 1;
		}

		// bind UDP socket
		if(bind(socketArray[i * 2 + 1], (struct sockaddr*) &addr_in[i], sizeof(addr_in[i]) ) != 0) {
			cerr << "Error: UDP bind failed at port: " << port << endl;
			exit(4);
		}

		cout << "UDP bind succesful at port: " << port << endl;
	}
	cout << "ready for input" << endl;
	int socketArraySize = sizeof(socketArray) / sizeof(socketArray[0]);
	for( ; ; ) {
		FD_ZERO(&fdvar);
		for(int i = 0; i < socketArraySize; i++) {
			FD_SET(socketArray[i], &fdvar);
		}
		struct timeval tv;
		tv.tv_sec = 10;
		int num = select(maxFD, &fdvar, NULL, NULL, &tv);
		for(int i = 0; i < socketArraySize; i++) {
			if(FD_ISSET(socketArray[i], &fdvar) ) {
				// if TCP server
				if(i % 2 == 0) {
					unsigned int len = sizeof(cliaddr);
					int connfd = accept(socketArray[i], (struct sockaddr*) &cliaddr, &len);
					int childpid;
					if ( (childpid = fork() ) == 0 ) {
						close(socketArray[i]);
						TCP_Chat(connfd, cliaddr);
						close(connfd);
						exit(0);
					}
					close(connfd);
				} else {
					UDP_Chat(socketArray[i], cliaddr, sizeof(cliaddr));
				}
			}
		}
	}
	return 0;
}
