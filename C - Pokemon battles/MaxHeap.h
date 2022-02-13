//
// Created by ise on 12/11/21.
//
#ifndef POKEMONS_MAXHEAP_H
#define POKEMONS_MAXHEAP_H

#include "Defs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct t_MaxHeap *MaxHeap;


////the function will create new Max Heap, in the size of capacity with unique name////
MaxHeap CreateHeap(copyFunction, freeFunction, printFunction, equalFunction, int capacity, char* id);




///func to free all elements in heap and the heap itself///
status destroyHeap(element);

///function to insert new element to the heap///
status insertHeap(element, MaxHeap);

///function to print all elements in the heap from big to small by certain category///
status printHeap(element);

///the function will give the user who using this adt the ability to get the max element from heap and remove it///
element PopMaxHeap(MaxHeap);

///the function will give the user who using this adt the ability to get the max element from heap///
element TopMaxHeap(MaxHeap);

///getter for the name of the heap///
char* getHeapId(element);

///getter for the number of elements currently in the heap///
int getHeapCurrentSize(MaxHeap);

///Help function to deepCopy an heap. will help in the printHeap func///
element copyHeap(MaxHeap);

#endif //POKEMONS_MAXHEAP_H
