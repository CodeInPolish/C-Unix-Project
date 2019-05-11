#ifndef IPC_H
#define IPC_H


#include "global.h"
#define MAX_PROGRAM 1000
#define SEM_KEY 123
#define SHM_KEY 321
#define PERM 0777

typedef union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
} Sem;

typedef struct Ressource {
  	int number;
  	char name[256];
  	int compile;
  	int execNumber;
  	int execTime;
	int isFree;
} Ressource;

Ressource* getMemory();
int getSem();
void up(int nb);
void down(int nb);

#endif