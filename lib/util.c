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
 *@file util.c
 *@author Giovanni Liboni
 */
#include "util.h"
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
char int_to_char(int op)
{
	switch(op)
	{
	case 0: return '+';
	case 1: return '-';
	case 2: return '*';
	case 3: return '/';
	default: return 'e';
	}
}
void dealloc_data(program_context *data)
{
	// RIMUOVO I SEMAFORI SPECIFICATI
	if(semctl(data->sync_id.semid_finish, 0, IPC_RMID, 0) < 0)
		print_err("semctl");
	if(semctl(data->sync_id.semid_start, 0, IPC_RMID, 0) < 0)
		print_err("semctl");
	if(semctl(data->sync_id.semid_sys, 0, IPC_RMID, 0) < 0)
		print_err("semctl");

	//	DISTACCO LA MEMORIA CONDIVISA
	shm_detach(data->shm_operazione);
	shm_detach(data->shm_status);

	// RIMUOVO LA MEMORIA CONDIVISA
	if(shmctl(data->sync_id.shmid_operazione, IPC_RMID, (struct shmid_ds *) NULL) < 0)
		print_err("shmctl");
	if(shmctl(data->sync_id.shmid_status, IPC_RMID, (struct shmid_ds *) NULL) < 0)
		print_err("shmctl");
}
void context_init(program_context *context, coda *coda)
{

	//------------------------------------------------------------
	//	TROVO LA LUNGHEZZA DELLA LISTA, COSÌ TROVO IL NUMERO
	//	DI OPERAZIONI DA FARE
	//------------------------------------------------------------
	context->lenght_coda = length_list(coda);
	context->coda = coda;

	//------------------------------------------------------------
	//	CREO I SEMAFORI
	//------------------------------------------------------------

	context->sync_id.semid_start = sem_creat(context->NPROC + 1, 'r');
	context->sync_id.semid_finish = sem_creat(context->NPROC + 1, 'P');
	context->sync_id.semid_sys = sem_creat(NUMSEMSYS, 'X');

	//------------------------------------------------------------
	//	SETTO I SEMAFORI
	//------------------------------------------------------------

	sem_set( context->sync_id.semid_sys, MUTEX, 1);
	sem_set( context->sync_id.semid_sys, MUTEXRES , 1);
	sem_set( context->sync_id.semid_sys, SEMPROCFREE, context->NPROC);
	sem_set( context->sync_id.semid_sys, SEMDATARECEIVE, 0);

	//------------------------------------------------------------
	//	CREO LA MEMORIA CONDIVISA
	//------------------------------------------------------------

	context->sync_id.shmid_operazione = shm_creat(sizeof(operazione), 'a');
	context->sync_id.shmid_status = shm_creat(sizeof(status_and_res_struct) *context->NPROC, 'b');

	//-------------------------------------------------------------
	//	ATTACCO LA MEMORIA CONDIVISA
	//-------------------------------------------------------------

	context->shm_operazione = (operazione *) shm_attack(context->sync_id.shmid_operazione);
	context->shm_status = (status_and_res_struct *) shm_attack(context->sync_id.shmid_status);

}
void creat_process(pid_t *pid, program_context *context)
{
	char
		id[16],
		semid_start[16],
		semid_finish[16],
		semid_sys[16],
		shm_operazione[16],
		shm_info[16];
	int i;

	//-------------------------------------------------------------
	//	USO LA STRUTTURA OPERAZIONE PER LA SINCRONIZZAZIONE COL
	//	MAIN DA PARTE DEI FIGLI CHE STO PER CREARE
	//-------------------------------------------------------------

	sem_wait(MUTEX, context->sync_id.semid_sys);

		//	USO LA MEMORIA CONDIVISA PER IL PASSSAGGIO DELL'OPERAZIONE PER SINCRONIZZARE I PROCESSI,
		//	INIZIALIZZO A ZERO IL NUM1 E AL NUMERO DI PROCESSI NUM2. QUANDO NUM1 È UGUALE A NUM2 ALLORA
		//	TUTTI I PROCESSI SONO PRONTI
		context->shm_operazione->num1 = 0;
		context->shm_operazione->num2 = context->NPROC;

	sem_signal(MUTEX, context->sync_id.semid_sys);

	for( i = 0 ; i < context->NPROC ; i++ )
		{
			pid[i]=fork();
			if(pid[i] == -1)
				print_err("fork");

			if(pid[i] == 0)
			{
				sprintf(id, "%d", i);
				sprintf(semid_start, "%d", context->sync_id.semid_start);
				sprintf(semid_finish, "%d", context->sync_id.semid_finish);
				sprintf(semid_sys, "%d", context->sync_id.semid_sys);
				sprintf(shm_operazione, "%d", context->sync_id.shmid_operazione);
				sprintf(shm_info, "%d", context->sync_id.shmid_status);

				sem_wait(MUTEX, context->sync_id.semid_sys);
					context->shm_operazione[i].kill = 'L';
				sem_signal(MUTEX, context->sync_id.semid_sys);

				char *arg[] = {
						"./client.x",
						id,
						semid_start,
						semid_finish,
						semid_sys,
						shm_operazione,
						shm_info,
						NULL
				};
				execvp(arg[0], arg);
				print_err("execvp");

			}
		}
	logsm("Aspetto i miei figli\n");

	//	SEMAFORO PER SINCRONIZZARE IL MAIN CON I FIGLI
	sem_wait(SEMREADY, context->sync_id.semid_sys);
	logsm("Sincronizzati\n");

	//	SETTO IL SEMAFORO DEI PROCESSI LIBERI AL NUMERO DEI PROCESSI MASSIMI
	sem_set( context->sync_id.semid_sys, SEMPROCFREE, context->NPROC);
}
/**
 * Inizializza il context per lo slave, in shm_operazione è salvato il puntatore
 * alla struttura condivisa con il main.
 */
