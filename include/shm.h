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
 *@file shm.h
 *@author Giovanni Liboni
 */
#ifndef SHM_H_
#define SHM_H_

#include "def.h"
#include "util.h"

/**
 *\fn int shm_creat(int size, int tok)
 *\param size Grandezza memoria condivisa
 *\param tok Numero causale per la creazione della memoria condivisa
 *\return L'id della memoria condivisa
 *\brief Crea una memoria condivisa e ritorna l'id della memoria condivisa appena creata
 */
int shm_creat(int size, int tok);
/**
 *\fn int * shm_attack(int shmid)
 *\param shmid Id della memoria condivisa
 *\return Ritorna l'indirizzo dell'area di memoria appena creata
 *\brief Attacca il processo ad un'area di memoria
 */
int * shm_attack(int shmid);
/**
 *@fn void shm_detach(const void *shmaddr)
 *@param shmaddr Indirizzo della memoria da cui distaccarsi
 *@brief Distacca il processo chiamante dalla memoria condivisa
 */
void shm_detach(const void *shmaddr);
#endif
