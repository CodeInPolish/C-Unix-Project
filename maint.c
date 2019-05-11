#include "ipc.h"

void initSharedMemory() {
  int shm_id = shmget(SHM_KEY, sizeof(Ressource) * MAX_PROGRAM, IPC_CREAT | PERM | 0777);
  SYS(shm_id,"error shmget in maint.c ");
  Ressource* ptr_mem_partagee= (Ressource*)SYSN(shmat(shm_id, NULL, 0),"initSharedMemory error");

   for(int programmeId = 0; programmeId < 1000; programmeId++){
      ptr_mem_partagee[programmeId].isFree = 1;
      ptr_mem_partagee[programmeId].number = programmeId;
   }
}

void initSem(){
  Sem sem;
  sem.val = 1;
  int semId = semget(SEM_KEY, 1, IPC_CREAT | PERM);
  SYS(semId, "Error semget in maint.c initSem");
  SYS(semctl(semId, 0, SETVAL, sem), "Error semctl in maint.c initSem");
}


void destroyMemory(){
  Ressource* ptr = getMemory();
  int shm_id = shmget(SHM_KEY, 0, 0);
  int i=shmctl(shm_id, IPC_RMID, NULL);
  SYS(i, "error semctlin maint.c destroyMemory");
  i = shmdt(ptr);
  SYS(i,"error sshmdt in maint.c destroyMemory");
}


void destroySem(){
  int semId = getSem();
  Sem sem;
  int i = semctl(semId, 0, IPC_RMID, sem);
  SYS(i, "error semctl in  maint.c destroySem");
}

int main(int argc, char const *argv[]){

  if(argc < 2) {
    perror("error call => maint type [opt]");
    exit(2);
  }else if(atoi(argv[1])==1){
    initSem();
    initSharedMemory();

  }else if(atoi(argv[1])==2){
    destroySem();
    destroyMemory();
  }else if(atoi(argv[1])==3 && argc==3){
    down(0);
    sleep(atoi(argv[2]));
    up(0);


  }else{
    perror("error call => maint type [opt]");
    exit(2);
 }
}
