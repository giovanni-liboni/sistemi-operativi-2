/**
 *@file shm.h
 *@author Giovanni Liboni - VR363021
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
