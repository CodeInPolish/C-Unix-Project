#ifndef IPC_H
#define IPC_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include "check.h"
#include <unistd.h>
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
} Ressource;

Ressource* getMemory();
int getSem();
void up(int nb);
void down(int nb);

#endif