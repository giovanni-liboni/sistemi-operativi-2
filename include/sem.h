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
 *@file sem.h
 *@author Giovanni Liboni
 */
#ifndef SEM_H_
#define SEM_H_

#include "def.h"
#include "util.h"
/**
 *\union semun
 *\brief Struttura per il semaforo
 */
union semun {
	///Usato per IPC_STAT e IPC_SET
	int val;
	/// IPC_INFO
	struct semid_ds *buf;
	/// Usato per GET_ALL E SET_ALL
	ushort *array;
} st_sem; /**< union variable*/
/**
 *@fn void sem_wait (int sem_number, int semid)
 *\param sem_number Numero del semaforo
 *\param semid Id del semaforo
 *\return void
 *\brief Esegue una wait sul semaforo specificato
 */
void sem_wait (int sem_number, int semid);
/**
 *\fn void sem_signal (int sem_number, int semid)
 *\param sem_number Numero del semaforo
 *\param semid Id del semaforo
 *\return void
 *\brief Esegue una signal sul semaforo specificato
 */
void sem_signal (int sem_number, int semid);
/**
 *\fn int sem_creat(int num_sem, char key)
 *\param num_sem Lunghezza dell'array di semafori
 *\param key Chiave per il semaforo
 *\return Id del semaforo creato
 *\brief Crea un semaforo e  ritorna l'id del semaforo creato
 */
int sem_creat(int num_sem, char key);
/**
 *\fn void sem_set(int semid, int semnum, int val)
 *\param semid Id del semaforo
 *\param semnum Numero del semaforo
 *\param val Valore che si vuole settare
 *\return void
 *\brief Setta il semaforo specificato con il valore specificato in val
 */
void sem_set(int semid, int semnum, int val);
#endif
