#ifndef FILEIO_H
#define FILEIO_H
void receiveAndWrite(char* path, int socketFd, int bufferSize);
void readAndSendFile(char* path, int socketFd, int bufferSize);
#endif