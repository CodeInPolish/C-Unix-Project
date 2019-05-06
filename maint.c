#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SHM_KEY 100
#define PERM 0666




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


//******************************************************************************
//INIT SHARED MEMORY
//******************************************************************************

void init_shm(int size, Ressource *ptr_ressource ) {
shm_id = shmget(SHM_KEY, size, IPC_CREAT | PERM);
// checkNeg(shm_id, "Error shmget");
 ptr_mem_partagee= (Ressource*)shmat(shm_id, NULL, 0);



//checkCond(msg == (void*) -1, "Error shmat");
  //if(msg==-1)
  //perror("error memory");
}




int main(int argc, char const *argv[])
{
init_shm(sizeof(Ressource)*1000, ptr_mem_partagee);

  for(int programme = 0; programme < 1000; k++){
        ptr_mem_partagee[programme].num_programme=k;
    }
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
