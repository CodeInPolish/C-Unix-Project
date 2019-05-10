#include "fileIO.h"
#include "global.h"

void receiveAndWrite(char* path, int socketFd, int bufferSize){
	int readChar;
	char* buffer = SYSN(malloc(bufferSize*sizeof(char)), "malloc error");
	int fileFd = SYS(open(path, O_WRONLY|O_CREAT|O_TRUNC), "open error");
	while((readChar = read(socketFd, buffer, bufferSize)) > 0){
		write(fileFd, buffer, readChar);
		memset(buffer, 0, bufferSize);
	}
	write(fileFd, buffer, strlen(buffer));
	SYS(fchmod(fileFd, S_IRUSR|S_IWUSR|S_IXUSR),"set permission error");
	free(buffer);
	close(fileFd);
}

void readAndSendFile(char* path, int socketFd, int bufferSize){
	int fileFd = open(path, O_RDONLY);
	int readChar;
	char* buffer = SYSN(malloc(bufferSize*sizeof(char)), "malloc error");
	while((readChar = read(fileFd, buffer, bufferSize)) > 0){
		send(socketFd, buffer, readChar, 0);
		memset(buffer, 0, bufferSize);
	}
	send(socketFd, buffer, strlen(buffer), 0);
	free(buffer);
	close(fileFd);
}