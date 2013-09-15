/**
 * @file def.h
 * @author Giovanni Liboni - VR363021
 */
#ifndef DEF_H_
#define DEF_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>

//#include "myio.h"
///Numero dei semafori generali
#define NUMSEMSYS 		10
///Numero del semaforo MUTEX
#define MUTEX 			1
///Numero del semaforo MUTEXRES
#define MUTEXRES 		2
///Numero del semaforo SEMPROCFREE
#define SEMPROCFREE 	3
///Numero del semaforo SEMREADY
#define SEMREADY 		4
///Numero del semaforo SEMDATARECEIVE
#define SEMDATARECEIVE 	5

#ifndef IPC_RMID
///Se non definito definisco IPC_RMID
#define IPC_RMID		0
#endif
/**
 *\struct Coda
 *@brief Struttura della coda in cui vengono memorizzare le operazioni da svolgere
 */
struct Coda{
	/// Puntatore alla successiva struttura nella coda
	struct Coda *next;
	/// Identifica l'operazioni da eseguire
	char op;
	/// Primo numero dell'operazione
	int num1;
	/// Secondo numero dell'operazione
	int num2;
	/// Identifica l'id del processo
	int id;
	/// Indica la posizione dell'operazione
	int pos;
};
/**
 *@typedef struct Coda coda
 *@brief Struttura della coda
 */
typedef struct Coda coda;
/**
 *@struct operazione
 *@brief Struttura per memorizzare l'operazione
 */
typedef struct
{
	/// Primo numero dell'operazione
	int num1;
	/// Secondo numero dell'operazione
	int num2;
	/// Risultato dell'operazione
	int res;
	/// Posizione dell'operazione
	int pos;
	/// Operazione da eseguire
	char simbolo;
	/// Carattere per terminare l'esecuzione del calcolo
	char kill;

}operazione;
/**
 *@struct sync_id
 *@brief Struttura per memorizzare informazioni relative agli id per la sincronizzazione
 */
typedef struct
{
	/// Id dei semafori generali
	int semid_sys;
	/// Id dei semafori per sincronizzare la partenza dei processi
	int semid_start;
	/// Id dei semafori per sincronizzare la fine
	int semid_finish;
	/// Id del segmento di memoria condivisa per l'operazione
	int shmid_operazione;
	/// Id del segmento di memoria condivisa per lo stato e il risultato
	int shmid_status;

}sync_id;
/**
 *@struct status_and_res_struct
 *@brief Struttura per memorizzare informazioni relative al risultato del comando e allo stato del processo.
 */
typedef struct
{
	/// Posizione del risultato all'interno del file di configurazione
	int pos;
	/// Risultato del comando
	int res;
	/// Stato del processo
	int status;

}status_and_res_struct;
/**
 *@struct slave_context
 *@brief Struttura privata di ogni processo per memorizzare le informazioni
 */
typedef struct
{
	/// Memoria condivisa per il passaggio dei comandi
	operazione *shm_operazione;
	/// Memoria condivisa per il passaggio della struttura status_and_res_struct
	status_and_res_struct *shm_status;
	/// Struttura per memorizzare il singolo comando
	operazione operazione_singola;
	/// Struttura per memorizzare informazioni relative alla sincronizzazione
	sync_id sync_id;
	/// Id del processo
	int id;

}slave_context;
/**
 *@struct program_context
 *@brief Context del programma principale
 */
typedef struct
{
	/// Coda dei comandi
	coda *coda;
	/// Lunghezza della coda
	int lenght_coda;
	/// Numero dei processi
	int NPROC;
	/// Struttura per memorizzare informazioni relative alla sincronizzazione
	sync_id sync_id;
	/// Memoria condivisa per il passaggio dei comandi
	operazione *shm_operazione;
	///  Memoria condivisa per il passaggio della struttura status_and_res_struct
	status_and_res_struct *shm_status;
}program_context;

#endif
