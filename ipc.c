#include "ipc.h"

int getSem(){
	Sem sem;
	sem.val = 1;
	int semId = semget(SEM_KEY, 1,  PERM);
	SYS(semId, "Error semget in maint.c getsem");
	SYS(semctl(semId, 0, SETVAL, sem), "Error semctl in maint.c getSem");
	return semId;
}

Ressource* getMemory(){
	int shm_id = shmget(SHM_KEY, sizeof(Ressource) * MAX_PROGRAM, PERM);
	SYS(shm_id,"error shmget in maint.c getMemory");
	Ressource* ptr_mem_partagee = (Ressource*)shmat(shm_id, NULL, 0);
	return ptr_mem_partagee;
}

void up(int nb){
	int semId = getSem();
	struct sembuf semb;
	semb.sem_num = nb;
	semb.sem_flg = 0;
	semb.sem_op = 1;
	SYS(semop(semId, &semb, 1), "Error smop in maint.c up()");
}

void down(int nb){
	int semId = getSem();
	struct sembuf semb;
	semb.sem_num = nb;
	semb.sem_flg = 0;
	semb.sem_op = -1;
	SYS(semop(semId, &semb, 1), "Error semop in maint.c down()");
}