void slave_context_init(slave_context *slave, char *argv[])
{
	slave->id = atoi(argv[1]);
	slave->sync_id.semid_start = atoi(argv[2]);
	slave->sync_id.semid_finish = atoi(argv[3]);
	slave->sync_id.semid_sys = atoi(argv[4]);

	//-----------------------------------------------
	//	ATTACCO ALLA MEMORIA CONDIVISA
	//-----------------------------------------------

	slave->shm_operazione = (operazione *) shm_attack(atoi(argv[5]));
	slave->shm_status = (status_and_res_struct *) shm_attack(atoi(argv[6]));

	// INIZIALIZZO LO STATO A -1 ( MAI USATO )
	sem_wait(MUTEXRES , slave->sync_id.semid_sys);
		slave->shm_status[slave->id].status = -1;
	sem_signal(MUTEXRES , slave->sync_id.semid_sys);

	//-----------------------------------------------
	//	SEZIONE PER SINCRONIZZARSI CON IL MAIN
	//-----------------------------------------------

	sem_wait(MUTEX, slave->sync_id.semid_sys);
	slave->shm_operazione->num1++;
	if(slave->shm_operazione->num1 == slave->shm_operazione->num2)
		sem_signal(SEMREADY, slave->sync_id.semid_sys);

	sem_signal(MUTEX, slave->sync_id.semid_sys);

}
/*
 * Scrive la struttura operazione nella memoria condivisa puntata da
 * coda->id
 */
void write_operazione(program_context *context)
{
	static char str[64];
	sem_wait(MUTEX, context->sync_id.semid_sys);

	context->shm_operazione->num1 = context->coda->num1;
	context->shm_operazione->num2 = context->coda->num2;
	context->shm_operazione->pos = context->coda->pos;
	context->shm_operazione->simbolo = context->coda->op;

	sprintf(str, "Comando.........%d\n", context->lenght_coda - context->coda->pos + 1);
	logsm(str);
	sprintf(str, "Processo........%d\n", context->coda->id+1);
	logsm(str);
	sprintf(str, "Operazione......%d %c %d\n", context->coda->num1 ,context->coda->op,context->coda->num2);
	logsm(str);

	sem_signal(MUTEX, context->sync_id.semid_sys);
}
void read_operazione(slave_context *slave)
{

	sem_wait(MUTEX, slave->sync_id.semid_sys);

	slave->operazione_singola.num1 = slave->shm_operazione->num1;
	slave->operazione_singola.num2 = slave->shm_operazione->num2;
	slave->operazione_singola.pos = slave->shm_operazione->pos;
	slave->operazione_singola.kill = slave->shm_operazione->kill;
	slave->operazione_singola.simbolo = slave->shm_operazione->simbolo;

	sem_signal(MUTEX, slave->sync_id.semid_sys);
}
/*
 * il risultato viene salvato nella struttura operazione dello slave.
 */
