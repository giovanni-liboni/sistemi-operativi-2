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
 *@file util.c
 *@author Giovanni Liboni
 */
#include "util.h"

void context_init( program_context *context)
{
	// SALVO LA LUNGHEZZA DELLA CODA NEL CAMPO SPECIFICO
	context->lenght_coda = length_list(context->coda);

	//------------------------------------------------------------
	//	ALLOCO LA MEMORIA NECESSARIA PER CONTENERE L'ID DELLE
	//	DELLE SINGOLE THREAD
	//------------------------------------------------------------
	context->threads = (pthread_t *) malloc(sizeof(pthread_t)*context->NTHREAD);

	//-------------------------------------------------------------
	//	ALLOCO LA MEMORIA PER LA STRUTTURA DELLE THREADS
	//-------------------------------------------------------------
	context->thread = (thread_context *) malloc( sizeof(thread_context) * context->NTHREAD);
	context->status.array = (int *) malloc (sizeof(int) * context->NTHREAD);

	//-------------------------------------------------------------
	//	INIZIALIZZO I MUTEX PER I PROCESSI LIBERI
	//-------------------------------------------------------------
	if ( pthread_mutex_init(&context->processi_liberi.lock, NULL))
		print_err("pthread_mutex_init");
	if ( pthread_mutex_init(&context->status.lock, NULL))
		print_err("pthread_mutex_init");
	if ( pthread_cond_init(&context->processi_liberi.busy, NULL))
		print_err("pthread_cond_init");
}
void creat_thread(program_context *context)
{
	pthread_attr_t attr;
	int i;

	// INIZIALIZZO L'ATTRIBUTO DELLE THREAD
	if (pthread_attr_init(&attr) < 0)
		print_err("pthread_attr_init");

	// SETTO LE THREAD CON JOINABLE
	if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE))
		print_err("pthread_attr_setdetachstate");

	for( i = 0 ; i < context->NTHREAD ; i++ )
	{
		// PASSO ALLE THREAD IL PUNTATORE DELLA STRUTTURA CON LE INFORMAZIONI
		// NECESSARIE
		context->thread[i].id = i;
		context->thread[i].status = &context->status;
		context->thread[i].status->array[i] = -1;
		context->thread[i].processi_liberi = &context->processi_liberi;
		context->thread[i].kill = 'N';
		context->thread[i].used = 0;

		//	INIZIALIZZO I SEMAFORI DELLA THREAD
		if (pthread_mutex_init(&context->thread[i].start , NULL))
			print_err("pthread_mutex_init");

		if (pthread_mutex_init(&context->thread[i].finish , NULL))
			print_err("pthread_mutex_init");

		// CREO LA THREAD
		if (pthread_create(&context->threads[i], &attr, slave, &context->thread[i]))
			print_err("pthread_create");

		// BLOCCO LA THREAD SULLO START
		lock_mutex(&context->thread[i].start);

	}
	// DISTRUGGO L'ATTRIBUTO
	if(pthread_attr_destroy(&attr))
		print_err(("pthread_attr_destroy"));

	//	ASPETTO CHE LE THREADS SIANO TUTTE PRONTE
	thread_wait(&context->processi_liberi, context->NTHREAD);

	// STAMPO
	logsm("Thread pronte\n");
}
void wait_and_stop(program_context *context,  double *result_main)
{
	int i;
	//-------------------------------------------------------------
	//	RECUPERO L'ULTIMO RISULTATO DI TUTTI I PROCESSI
	//	ATTENDENDO CHE ABBIANO TUTTI FINITO
	//-------------------------------------------------------------

	for(i = 0 ; i < context->NTHREAD ; i++)
	{
		//-------------------------------------------------------------
		//	ASPETTO CHE IL PROCESSO ABBIA FINITO
		//-------------------------------------------------------------
		lock_mutex(&context->thread[i].finish);

		//-------------------------------------------------------------
		//	RECUPERO IL PRECEDENTE RISULTATO E LO MEMORIZZO NELL'ARRAY
		//	DEI RISULTATI
		//-------------------------------------------------------------

		if (context->thread[i].used)
			result_main[context->thread[i].operazione.pos] = context->thread[i].operazione.res;


	}
	logs("\n\t-------- FINE CALCOLI --------\n\n");

	//-------------------------------------------------------------
	//	AVVERTO I PROCESSI CHE DEVONO TERMINARE
	//-------------------------------------------------------------

	for(i = 0 ; i < context->NTHREAD ; i++)
	{
		// SCRIVO NELLA STRUTTURA IL SEGNALE DI TERMINAZIONE
		context->thread[i].kill = 'K';

		//	SBLOCCO LA THREAD
		unlock_mutex(&context->thread[i].start);

		// ESEGUO IL JOIN DELLA THREAD
		if (pthread_join( context->threads[i], NULL))
			print_err("pthread_join");
	}
	logsm("Segnale arresto inviato\n");
}
void dealloc_all(program_context *context)
{
	int i;
	logs("\n\tDealloco tutte le risorse.\n");

	// DEALLOCO LA CODA
	destruct_coda(context->coda);

	// DEALLOCO I MUTEX DELLE THREADS
	for(i = 0 ; i < context->NTHREAD ; i++)
	{
		unlock_mutex(&context->thread[i].start);

		if (pthread_mutex_destroy(&context->thread[i].start) )
			print_err("pthread_mutex_destroy");

		unlock_mutex(&context->thread[i].finish);

		if (pthread_mutex_destroy(&context->thread[i].finish))
			print_err("pthread_mutex_destroy");
	}

	// DEALLOCO IL MUTEX DELLA CONDITION VARIABLE
	if (pthread_mutex_destroy(&context->status.lock))
		print_err("pthread_mutex_destroy");

	// DEALLOCO LA MEMORIA ALLOCATA
	free(context->status.array);
	free(context->threads);
	free(context->thread);

}
double calcolo(int num1, int num2, char op)
{
	switch(op)
	{
		case '+': return num1 + num2;
		case '-': return num1 - num2;
		case '*': return num1 * num2;
		case '/': return (num1 / num2);
	}

	// CASO DI RITORNO DI DEFAULT
	return 1;
}
int char_to_int(char *car)
{
	switch(*car)
	{
	case '+': return 0;
	case '-': return 1;
	case '*': return 2;
	case '/': return 3;
	default: return atoi(car);
	}
}
char int_to_char(int num)
{
	switch(num)
	{
	case 0: return '+';
	case 1: return '-';
	case 2: return '*';
	case 3: return '/';
	default: return 'e';
	}
}
