#ifndef SOCKETS_H
#define SOCKETS_H

#define SOCKET_BUFFER_SIZE 1024

struct {
	int command;
	char programNameLength;
	char programName[256];
	int programNumber;
} serverCommand;

typedef enum {NotExist = -2, NotCompile = -1, RuntimeError = 0, Normal = 1} STATE;

struct {
	bool addedProgram;
	int programNumber;
	STATE programState;
	int executionTime;
	int returnCode;
} serverResponse;

#endif