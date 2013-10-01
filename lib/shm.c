/*
     This file is part of sistemi-operativi-2
     (C) 2013 Giovanni Liboni

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public
     License as published by the Free Software Foundation; either
     version 2.1 of the License, or (at your option) any later version.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
/**
 *@file shm.c
 *@author Giovanni Liboni
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
