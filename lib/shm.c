/**
 *@file shm.c
 *@author Giovanni Liboni - VR363021
 */
#include "shm.h"

int shm_creat(int size, int tok)
{
	static int shmid;

	if((shmid = shmget(ftok("/tmp", tok), size, IPC_CREAT | 0666)) < 0)
	{
		if (shmctl(shmid, IPC_RMID, (struct shmid_ds *) NULL) < 0)
			print_err("shmctl");

		print_err("shmget");
	}
	return shmid;
}
int * shm_attack(int shmid)
{
	int * shm;
	if((shm = (int *) shmat(shmid , 0 , 0)) == (int *) -1)
		print_err("shmat");

	return shm;
}

void shm_detach(const void *shmaddr)
{
	if (shmdt(shmaddr))
		print_err("Shmdt");
}
