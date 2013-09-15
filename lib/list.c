/**
 *@file list.c
 *@author Giovanni Liboni - VR363021
 */
#include "list.h"
/*
 * funzioni per la gestione della coda
 */
coda *construct_list(int id, int num1, int op, int num2, coda *next)
{
  coda *this = malloc(sizeof(coda));
  this->id = id;
  this->next = next;
  this->op = op;
  this->num1 = num1;
  this->num2 = num2;
  return this;
}
void destruct_coda(coda *this)
{
  if (this)
  {
	free(this);
    destruct_coda(this->next);
  }
}
int length_list(coda *this)
{
  int pos;
  if (!this)
	    return 0;
  pos = 1 + length_list(this->next);
  this->pos = pos;
  return pos;
}
