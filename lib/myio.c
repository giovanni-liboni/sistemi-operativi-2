/**
 *@file myio.c
 *@author Giovanni Liboni - VR363021
 */
#include "myio.h"

#ifndef SIZE
#define SIZE 1024 /*dimensione per il buffer di lettura/scrittura*/
#endif

coda* readFile(char *file)
{
	// VETTORE CHE CONTIENE UNA RIGA DEL FILE
	char s[128];
	// BUFFER PER LEGGERE UN SOLO CARATTERE ALLA VOLTA
	char c;
	// INDICE PER IL VETTORE DI CARATTERI S
	int i;
	// FILE DESCRIPTOR
	int fd_from;
	// NUMERO DI CARATTERI LETTI
	int nread;
	// POSIZIONE ALL'INTERNO DEL FILE
	long where;
	// CONTATORE PER SALTARE L'ULTIMA RIGA
	int cont=0;
	//	ARRAY DI SUPPORTO PER LEGGERE IL FILE
	char tmp[100];
	// ARRAY PER I DATI DELLA RIGA
	int data[4];
	// INDICE PER L'ARRAY DEI DATI
	int pos =0;
	// CODA DOVE SALVARE I COMANDI
	coda * coda = NULL;

	// APRO IL FILE
	if ((fd_from = open(file, O_RDONLY)) == -1)
	{
		logs("open() failure\n");
		exit(1);
	}
	// SPOSTO IL PUNTATORE DEL FILE ALLA FINE
	if ((where = lseek(fd_from, -1, SEEK_END)) == -1)
	{
		logs("lseek() failure\n");
		exit(1);
	}

	//
	i = sizeof(s) - 1;
	s[i] = '\0';

	while ((nread = read(fd_from, &c, sizeof(char))) == 1)
	{

		//	SE RAGGIUNGO LA FINE DELLA RIGA
		if (c == '\n')
		{
			if ( cont > 0)
			{
				sprintf(tmp, "%s", &s[i]); /* stampo la stringa s a partire dall'elemento i */
					char *token = strtok(tmp, " ");
					while(token != NULL)
					{
						//	SE È UN NUMERO NEGATIVO ALLORA LEGGO IL MODULO DEL NUMERO E POI LO MOLTIPLICO
						//	PER -1
						if(strlen(token) > 1 && token[0] == '-' && token[1] != ' ')
						{
							data[pos]=char_to_int(&token[1]);
							data[pos]= data[pos] * (-1);
							token = strtok(NULL, " ");
							pos++;
						}
						else
						{
							data[pos]=char_to_int(token);
							token = strtok(NULL, " ");
							pos++;
						}

					}

					//	RIPORTO ALL'INIZIO L'INDICE PER I DATI
					pos=0;

					// CONTROLLO CHE LA RIGA ABBIA IL CARATTERE DELL'OPERAZIONE NELLA POSIZIONE GIUSTA
					if(is_op(int_to_char(data[2])))

						// COSTRUISCO LA CODA DEI COMANDI
						coda = construct_list(data[0], data[1], int_to_char(data[2]), data[3] ,coda);
					else
					{
						logs("Errore lettura file!\n");
						exit(1);
					}
			}
			// INCREMENTO IL CONTATORE
			cont++;

			// RIPORTO L'INDICE DEL VETTORE S ALL'ULTIMO VALORE
			i = sizeof(s) - 1;
		}
		// SE L'INDICE I DIVENTA 0 ALLORA HO FINITO LO SPAZIO SUL VETTORE S
		if (i == 0)
		{
			logs("Riga troppo lunga.\n");
			exit(1);
		}

		//	MEMORIZZO IL CARATTERE LETTO DOPO AVER DECREMENTATO L'INDICE I.
		//	I CARATTERI VENGONO MEMORIZZATI DALLA FINE PER PRESERVARE L'ORDINE DELLA RIGA
		s[--i] = c;

		// SE LSEEK RITORNA 0 SIGNIFICA CHE SONO ARRIVATO ALL'INIZIO DEL FILE E QUINDI ESCO DAL WHILE
		if (where == 0)
			break;

		//	DOPO AVER LETTO UN CARATTERE DEVO RISALIRE DI 2 POSIZIONI, IL CARATTERE CHE HO APPENA LETTO
		//	E QUELLO CHE VOLGIO LEGGERE
		if ((where = lseek(fd_from, -2L, SEEK_CUR)) == -1)
			print_err("lseek");
	}

	// CHIUDO IL FILE
	if (close(fd_from) == -1)
			print_err("close");


	return coda;
}
int readProc(char *file)
{
	// BUFFER DI LETTURA
	char buf[SIZE];
	// CARATTERE DOVE MEMORIZZO QUELLO CHE STO LEGGENDO
	char c;
	// NUMERO LETTO
	int nread;
	// FILE DESCRIPTOR
	int fd = open(file, O_RDONLY);
	// INDICE PER IL BUFFER
	int i = 0;

	if (fd == -1 )
	{
		print_err("Open");
		exit(1);
	}
	while((nread = read(fd, &c , 1)) > 0)
	{
		if(nread < 0)
		{
			close(fd);
			print_err("read");
		}
		if ( c == '\n')
		{
			buf[i] = '\0';
			lseek(fd, 0 , SEEK_END);
			close(fd);
			return atoi(buf);
		}
		else
			buf[i++] = c;
	}

	close(fd);
	return -1;
}
void writeFile(const double *data, coda *contextCoda, char *file, int max)
{
	// FILE DESCRIPTOR
	int	fd;
	// INDICE PER IL BUFFER
	int	i;
	// BUFFER PER LA SCRITTURA
	char buf[SIZE];
	// CODA TEMPORANEA
	coda *this;

	this = contextCoda;

	if ((fd = creat(file,0666)) < 0)
		print_err("creat");

	for(i = max - 1; i >= 0 ; i--)
	{
		// STAMPO SUL BUFFER I DATI
		sprintf(buf, "%4d %8d %c %8d = %8.0f\n",max +1 - this->pos, this->num1 ,this->op, this->num2, data[i] );
		//	SCRIVO SUL FILE I DATI
		if (write(fd, buf, strlen(buf)) < 0)
			print_err("write");

		this = this->next;
	}
	logsm("File creato\n");
	close(fd);
}
int is_op(char op)
{
	return op == '+' || op == '-' || op == '/' || op == '*';
}
void print_err(char *s)
{
	char *f="() failure\n";
	if (! write(1, s, strlen(s)))
		exit(1);
	if (! write(1, f, strlen(f)))
		exit(1);

	exit(1);
}
void logs(char *string)
{
	if (! write(1, string, strlen(string)))
	{
		print_err("write");
	}
}
void logi(int num)
{
	static char buf[10];
	sprintf(buf, "%d", num);
	if (! write(1,buf , strlen(buf)))
	{
		print_err("write");
	}
}
void logsm(char *string)
{
	char str[128];
	sprintf(str, "[MAIN] %s", string);
	if (! write(1, str, strlen(str)))
		exit(1);

}
void logsc(int id, char *string)
{
	static char str[128];
	sprintf(str, "[%4d] %s",id+1, string);
	if (! write(1, str, strlen(str)))
		exit(1);
}
