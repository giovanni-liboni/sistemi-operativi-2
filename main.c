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
 * @file main.c
 * @author Giovanni Liboni
 *@mainpage
 *@author Giovanni Liboni
 *@date 2012 - 2013
 *@section intro_sec Testo elaborato
 *	Si vuole realizzare un programma in C che utilizzi le system call, ove possibile, per implementare un
	sistema di calcolo aritmetico parallelo basato su POSIX thread. Il progetto deve essere commentato in
	formato Doxygen, e corredato di uno script configurazione per tale tool e di uno script Makefile per la
	sua compilazione.

	Il programma dovrà leggere un file di configurazione, contenente:
	 -# Il numero di thread di calcolo parallelo.
	 -# I dati di computazione
	 .

	Il file di configurazione avra' la seguente struttura:
		- La prima riga conterra' un intero, che corrisponde al numero di thread di calcolo parallelo da
			creare, NTHREAD.
		- Le altre righe avranno il seguente formato: <id> <num1> <op> <num2>, dove:
			-# 	id e' il numero di thread di calcolo parallelo a cui far eseguire l'operazione, se e' maggiore
				o uguale a 1. Se e' zero, indica che l'operazione dovra' essere eseguita dal primo thread
				libero. Si assuma che id corrisponda sempre ad un valore corretto.
			-# 	num1 e num2 sono due numeri interi, corrispondenti ai dato sui quali eseguire
				l'operazione.
			-# 	op e' un carattere rappresentante l'operazione da eseguire sui due numeri. Caratteri validi
				per op sono: '+', '-', '*', '/', rappresentanti rispettivamente la somma, la sottrazione, la
				moltiplicazione e la divisione.
			.
		.

	Ad esempio, un file di configurazione potrebbe essere:

	5\n
	32/9\n
	3 234 + 653\n
	4 3444 + 153\n
	0 34 * 1873\n
	3 0 - 21534\n

	Ogni thread di calcolo parallelo eseguirà una routine di questo tipo:
		-# Attesa su un mutex per eseguire un calcolo.
		-# Ricezione dei dati e dell'operazione da eseguire da parte del thread principale.
			-# 	Se riceve il comando di terminazione, termina la sua esecuzione. Il comando di
				terminazione e' indicato dal carattere 'K'.
			 .
		-# Esecuzione del calcolo.
		-# Invio del risultato al thread principale, e segnalazione al thread principale che ha finito di
			calcolare.
		-# Attesa sullo stesso mutex del punto 1, per attendere il comando successivo.
		.

	Il thread principale eseguira' in questo modo:
		-# Setup della simulazione, leggendo dal file di configurazione il numero di processori da
		simulare, creandone i thread di calcolo parallelo relativi, e creando ed inizializzando le
		eventuali strutture di supporto per la comunicazione. Inoltre, verra' creato un array in cui
		memorizzare i risultati delle operazioni eseguite. La lunghezza di tale array sara' ricavata dal
		numero di operazioni lette dal file di configurazione.
		-# Entrata in un ciclo che per ogni operazione da simulare effettua quanto segue:
			-# 	Se il comando ha id diverso da zero, attende che il thread di calcolo parallelo numero id sia
				libero, salva il risultato dell'eventuale calcolo precedente nell'array dei risultati, e poi
				interagisce con lui passandogli il comando da simulare. Il thread principale non deve
				attendere che il processore abbia completato la simulazione dell'operazione passata al
				figlio.
			-# 	Se l'istruzione ha id 0, trova il primo processore libero ed in caso interagisce con esso,
				come al punto 1. Altrimenti attende che almeno un processore sia libero, e poi interagisce
				con esso come al punto 1.
			.
		-# 	Passati tutti i comandi ai thread di calcolo parallelo, attende che i thread abbiano eseguito tutti
			i loro calcoli.
		-# 	Salva nell'array dei risultati gli ultimi risultati computati, e fa terminare i thread di calcolo
			parallelo passando loro il comando di terminazione 'K'.
		-# Attende che tutti i thread abbiano terminato.
		-# Stampa su un file di output tutti i risultati.
		-# Libera eventuali risorse.
		-# Esce.
		.

	Il thread principale e le thread figlio si scambiano dati tramite puntatori passati alla
	pthread_create(), e non tramite variabili globali.

	Si aggiungano alle thread di calcolo parallelo e a quella principale delle stampe a video per poter
	seguire l'esecuzione.

	Per ogni chiamata ad una system call, si deve controllare che tale funzione abbia successo.
	//-------------------------------------------------------------
	//	ALLOCO LA MEMORIA PER L'ARRAY DEI RISULTATI
	//-------------------------------------------------------------
	double main_result[context.lenght_coda];
	Tutte le stampe a video, le letture e le scritture su file devono avvenire tramite system call
	(quindi ad esempio non si possono utilizzare printf, fprintf, scanf, fscanf, perror).


 */


#include "util.h"
#include "my_thread.h"
#include "list.h"
#include "myio.h"
#include "def.h"
/**
 *\fn int main(int argc, char *argv[])
 *\param argc Numero argomenti passati
 *\param argv Puntatore alla stringa di argomenti passati
 *\brief Funzione main del programma che gestisce il calcolo parallelo su più threads
 */

