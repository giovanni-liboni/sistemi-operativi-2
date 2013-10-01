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
 *@file my_thread.h
 *@author Giovanni Liboni
 */
#ifndef MY_THREAD_H_
#define MY_THREAD_H_

#include "myio.h"
#include "def.h"
#include "util.h"
/**
 *@fn void *slave(void *arg)
 *@param arg Argomenti passati alla thread
 *@return void
 */
void *slave(void *arg);
/**
 *@fn void thread_wait(free_proc_sync *data, int max)
 *@param data Struttura per sincronizzare i processi liberi e occupati
 *@param max Numero dei processi totali
 *@return void
 *@brief Aspetta che tutti i processi siano pronti.
 */
void thread_wait(free_proc_sync *data, int max);
/**
 *@fn void thread_ready(thread_context * data)
 *@param data Context della thread
 *@return void
 *@brief Cambia lo stato della thread in pronta.
 */
void thread_ready(thread_context * data);
/**
 *@fn void thread_free(thread_context * data)
 *@param data Context della thread
 *@return void
 *@brief Incrementa i processi liberi.
 */
void thread_free(thread_context * data);
/**
 *@fn void thread_busy(thread_context * data)
 *@param data Context della thread
 *@brief Cambia lo stato della thread in occupata.
 */
void thread_busy(thread_context * data);
/**
 *@fn void thread_wait_for_ready(free_proc_sync *data, int max)
 *@param data Struttura per sincronizzare i processi liberi e occupati
 *@param max Numero dei processi totali
  *@return void
 *@brief Aspetta che almeno un processo si liberi.
 */
void thread_wait_for_ready(free_proc_sync *data, int max);
/**
 *@fn void thread_not_ready(thread_context * data)
 *@param data Context della thread
  *@return void
 *@brief Decrementa i processi liberi.
 */
void thread_not_ready(thread_context * data);
/**
 *@fn void unlock_mutex(pthread_mutex_t *mutex)
 *@param mutex Mutex su cui operare
 *@brief Esegue una lock sul mutex specificato
 */
void unlock_mutex(pthread_mutex_t *mutex);
/**
 *@fn void lock_mutex(pthread_mutex_t *mutex)
 *@param mutex Mutex su cui operare
 *@brief Esegue una unlock sul mutex specificato
 */
void lock_mutex(pthread_mutex_t *mutex);
#endif /* MY_THREAD_H_ */
