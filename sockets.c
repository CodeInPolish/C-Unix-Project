#include "global.h"
#include "sockets.h"

int setupClientSocket(int port) {
	int socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketFd == -1) {
		exit(1);
	}
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_aton(SERVER_IP, &addr.sin_addr);
	if(connect(socketFd, (struct sockaddr*) &addr, sizeof(addr)) == -1){
		exit(1);
	}
	return socketFd;
}

int setupServerSocket(int port) {
	int socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketFd == -1) {
		exit(1);
	}
	struct sockaddr_in addr;
	int returnCode;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr= htonl(INADDR_ANY);
	returnCode = bind(socketFd, (struct sockaddr*) &addr, sizeof(addr));
	if(returnCode == -1) {
		exit(1);
	}
	listen(socketFd, MAX_CLIENTS);
	return socketFd;
}

void closeSocketRead(int socketFd) {
	int returnCode = shutdown(socketFd, 2);
	if(returnCode == -1) {
		exit(1);
	}
}

void closeSocketWrite(int socketFd) {
	int returnCode = shutdown(socketFd, 1);
	if(returnCode == -1) {
		exit(1);
	}
}

void closeSocket(int socketFd) {
	int returnCode = shutdown(socketFd, 0);
	if(returnCode == -1) {
		exit(1);
	}
}
