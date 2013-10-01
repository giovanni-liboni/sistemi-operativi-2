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
 *@file list.c
 *@author Giovanni Liboni 
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
