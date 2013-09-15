/**
 * @file util.h
 * @author Giovanni Liboni - VR363021
 */
#ifndef UTIL_H_
#define UTIL_H_
#include "def.h"
#include "list.h"
#include "sem.h"
#include "shm.h"
#include "myio.h"
/**
 *@fn char int_to_op(int op)
 *@param op intero rappresentante l'operazione
 *@return L'operazione in formato char
 *@brief Trasforma un intero rappresentante l'operazione in un carattere char rappresentante l'operazione
 */
char int_to_char(int op);
/**
 *@fn int op_to_int(char *car)
 *@param car Carattere da trasformare in un numero. Se è un numero verrà trasformato nell'intero.
 *Se invece è un'operazione verrà trasformato in 0,1,2,3 a seconda dell'operazione.
 *
 *	Funzione per trasformare un carattere in un numero.
 */
int char_to_int(char *car);
/**
 *@fn void context_init(program_context *context, coda *coda)
 *@param context Puntatore al context del programma main
 *@param coda	Puntatore alla coda di comandi
 *@brief Inizializza il context del main, creando i semafori e la memoria condivisa.
 */
void context_init(program_context *context, coda *coda);
/**
 *@fn void dealloc_data(program_context *data)
 *@param data Context da deallocare
 *@brief Dealloca la struttura program_context
 */
void dealloc_data(program_context *data);
/**
 *@fn void calcolo_and_save_operazione(slave_context *slave)
 *@param slave Context su cui lavorare
 *@brief Calcola il comando e salva il risultato nella struttura
 */
void calcolo_and_save_operazione(slave_context *slave);
/**
 *@fn void read_operazione(slave_context *slave)
 *@param slave Context dello slave
 *@brief Legge il comando dalla zona condivisa e lo salva nella struttura privata
 */
void read_operazione(slave_context *slave);
/**
 *@fn void wait_and_stop_process(program_context *context, pid_t *pid, double *result_main)
 *@param context Context del programma
 *@param pid Array dei pid dei processi figli
 *@param result_main Array dei risultati del main
 *@brief Aspetta la fine di tutti i processi, salva l'ultimo risultato calcolato, manda il segnale di arresto
 * e aspetta che tutti i figli terminino
 */
void wait_and_stop_process(program_context *context, pid_t *pid, double *result_main);
/**
 *@fn double calcolo(int num1, int num2, char op)
 *@param num1 Primo numero
 *@param num2 Secondo numero
 *@param op Operazione da eseguire
 *@brief Calcola il risultato dell'operazione
 */
double calcolo(int num1, int num2, char op);
/**
 *@fn void coda_handler(program_context *context, double *result_main)
 *@param context Context del programma
 *@param result_main Array dei risultati del main
 *@brief Funzione che gestisce la coda. Ogni comando della coda lo esegue sull'id del processo specificato
 * e se l'id è uguale a 0 cerca il primo processo libero per assegnargli il comando.
 */
void coda_handler(program_context *context, double *result_main);
/**
 *@fn void save_result(program_context *context, double *result_main)
 *@param context Context del programma
 *@param result_main Array dei risultati del main
 *@return void
 *@brief Salva il risultato dalla memoria condivisa nell'array result_main
 */
void save_result(program_context *context, double *result_main);
/**
 *@fn void search_free_process(program_context *context)
 *@param context Context del programma
 *@brief Cerca un processo libero. Se non lo trova aspetta che almeno un processo sia libero e assegna
 * all'id della coda l'id del processo libero
 */
void search_free_process(program_context *context);
#endif
