#ifndef SOCKETS_H
#define SOCKETS_H

#define MAX_CLIENTS 50
#define SOCKET_BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

typedef struct {
	int command;
	char programNameLength;
	char programName[256];
	int programNumber;
} serverCommand;

typedef enum {NotExist = -2, NotCompile = -1, RuntimeError = 0, Normal = 1} STATE;

typedef struct {
	bool addedProgram;
	int programNumber;
	STATE programState;
	int executionTime;
	int returnCode;
} serverResponse;

int setupServerSocket(int port);
int setupClientSocket(char* address, int port);

int acceptConn();
int socketConn();

void readStruct();

void writeBlock();
void readBlock();

void closeSocketRead(int socketFd);
void closeSocketWrite(int socketFd);
void closeSocket(int socketFd);

#endif