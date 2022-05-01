# include <string>
# include <string.h>
# include <sstream>
# include <iostream>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>
using namespace std;
# define MAX 1024

void tcp(int port) {
	struct sockaddr_in servaddr, cliaddr;
	int sockfd;
	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0) {
		cerr << "socket creation failed" << endl;
		exit(2);
	}

	bzero(&servaddr, sizeof(servaddr) );
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(port);

	if( connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr) ) != 0 ) {
		cerr << "connect failed" << endl;
		exit(3);
	}
	socklen_t len = (unsigned) sizeof(servaddr);
	char buff[MAX];
	for( ; ; ) {
		bzero(&buff, sizeof(buff) );

		cout << "Enter your message: ";
		cin.getline(buff, sizeof(buff) );

				
		sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr*) &servaddr, len);
		
		if(strcmp(buff, "exit") == 0) {
			cout << "disconnect" << endl;
			break;
		}

		bzero(&buff, sizeof(buff) );
		recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*) &servaddr, &len);
		
		cout << "From Server: \"" << buff << "\"" <<endl;
	}
	close(sockfd);
}

void udp(int port) {
	struct sockaddr_in servaddr;
	int sockfd;
	if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0) ) < 0) {
		cerr << "socket creation failed" << endl;
		exit(2);
	}

	bzero(&servaddr, sizeof(servaddr) );
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(port);

	if( connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr) ) != 0 ) {
		cerr << "connect failed" << endl;
		exit(3);
	}

	char buff[MAX];
	for( ; ; ) {
		bzero(&buff, sizeof(buff) );

		cout << "Enter your message: ";
		cin.getline(buff, sizeof(buff) );

		if(strcmp(buff, "exit") == 0) {
			cout << "disconnect" << endl;
			break;
		}
		socklen_t len = (unsigned) sizeof(servaddr);
		sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr*) &servaddr, len);

		bzero(&buff, sizeof(buff) );
		recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*) &servaddr, &len);
		
		cout << "From Server: \"" << buff << "\"" <<endl;
	}
	close(sockfd);
}

int main(int argc, char** argv) {
	if(argc <= 1) {
		cout << "Error: no port" << endl;
		exit(1);
	}
	if(argc >= 3) {
		cout << "Error: too many ports" << endl;
		exit(1);
	}
	int port;
	stringstream ss(argv[1]);
	ss >> port;
	
	for( ; ; ) {
		cout << "tcp or udp?" << endl;
		string str;
		cin >> str;
		
		if(str.compare("tcp") == 0) {
			tcp(port);
			return 0;
		} else if (str.compare("udp") == 0) {
			udp(port);
			return 0;
		}
		
		cout << "Error: Illegal input: " << str << "\n    Input must be either \"tcp\" or \"udp\"\n" << endl;
	}
}
