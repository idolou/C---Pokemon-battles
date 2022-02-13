//
// Created by ise on 11/19/21.
//
#include <stdio.h>

#ifndef POKEMONS_DEFS_H
#define POKEMONS_DEFS_H

typedef enum e_bool {false, true} bool;
typedef enum e_status {success, failure} status;
typedef enum { F, T } boolean;
#define buff 300


///////second part
//typedef void* Element;
//typedef Element (*copyFunction)(Element);
//typedef status (*freeFunction)(Element);
//typedef status (*printFunction)(Element);
//typedef bool (*equalFunction)(Element);


typedef void * element;
typedef element(*copyFunction) (element);
typedef status(*freeFunction) (element);
typedef status(*printFunction) (element);
typedef int(*equalFunction) (element, element);//1 first bigger, 0 - same, -1 second bigger
typedef char*(*getCategoryFunction)(element);
typedef int(*getAttackFunction)(element firstElem ,element secondElem ,int* attackFirst,int* attackSecond);
#endif //POKEMONS_DEFS_H