int main(int argc, char *argv[])
{
	//------------------------------------------------------------
	// 	VARIABILI
	//------------------------------------------------------------
	int id;
	int i;
	int res;
	int pos;
	char *str = (char *) malloc(sizeof(char) * 128);
	coda *temp;
	program_context context;

	// SCRITTA INIZIALE
	logs("\n\t---------- Inizio il programma ----------\n\n");

	//------------------------------------------------------------
	// 	OPERAZIONI CON IL FILE DI CONFIGURAZIONE
	//------------------------------------------------------------

	// LEGGO IL NUMERO DI PROCESSI
	context.NTHREAD = readProc(argv[1]);

	// LEGGO IL FILE E MEMORIZZO I COMANDI NELLA CODA
	context.coda = readFile(argv[1]);

	// SALVO IL PUNTATORE DELLA CODA
	temp = context.coda;

	//-------------------------------------------------------------
	//	INIZIALIZZO IL CONTEXT
	//-------------------------------------------------------------

	context_init( &context );

	//-------------------------------------------------------------
	//	ALLOCO LA MEMORIA PER L'ARRAY DEI RISULTATI
	//-------------------------------------------------------------
	double main_result[context.lenght_coda];

	//------------------------------------------------------------
	//	SE CI SONO ERRORI NEL FILE ESCO
	//------------------------------------------------------------
	if (! context.NTHREAD && ! context.coda )
	{
		logs("\tFile letto...");logs("NO");logs("\n");
		exit(1);
	}

	//------------------------------------------------------------
	//	STAMPO LE INFO SULLA CONFIGURAZIONE
	//------------------------------------------------------------
	logs("Info: \n");
	logs("\tFile letto...");logs("OK");logs("\n");
	logs("\tProcessi.....");logi(context.NTHREAD);logs("\n");
	logs("\tComandi......");logi(context.lenght_coda);logs("\n");
	logs("\n");

	//-------------------------------------------------------------
	//	CREO I THREADS PER IL CALCOLO PARALLELO
	//-------------------------------------------------------------
	creat_thread(&context);

	//-------------------------------------------------------------
	//	GESTISCO IL CALCOLO PARALLELO
	//-------------------------------------------------------------
	
	logs("\n\t--- Inizio il calcolo ---\n\n");

	while(context.coda)
		{

			if(context.coda->id)
			{
				// SALVO IN id IL VALORE REALE DELL'ID
				id = context.coda->id - 1;

				//-------------------------------------------------------------
				//	ASPETTO CHE IL PROCESSO SIA LIBERO
				//-------------------------------------------------------------
				lock_mutex(&context.thread[id].finish);

				//-------------------------------------------------------------
				//	RECUPERO IL PRECEDENTE RISULTATO E LO MEMORIZZO NELL'ARRAY
				//	DEI RISULTATI
				//-------------------------------------------------------------
				res = context.thread[id].operazione.res;
				pos = context.thread[id].operazione.pos;
				main_result[ pos ] =  res;

				//-------------------------------------------------------------
				//	CARICO IN MEMORIA I DATI DA PASSARE ALLA THREAD
				//-------------------------------------------------------------
				context.thread[id].operazione.num1 = context.coda->num1;
				context.thread[id].operazione.simbolo = context.coda->op;
				context.thread[id].operazione.num2 = context.coda->num2;
				context.thread[id].operazione.pos = context.coda->pos - 1;
				context.thread[id].kill = 'N';
				context.thread[id].used = 1;

				// STAMPO LE INFORMAZIONI RELATIVE AL COMANDO PASSATO
				sprintf(str, "Comando.........%d\n", context.lenght_coda - context.coda->pos + 1);
				logsm(str);

				sprintf(str, "Processo........%d\n", context.coda->id);
				logsm(str);

				sprintf(str, "Operazione......%d %c %d\n",context.coda->num1 , context.coda->op ,context.coda->num2);
				logsm(str);

				//-------------------------------------------------------------
				//	SBLOCCO LA THREAD
				//-------------------------------------------------------------
				unlock_mutex(&context.thread[id].start);

				//-------------------------------------------------------------
				//	PASSO AL PROSSIMO COMANDO
				//-------------------------------------------------------------
				context.coda = context.coda->next;
			}
			else
			{
				// CONTROLLO FINO A QUANDO NON SI È LIBERATO UN PROCESSO
				while( !context.coda->id )
				{
				logsm(" --- Controllo processi liberi ---\n");

					// CONTROLLO SE SONO PRESENTI THREAD LIBERE
					for( i = 1 ; i <= context.NTHREAD ; i++)
					{
						if( context.status.array[i - 1] != 0 )
						{
							context.coda->id = i;
							break;
						}
					}
					// SE NON SONO PRESENTI THREAD LIBERE ASPETTO
					if( !context.coda->id)
					{
						//-------------------------------------------------------------
						//	ASPETTO CHE UNA THREAD SI LIBERI
						//-------------------------------------------------------------
						thread_wait_for_ready(&context.processi_liberi, context.NTHREAD);

					}
				}
			}
	}

	//-------------------------------------------------------------
	//	ASPETTO LA FINE DELL'ULTIMO CALCOLO E TERMINO LE THREAD
	//-------------------------------------------------------------
	wait_and_stop(&context, main_result);

	//-------------------------------------------------------------
	//	STAMPO SU FILE I RISULTATI
	//-------------------------------------------------------------
	context.coda = temp;

	writeFile(main_result, temp , "out.txt", context.lenght_coda );

	//-------------------------------------------------------------
	//	DEALLOCO LE RISORSE UTILIZZATE
	//-------------------------------------------------------------
	dealloc_all(&context);

	logs("\n\t---------- Fine programma ----------\n\n");
	exit(0);
}
