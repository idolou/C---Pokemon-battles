//
// Created by ise on 12/11/21.
#ifndef POKEMONS_LINKEDLIST_H
#define POKEMONS_LINKEDLIST_H

#include "Defs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct t_LinkedList *LinkedList;
typedef struct t_Node Node;

///function to create the Linked list, each node will eventually represent type of pokemon///
LinkedList createLinkedList(copyFunction, freeFunction, printFunction, equalFunction);

///function to free all elements(nodes) in the list and the list itself///
status destroyList(LinkedList);

///func to add new node to head of the list///
status appendNode(LinkedList, element);

///func to delete node from the list. the func will use freefunc to free the memory///
status deleteNode(LinkedList, element);

///func to print all elements in list using printfunc which the user will provide///
void displayList(LinkedList);

///func to search an element in the list by specific category///
element searchByKeyInList(LinkedList, element);

#endif //POKEMONS_LINKEDLIST_H
