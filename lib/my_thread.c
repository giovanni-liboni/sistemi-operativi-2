/**
 *@file my_thread.c
 *@author Giovanni Liboni - VR363021
 */
#include "my_thread.h"
void *slave(void *arg)
{

	thread_context *context;
	context = (thread_context *)arg;

	// MEMORIZZO IN ID L'ID DELLA THREAD
	int id =context->id;

	// ASPETTO UN SECONDO
	sleep(1);

	logsc(id, "Creata\n");
	while(1)
	{
		//	MODIFICO LO STATO DELLA THREAD IN LIBERA
		thread_free(context);

		// DECREMENTO IL NUMERO DI THREADS LIBERE
		thread_ready(context);

		//-----------------------------------------------
		//	ASPETTO UN COMANDO
		//-----------------------------------------------
		logsc(id, "Aspetto un comando\n");
		lock_mutex(&context->start);

		//	MODIFICO LO STATO DELLA THREAD IN OCCUPATA
		thread_busy(context);

		// AUMENTO IL NUMERO DI THREADS OCCUPATE
		thread_not_ready(context);

		//-----------------------------------------------
		//	CONTROLLO SE È ARRIVATO IL SEGNALE DI FINE
		//-----------------------------------------------
		if(context->kill == 'K')
		{
			logsc(id, "Termino\n");
			// ESCO
			return (void *) NULL;
		}
		logsc(id, "Processo partito\n");

		// SALVO IL RISULTATO NELLA STRUTTURA PRIVATA DELLA THREAD

		context->operazione.res = calcolo(context->operazione.num1, context->operazione.num2, context->operazione.simbolo);

		logsc(id, "Finito di calcolare\n");

		//-----------------------------------------------
		//	AVVERTO DI AVER FINITO IL LAVORO
		//-----------------------------------------------
		unlock_mutex(&context->finish);
	}

	// ESCO
	return NULL;
}
void thread_wait(free_proc_sync *data, int max)
{
	// BLOCCO IL MUTEX ASSOCIATO ALLA VARIBILE CONDITION
	lock_mutex(&data->lock);

	// INIZIALIZZO IL CONTATORE AL NUMERO DI PROCESSI MASSIMI
	data->cont = max;

	// ASPETTO FINO A QUANDO IL NUMERO DI PROCESSI È INFERIORE A 0, OVVERO SONO TUTTI PRONTI
	while(data->cont > 0)
	{
		if ( pthread_cond_wait(&data->busy, &data->lock))
			print_err("pthread_cond_wait");
	}

	// I PROCESSI SONO TUTTI PRONTI E RIPORTO IL CONTATORE A MAX
	data->cont = max;

	// SBLOCCO IL MUTEX ASSOCIATO ALLA VARIABILE CONDITION
	unlock_mutex(&data->lock);
}
void thread_wait_for_ready(free_proc_sync *data, int max)
{
	// BLOCCO IL MUTEX ASSOCIATO ALLA VARIBILE CONDITION
	lock_mutex(&data->lock);

	// ASPETTO CHE ALMENO UN PROCESSO SIA LIBERO
	while(data->cont > max)
	{
		if (pthread_cond_wait(&data->busy, &data->lock))
			print_err("pthread_cond_wait");
	}

	// SBLOCCO IL MUTEX ASSOCIATO ALLA VARIABILE CONDITION
	unlock_mutex(&data->lock);
}
void thread_ready(thread_context * data)
{
	// BLOCCO IL MUTEX ASSOCIATO ALLA VARIBILE CONDITION
	lock_mutex(&data->processi_liberi->lock);

			// DECREMENTO I PROCESSI OCCUPATI
			data->processi_liberi->cont--;
			if ( pthread_cond_signal(&data->processi_liberi->busy))
				print_err("pthread_cond_signal");

	// SBLOCCO IL MUTEX ASSOCIATO ALLA VARIABILE CONDITION
	unlock_mutex(&data->processi_liberi->lock);
}
void thread_not_ready(thread_context * data)
{
	// BLOCCO IL MUTEX ASSOCIATO ALLA VARIBILE CONDITION
	lock_mutex(&data->processi_liberi->lock);

			// INCREMENTO I PROCESSI OCCUPATI
			data->processi_liberi->cont++;

	// SBLOCCO IL MUTEX ASSOCIATO ALLA VARIABILE CONDITION
	unlock_mutex(&data->processi_liberi->lock);
}
void thread_free(thread_context * data)
{
	// BLOCCO IL MUTEX ASSOCIATO ALL'ARRAY DELLO STATO
	lock_mutex(&data->status->lock);

		// PONGO LA FLAG A 1 ( PROCESSO LIBERO E USATO ALMENO UNA VOLTA )
		data->status->array[data->id] = 1;

	// SBLOCCO IL MUTEX ASSOCIATO ALL'ARRAY DELLO STATO
	unlock_mutex(&data->status->lock);
}
void thread_busy(thread_context * data)
{
	// BLOCCO IL MUTEX ASSOCIATO ALL'ARRAY DELLO STATO
	lock_mutex(&data->status->lock);

		// PONGO LA FLAG A 0 ( PROCESSO OCCUPATO )
		data->status->array[data->id] = 0;

	// SBLOCCO IL MUTEX ASSOCIATO ALL'ARRAY DELLO STATO
	unlock_mutex(&data->status->lock);
}
void lock_mutex(pthread_mutex_t *mutex)
{
	if (pthread_mutex_lock(mutex))
		print_err("lock_mutex");
}
void unlock_mutex(pthread_mutex_t *mutex)
{
	if (pthread_mutex_unlock(mutex))
		print_err("unlock_mutex");
}
