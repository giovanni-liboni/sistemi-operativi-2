/*
     This file is part of sistemi-operativi-2
     (C) 2013 Giovanni Liboni

     This program is free software; you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public
     License as published by the Free Software Foundation; either
     version 2.1 of the License, or (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with this program; if not, write to the Free Software
     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
/**
 *@file def.h
 *@author Giovanni Liboni
 */
#ifndef DEF_H_
#define DEF_H_

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

/**
 * \struct Coda
 * @brief Struttura della coda in cui vengono memorizzare le operazioni da svolgere
 * \param *next Puntatore alla successiva struttura nella coda
 * \param op Intero che identifica l'operazioni da eseguire
 * \param num1 Primo coefficiente dell'operazione
 * \param num2 Secondo coefficiente dell'operazione
 * \param id Identifica l'id del processo
 * \param pos Indica la posizione dell'operazione
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
 *\struct status_struct
 *@brief Struttura per memorizzare i processi liberi e occupati
 */
typedef struct
{
	/// Viene memoriazzato lo stato del processo: -1 se è nuovo, 1 se è libero, 0 se è occupato
	int *array;

	/// Mutex associato all'array
	pthread_mutex_t lock;
}status_struct;
/**
 *@struct free_proc_sync
 *@brief Struttura per memorizzare quanti processi sono liberi e quanti occupati
 */
typedef struct
{
	/// Mutex associato alla variabile cont
	pthread_mutex_t lock;

	/// Variabile condition
	pthread_cond_t busy;

	/// Numero dei processi occupati
	int cont;
}free_proc_sync;
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
	double res;
	/// Posizione dell'operazione
	int pos;
	/// Operazione da eseguire
	char simbolo;
}operazione;
/**
 *@struct thread_context
 *@brief Context della singola thread. Sono presenti informazioni per la singola thread.
 */
typedef struct
{
	/// Id della thread
	int id;

	/// Contiene le informazioni sul comando
	operazione operazione;

	/// Status del processo
	status_struct *status;

	/// Mutex per l'avvio del processo
	pthread_mutex_t start;

	/// Mutex per la fine del calcolo del processo
	pthread_mutex_t finish;

	// Tiene traccia dei processi liberi
	free_proc_sync *processi_liberi;

	/// Indica se la thread deve terminare
	char kill;

	/// Se la thread è stata usata vale 1
	int used ;

}thread_context;
/**
 *@struct program_context
 *@brief Context del programma. Sono memorizzate le informazioni del programma.
 */
typedef struct
{
	/// Numero delle thread da usare
	int NTHREAD;

	/// Lunghezza coda
	int lenght_coda;

	/// Array dei risultati
	int *result_main;

	/// Coda dei comandi
	coda *coda;

	/// Context delle singole thread
	thread_context *thread;

	/// Contiene l'id delle thread
	pthread_t *threads;

	/// Status dei processi
	status_struct status;

	/// Struttura per sincronizzarsi con i processi liberi e occupati
	free_proc_sync processi_liberi;


}program_context;
#endif
