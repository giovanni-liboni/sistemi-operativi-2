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
 *@file sem.c
 *@author Giovanni Liboni
*/
#include "sem.h"

void sem_wait (int sem_number, int semid)
{
	static struct sembuf wait_w;
	wait_w.sem_op  = -1;
	wait_w.sem_num = sem_number;
	wait_w.sem_flg = 0;

	if (semop(semid,&wait_w,1)==-1)
		print_err("*sem_wait* semop");


}
void sem_signal (int sem_number, int semid)
{
	static struct sembuf signal_s;
	signal_s.sem_op=1;
	signal_s.sem_num=sem_number;
	signal_s.sem_flg = 0;

	if (semop(semid,&signal_s,1)==-1)
		print_err("*sem_signal* semop");


}
int sem_creat(int num_sem, char key)
{
	static int semid,i;

	/* Genero un id univoco per il semaforo*/
	semid = semget(ftok("/usr", key), num_sem ,IPC_CREAT | 0666 |IPC_EXCL);

	/** Controllo che il semaforo sia stato creato, altrimenti dealloco il semaforo*/
	if(semid == -1)
	{
		semctl(semid, 0, IPC_RMID, 0);
		print_err("*init_sem* semget");
	}

	/* Inizializzo i semafori a 0 */
	st_sem.val = 0;
	for(i = 0 ; i < num_sem ; i++)
	    if (semctl(semid, i, SETVAL, st_sem) == -1)
	    {
	    	if (semctl(semid, 0, IPC_RMID, 0) == -1)
	    		 print_err("semctl");

	    	print_err("semctl");
	    }

	return semid;
}
void sem_set(int semid, int semnum, int val)
{
	/* Setto il valore */
	st_sem.val = val;

    if (semctl(semid, semnum, SETVAL, st_sem) == -1)
    {
    	if (semctl(semid, 0, IPC_RMID, 0) == -1)
    		    		 print_err("semctl");

    	print_err("semctl");

    }
}
