/**
 *@file list.h
 *@author Giovanni Liboni - VR363021
 */
#ifndef LIST_H_
#define LIST_H_

#include "def.h"

/**
 *\fn coda *construct_list(int id, int num1, int op, int num2, coda *next)
 *Costruttore per la coda
 *\param id Processo a cui far eseguire il comando
 *\param num1 Primo coefficiente
 *\param num2 Secondo coefficiente
 *\param *next Puntatore alla struttura successiva
 *\return Puntatore ad una nuova coda
 */
coda *construct_list(int id, int num1, int op, int num2, coda *next);
/**
 *\fn void destruct_coda(coda *this)
 *\param *this Puntatore alla coda da distruggere
 *\return void
 */
void destruct_coda(coda *this);
/**
 *\fn int length_list(coda *this)
 *\param *this Puntatore alla lista
 *\brief Calcola la lunghezza della lista
 *\return Ritorna la lunghezza della lista
 */
int length_list(coda *this);
#endif /* LIST_H_ */
