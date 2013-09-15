/**
 *@file main.c
 *@author Giovanni Liboni - VR363021
 *@mainpage
 *@author Giovanni Liboni - VR363021
 *@date 2012 - 2013
 *@section intro_sec Testo elaborato
 * Si vuole realizzare un programma in C che utilizzi le system call, ove possibile, per implementare lo
 * scheletro di un sistema di calcolo aritmetico parallelo. Il progetto deve essere commentato in formato
 * Doxygen, e corredato di uno script configurazione per tale tool e di uno script Makefile per la sua
 * compilazione.\n\n
 * Il programma dovrà leggere un file di configurazione, contenente:\n\n
 *	-# Il numero di processi di calcolo parallelo.\n
 *	-# I dati di computazione.\n\n
 *	.
 *	Il file di configurazione avra' la seguente struttura:\n
 *      - La prima riga conterra' un intero, che corrisponde al numero di processi paralleli da creare, NPROC.
 *
 *		- Le altre righe avranno il seguente formato: <id> <num1> <op> <num2>, dove:
 *      				-# id e' il numero di processo parallelo a cui far eseguire l'operazione se e' maggiore o uguale a 1
 *      					- Se e' zero, indica che l'operazione dovra' essere eseguita dal primo processo libero
 *      					- si assuma che id corrisponda sempre ad un valore corretto
 *      					.
 * 				-# num1 e num2 sono due numeri interi , corrispondenti ai dato su i quali eseguire l'operazione.
 * 				-# op e' un carattere rappresentante l'operazione da eseguire sui due numeri. Caratteri validi per op sono: '+', '-', '*', '/', rappresentanti rispettivamente la somma, la sottrazione, la	moltiplicazione e la divisione.
 *						.
 *		.
 *
 * Ad esempio, un file di configurazione potrebbe essere:
 *	5\n
 *	3 2/9\n
 *	3 234 + 653\n
 *	4 3444 + 153\n
 *	0 34 * 1873\n
 *	3 0 - 21534\n
 *
 *	Ogni processo eseguirà una routine di questo tipo:
 *	-# Attesa su un semaforo per eseguire un calcolo.
 *	-# Ricezione dei dati e dell'operazione da eseguire, da parte del processo padre.
 *		- Se riceve il comando di terminazione, termina la sua esecuzione. Il comando di terminazione e' indicato dal carattere 'K'.
 *		.
 *	-# Esecuzione del calcolo.
 *	-# Invio del risultato al padre, e segnalazione al padre che ha finito di calcolare.
 *	-# Attesa bloccante sullo stesso semaforo al punto 1, per attendere il comando successivo.
 *	.
 *
 *	Il processo padre comunichera' con i processi figlio tramite memoria condivisa.\n\n
 *	Il processo padre eseguira' in questo modo:
 *		-# Setup della simulazione, leggendo dal file di configurazione il numero di processori da simulare, creandone i processi relativi, e creando ed inizializzando le eventuali strutture di
			supporto quali semafori e memoria condivisa. Inoltre, verra' creato un array in cui
			memorizzare i risultati delle operazioni eseguite. La lunghezza di tale array sara' ricavata dal
			numero di operazioni lette dal file di configurazione.
		-# Entrata in un ciclo che per ogni operazione da simulare effettua quanto segue:
				-# Se il comando ha id diverso da zero, attende che il processo numero id sia libero, salva il
				risultato dell'eventuale calcolo precedente nell'array dei risultati, e poi interagisce con lui
				passandogli il comando da simulare. Il processo padre non deve attendere che il processore
				abbia completato la simulazione dell'operazione passata al figlio.
				-# Se l'istruzione ha id 0, trova il primo processore libero ed in caso interagisce con esso,
				come al punto 1. Altrimenti attende che almeno un processore sia libero, e poi interagisce
				con esso come al punto 1.
				.
		-# Passati tutti i comandi ai figli, attende che i processi figlio abbiano eseguito tutti i loro calcoli.
		-# Salva nell'array dei risultati gli ultimi risultati computati, e fa terminare i processi figlio
			passando il comando di terminazione 'K'.
		-# Attende che tutti i figli abbiano terminato.
		-# Stampa su un file di output tutti i risultati.
		-# Libera eventuali risorse.
		-# Esce.
		.

	Si aggiungano ai processi figlio e al processo padre delle stampe a video per poter seguire
	l'esecuzione.

	Per ogni chiamata ad una system call, si deve controllare che tale funzione abbia successo.

	Tutte le stampe a video, le letture e le scritture su file devono avvenire tramite system call
	(quindi ad esempio non si possono utilizzare printf, fprintf, scanf, fscanf, perror).
 */
#include "util.h"
#include "list.h"
#include "myio.h"
#include "def.h"

/**
 *@fn int main(int argc, char *argv[])
 *@param argc Numero di argomenti
 *@param argv Puntatore all'array degli argomenti
 *@brief Funzione main che riceve come parametro il file di configurazione e gestisce
 * il calcolo parallelo su più processi
 */
int main(int argc, char *argv[])
{
	//------------------------------------------------------------
	// 	VARIABILI
	//------------------------------------------------------------
	coda *temp;
	program_context context;

	logs("\n\t---------- Inizio il programma ----------\n\n");
	logs("Info: \n");

	//------------------------------------------------------------
	// 	OPERAZIONI CON IL FILE DI CONFIGURAZIONE
	//------------------------------------------------------------
	context.NPROC = readProc(argv[1]);

	if (context.NPROC < 1)
	{
		logs("Numero processi inferiore a 1\n");
		exit(1);
	}

	if (!(temp = (coda *)readFile(argv[1])))
	{
		logs("\tFile letto...");logs("NO");logs("\n");
		exit(1);
	}
	//------------------------------------------------------------
	// 	INIZIALIZZO IL CONTESTO DEL MAIN
	//------------------------------------------------------------
	context_init(&context, temp);

	//------------------------------------------------------------
	//	STAMPO LE INFO SULLA CONFIGURAZIONE
	//------------------------------------------------------------
	logs("\tFile letto...");logs("OK");logs("\n");
	logs("\tProcessi.....");logi(context.NPROC);logs("\n");
	logs("\tComandi......");logi(context.lenght_coda);logs("\n\n");

	//------------------------------------------------------------
	//	Creo un array in cui memorizzo i pid dei figli
	//------------------------------------------------------------
	pid_t pid[context.NPROC];

	//-------------------------------------------------------------
	//	CREO L'ARRAY DEI RISULTATI
	//-------------------------------------------------------------
	double result_main[context.lenght_coda];

	//-------------------------------------------------------------
	//	CREO I PROCESSI PARALLELI
	//-------------------------------------------------------------
	creat_process(pid, &context);

	//-------------------------------------------------------------
	//	GESTISCO LA CODA
	//-------------------------------------------------------------
	coda_handler(&context, result_main);

	//-------------------------------------------------------------
	//	ASPETTO CHE I PROCESSI ABBIANO FINITO DI CALCOLARE
	//	E LI TERMINO
	//-------------------------------------------------------------
	wait_and_stop_process(&context, pid, result_main);

	//-------------------------------------------------------------
	//	STAMPO SU FILE I RISULTATI
	//-------------------------------------------------------------
	writeFile(result_main, temp , "out.txt", context.lenght_coda );

	//-------------------------------------------------------------
	//	DEALLOCO LE RISORSE UTILIZZATE
	//-------------------------------------------------------------
	logsm("Dealloco tutte le risorse.\n");
	dealloc_data(&context);
	destruct_coda(context.coda);

	logs("\n\t---------- Fine programma ----------\n\n");

	exit(0);
}// fine main
