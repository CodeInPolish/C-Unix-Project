#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define MAXBUFFERSIZE 10

int main(int argc, char** argv){
	int* ptr;
	while(1){
		ptr = *ptr + 1;
	}
	printf("%s\n", ptr);
}