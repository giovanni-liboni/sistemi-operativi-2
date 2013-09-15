/**
 * @file util.h
 * @author Giovanni Liboni - VR363021
 */
#ifndef UTIL_H_
#define UTIL_H_

#include "list.h"
#include "myio.h"
#include "def.h"
#include "my_thread.h"

/**
 *\fn int op_to_int(char *string)
 *\param *string Puntatore al carattere dell'operazione da trasformare in intero
 *\return L'intero associato all'operazione
 *\brief trasforma il carattere dell'operazione in un intero
 */
int char_to_int(char *string);
/**
 *\fn char int_to_op(int op)
 *\param op intero rappresentante l'operazione
 *\return L'operazione in formato char
 *\brief Trasforma un intero rappresentante l'operazione in un carattere char rappresentante l'operazione
 */
char int_to_char(int op);
/**
 *@fn double calcolo(int num1, int num2, char op)
 *@param num1 Primo numero
 *@param num2 Secondo numero
 *@param op Operazione da eseguire
 *@return Risultato del calcolo effettuato.
 *@brief Calcola il risultato dell'operazione.
 */
double calcolo(int num1, int num2, char op);
/**
 *@fn void dealloc_all(program_context *context)
 *@param context Context del programma main
  *@return void
 *@brief Dealloca tutte le risorse allocate.
 */
void dealloc_all(program_context *context);
/**
 *@fn void wait_and_stop(program_context *context, double *result_main)
 *@param context Context del programma main
 *@param result_main Array dei risultati
  *@return void
 *@brief Aspetta che tutte le thread abbiano finito di calcolare, salva l'ultimo risultato
 * comuputato e poi termina le thread.
 */
void wait_and_stop(program_context *context,  double *result_main);
/**
 *@fn void creat_thread(program_context *context)
 *@param context Context del programma main
  *@return void
 *@brief Crea le thread passando come parametro la struttura thread_context.
 */
void creat_thread(program_context *context);
/**
 *@fn void context_init( program_context *context)
 *@param context Context del programma main
  *@return void
 *@brief Inizializza il context del main.
 */
void context_init( program_context *context);
#endif
