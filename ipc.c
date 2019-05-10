#include "ipc.h"

//typedef int make_iso_compilers_happy;


void getSem(){

	sem.val = 1;
	semid = semget(SEM_KEY, 1,  PERM);
	SYS(semid, "Error semget in maint.c getsem");
	SYS(semctl(semid, 0, SETVAL, sem), "Error semctl in maint.c getSem");
}

void getMemory(){

	shm_id = shmget(SHM_KEY, sizeof(Ressource) * MAX_PROGRAM, PERM);
	SYS(shm_id,"error shmget in maint.c getMemory");
	ptr_mem_partagee= (Ressource*)shmat(shm_id, NULL, 0);

}

void up(int nb){
	getSem();
	printf("%d\n", semid );
	struct sembuf semb;
	semb.sem_num = nb;
	semb.sem_flg = 0;
	semb.sem_op = 1;
	SYS(semop(semid, &semb, 1), "Error smop in maint.c up()");
}

void down(int nb){
	getSem();
	printf("%d\n", semid );
	struct sembuf semb;
	semb.sem_num = nb;
	semb.sem_flg = 0;
	semb.sem_op = -1;
	printf("%d\n",semid );
	SYS(semop(semid, &semb, 1), "Error semop in maint.c down()");

}