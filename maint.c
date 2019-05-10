
#include "ipc.h"



void initSharedMemory() {
  shm_id = shmget(SHM_KEY, sizeof(Ressource) * MAX_PROGRAM, IPC_CREAT | PERM);
  SYS(shm_id,"error shmget in maint.c ");
  ptr_mem_partagee= (Ressource*)SYSN(shmat(shm_id, NULL, 0),"eroooror");   ///!!!

  for(int programme = 0; programme < 1000; programme++){
   ptr_mem_partagee[programme].num_programme=programme;
   ptr_mem_partagee[programme].compile=-1;// structure vide

  // strcpy( ptr_mem_partagee[programme].nom_fichier_source, "const char *src");

 }
}
void initSem(){
 sem.val = 1;
 semid = semget(SEM_KEY, 1, IPC_CREAT | PERM);
 SYS(semid, "Error semget in maint.c initSem");
 SYS(semctl(semid, 0, SETVAL, sem), "Error semctl in maint.c initSem");
}


void destroyMemory(){
  getMemory();
  int i=shmctl(shm_id, IPC_RMID, NULL);
  SYS(i, "error semctlin maint.c destroyMemory");
  i = shmdt(ptr_mem_partagee);
  SYS(i,"error sshmdt in maint.c destroyMemory");

}


void destroySem(){
  getSem();
  int i = semctl(semid, 0, IPC_RMID, sem);
  SYS(i, "error semctl in  maint.c destroySem");

}

int main(int argc, char const *argv[])
{

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
 //getMemory();
 //printf("%d\n",  ptr_mem_partagee[22].num_programme);


//*((Ressource*)ptr_mem_partagee) = r;
//(*ptr_mem_partagee)->num_programme=22;
//(*ptr_mem_partagee)->num_programme=25;
//printf("%d\n",  (void * )(*ptr_mem_partagee));

// strncpy(msg, "argv[1]", 10);
//  printf("%s\n", msg);


//shmdt(ptr_mem_partagee);
//shmctl(shm_id, IPC_RMID, NULL);
}
