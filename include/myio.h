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
 *@file myio.h
 *@author Giovanni Liboni
 */
#ifndef MY_IO_H_
#define MY_IO_H_
#include "def.h"
#include "list.h"
#include "util.h"

/**
 *@fn void print_err(char *s)
 *\brief Stampa a video l'errore ed esce
 *\param s Nome della funzione che ha generato l'errore
 *\return void
 *\brief Stampa a video l'errore ed esce
 */
void print_err(char *s);
/**
 *\fn void logsm(char *string)
 *\param *string Stringa da stampare
 *\return void
 *\brief Stampa l'indicazione del main e stampa la stringa a video
 */
void logsm(char *string);
/**
 *\fn void logsc(int id, char *string)
 *\param *string Stringa da stampare
 *\param id Id del processo da stampare
 *\return void
 *\brief Stampa l'id del processo e stampa la stringa a video
 */
void logsc(int id, char *string);
/**
 *@fn void logs(char *string)
 *\param *string Stringa da stampare
 *\return void
 *\brief Stampa a video la stringa
 */
void logs(char *string);
/**
 *\fn void logi(int num)
 *\param num Intero da stampare
 *\return void
 *\brief Stampa a video l'intero
 */
void logi(int num);
/**
 *\fn coda * readFile(char *file)
 *\param *file Nome del file
 *\return Coda con le operazioni da eseguire
 *\brief Legge dal file le operazioni e le memorizza dentro una coda
 */
coda * readFile(char *file);
/**
 *\fn void writeFile(int *data, coda *coda, char *file, int max)
 *\param *data Array dei risultati
 *\param *coda Coda con le operazioni
 *\param *file Nome del file
 *\param max Lunghezza della coda
 *\return void
 *\brief Scrive su file i risultati accoppiati con le rispettive operazioni
 */
void writeFile(const double *data, coda *coda, char *file, int max);
/**
 *\fn int readProc(char *file)
 *\param *file Nome del file
 *\return Il numero dei processi letti
 *\brief Legge il primo numero del file e lo interpreta come il numero di processi
 */
int readProc(char *file);
/**
 *@fn int is_op(char op)
 *@param op Carattere da controllare
 *@return Ritorna vero se e solo se il carattere è un'operazione
 *@brief Controlla che il carattere passato sia un'operazione
 */
int is_op(char op);

#endif /* MY_IO_H_ */