void calcolo_and_save_operazione(slave_context *slave)
{
	slave->operazione_singola.res = calcolo(slave->operazione_singola.num1, slave->operazione_singola.num2,slave->operazione_singola.simbolo);
}
/**
 *\fn int calcolo(int num1 ,int num2, char op)
 *\param num1 Primo coefficiente dell'operazione
 *\param op Operazione
 *\param num2 Secondo coefficiente dell'operazione
 *\return Risultato dell'operazione
 *\brief esegue il calcolo e ritorna l'intero risultante
 */
double calcolo(int num1, int num2, char op)
{
	switch(op)
	{
		case '+': return num1 + num2;
		case '-': return num1 - num2;
		case '*': return num1 * num2;
		case '/': return (num1 / num2);
	}
	return 0;
}
void wait_and_stop_process(program_context *context, pid_t *pid, double *result_main)
{
	static int i;
	//-------------------------------------------------------------
	//	ASPETTO CHE TUTTI I PROCESSI ABBIANO FINITO
	//	E RECUPERO L'ULTIMO RISULTATO DI TUTTI I PROCESSI
	//-------------------------------------------------------------
	for(i = 0 ; i < context->NPROC ; i++)
	{
		sem_wait(i +1 , context->sync_id.semid_finish);

		sem_wait(MUTEXRES, context->sync_id.semid_sys);
			if(context->shm_status[i].status > 0)
				result_main[context->shm_status[i].pos - 1] = context->shm_status[i].res;

		sem_signal(MUTEXRES, context->sync_id.semid_sys);
	}
	logs("\n\t---------- FINE CALCOLI ----------\n\n");

	//-------------------------------------------------------------
	//	SCRIVO IL SIMBOLO DI ARRESTO NELLA MEMORIA CONDIVISA
	//-------------------------------------------------------------
	sem_wait(MUTEX, context->sync_id.semid_sys);
		context->shm_operazione->kill = 'K';
	sem_signal(MUTEX, context->sync_id.semid_sys);

	//-------------------------------------------------------------
	//	SEGNALO AI PROCESSI DI TERMINARE L'ESECUZIONE
	//-------------------------------------------------------------

	for(i=0; i < context->NPROC ; i++)
	{
		sem_signal(i + 1, context->sync_id.semid_start);
		wait(&pid[i]);
	}
}
void coda_handler(program_context *context, double *result_main)
{
	program_context *this = context;
	logs("\n\t--- Inizio il calcolo ---\n\n");

		while(this->coda)
		{

			if(this->coda->id)
			{
				this->coda->id--;

				//	Aspetto che il processo abbia finito
				sem_wait( this->coda->id + 1, this->sync_id.semid_finish);

				save_result(this, result_main);

				// CARICO IN MEMORIA I DATI
				write_operazione(this);

				// FACCIO PARTIRE IL PROCESSO
				sem_signal(this->coda->id + 1, this->sync_id.semid_start);

				// ASPETTO CHE I DATI SIANO STATI RICEVUTI
				sem_wait(SEMDATARECEIVE, this->sync_id.semid_sys);

				this->coda = this->coda->next;
			}
			else
			{
				search_free_process(this);
			}
		}
}
void save_result(program_context *context, double *result_main)
{
	sem_wait(MUTEXRES, context->sync_id.semid_sys);

	// SE È PRESENTE UN RISULTATO ALLORA LO SALVO NELL'ARRAY
		static int pos;
		pos = context->shm_status[context->coda->id].pos - 1;
		if(context->shm_status[context->coda->id].status > 0)
			result_main[pos] = context->shm_status[context->coda->id].res;

	sem_signal(MUTEXRES, context->sync_id.semid_sys);
}
void search_free_process(program_context *context)
{
	static int i;
	while( !context->coda->id )
	{
		for( i = 0 ; i < context->NPROC ; i++)
		{
			sem_wait(MUTEXRES, context->sync_id.semid_sys);
				if( context->shm_status[i].status )
				{
					context->coda->id = i + 1;
					sem_signal(MUTEXRES, context->sync_id.semid_sys);
					break;
				}
			sem_signal(MUTEXRES, context->sync_id.semid_sys);
		}

		if( !context->coda->id)
		{
			// aspetto che almeno un processo si liberi
			sem_wait(SEMPROCFREE, context->sync_id.semid_sys);
			sem_signal(SEMPROCFREE, context->sync_id.semid_sys);
		}
	}
}
