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
#define PERM 0666



typedef union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
} Sem;

Sem sem;

typedef struct Ressource {
  int num_programme;
  int nom_fichier_source;
  int compile;
  int nombre_exec;
  int temps_exec;
} Ressource;
Ressource r;

int shm_id;
Ressource* ptr_mem_partagee;
int semid;


#endif