#include "global.h"
#include "sockets.h"

int setupClientSocket(int port) {
	int socketFd;
	SYS((socketFd = socket(AF_INET, SOCK_STREAM, 0)), "createSocket");
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_aton(SERVER_IP, &addr.sin_addr);
	SYS(connect(socketFd, (struct sockaddr*) &addr, sizeof(addr)), "connect");
	return socketFd;
}

int setupServerSocket(int port) {
	int socketFd;
	SYS((socketFd = socket(AF_INET, SOCK_STREAM, 0)), "createSocket");
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr= htonl(INADDR_ANY);
	SYS(bind(socketFd, (struct sockaddr*) &addr, sizeof(addr)), "bind");
	listen(socketFd, MAX_CLIENTS);
	return socketFd;
}

void closeSocketRead(int socketFd) {
	SYS(shutdown(socketFd, 2),"closeReadSocket");
}

void closeSocketWrite(int socketFd) {
	SYS(shutdown(socketFd, 1),"closeWriteSocket");
}

void closeSocket(int socketFd) {
	SYS(shutdown(socketFd, 0),"closeSocket");
}
