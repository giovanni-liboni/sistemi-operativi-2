/**
 *@file sem.h
 *@author Giovanni Liboni - VR363021
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
