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
 *@file client.c
 *@author Giovanni Liboni
 */
#include "def.h"

int main (int argc, char *argv[]){

        slave_context context;
        char *str = (char *) malloc(sizeof(char) * 128);

        //-----------------------------------------------
        // RECUPERO LE INFORMAZIONI DEL PROCESSO
        //-----------------------------------------------
        slave_context_init(&context, argv);

        //-----------------------------------------------
        //      CICLO PER SVOLGERE IL COMANDO
        //-----------------------------------------------
        while(1){

                //-----------------------------------------------
                //      INCREMENTO I PROCESSI LIBERI
                //-----------------------------------------------
                sem_signal(SEMPROCFREE, context.sync_id.semid_sys  );

                //-----------------------------------------------
                //      SEGNALO DI AVER FINITO IL LAVORO PRECEDENTE
                //-----------------------------------------------
                sem_signal(context.id + 1,context.sync_id.semid_finish  );

                //-----------------------------------------------
                //      ASPETTO DI RICEVERE IL SEGNALE START
                //-----------------------------------------------
                sem_wait(context.id + 1, context.sync_id.semid_start  );

                // decremento il semaforo dei processi liberi
                sem_wait(SEMPROCFREE, context.sync_id.semid_sys);

                logsc(context.id, "Avvio...........OK\n");

                //-----------------------------------------------
                //      IMPOSTO LA FLAG DEL PROCESSO OCCUPATO
                //-----------------------------------------------
                sem_wait(MUTEXRES, context.sync_id.semid_sys);
                        context.shm_status[context.id].status = 0;
                sem_signal(MUTEXRES, context.sync_id.semid_sys);

                //-----------------------------------------------
                //      MEMORIZZO GLI OPERANDI PER IL CALCOLO
                //-----------------------------------------------
                read_operazione(&context);

                //-----------------------------------------------
                //      SEGNALO CHE MI SONO ARRIVATI I DATI
                //-----------------------------------------------
                sem_signal(SEMDATARECEIVE, context.sync_id.semid_sys );

                //-----------------------------------------------
                //      CONTROLLO CHE NON SIA IL SEGNALE KILL
                //-----------------------------------------------
                if(context.operazione_singola.kill == 'K'){
                        shm_detach(context.shm_operazione);
                        shm_detach(context.shm_status);
                        logsc(context.id, "Termino.........OK\n");
                        exit(0);
                }
                sprintf(str, "Dati arrivati...%d %c %d\n", context.operazione_singola.num1, context.operazione_singola.simbolo, context.operazione_singola.num2);
                logsc(context.id, str);

                //-----------------------------------------------
                //      CALCOLO IL RISULTATO
                //-----------------------------------------------
                calcolo_and_save_operazione(&context);

                //-----------------------------------------------
                //      SCRIVO I RISULTATI NELLA MEMORIA CONDIVISA
                //-----------------------------------------------
                sem_wait(MUTEXRES, context.sync_id.semid_sys  );
                        context.shm_status[context.id].status = 1;
                        context.shm_status[context.id].res = context.operazione_singola.res;
                        context.shm_status[context.id].pos = context.operazione_singola.pos;
                sem_signal(MUTEXRES, context.sync_id.semid_sys );
        }
        exit(0);
}// fine main
