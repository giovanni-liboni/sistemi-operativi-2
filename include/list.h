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
 *@file list.h
 *@author Giovanni Liboni
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
