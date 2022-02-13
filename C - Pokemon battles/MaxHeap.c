//
// Created by ise on 12/11/21.
//
#include "MaxHeap.h"
#include <string.h>
#include <stdio.h>


///declared in MaxHeap.h///

struct t_MaxHeap {
    element *max_heap_arr;  //this array will hold all the elements
    copyFunction copyFunc;
    freeFunction freeFunc;
    printFunction printFunc;
    equalFunction equalFunc;
    int max_ele;    //the capacity of the heap
    int size;       //the current number of elements stored in the heap
    char *id;
};


///Util function for the insert function and the Maxheapify function///
///will give the indexes of parent, left and right child of each element ( in array)///
int parent(int i) { return ((i-1) / 2); }

int leftChild(int i) { return (2*i); }

int rightChild(int i) { return ((2 * i)+1); }


///Max heapify function to keep the heap updated and legal///
void Max_Heapify(MaxHeap heap, int i) {
    int largest;
    int left = leftChild(i);
    int right = rightChild(i);
    if (left <= heap->size && heap->equalFunc(heap->max_heap_arr[left], heap->max_heap_arr[i]) == 1) {
        largest = left;
    } else { largest = i; }
    if (right <= heap->size && heap->equalFunc(heap->max_heap_arr[right], heap->max_heap_arr[largest]) == 1) {
        largest = right;
    }
    if (largest != i) {
        element temp_ele;
        temp_ele = heap->max_heap_arr[i];
        heap->max_heap_arr[i] = heap->max_heap_arr[largest];
        heap->max_heap_arr[largest] = temp_ele;
        Max_Heapify(heap, largest);
    }
}


MaxHeap
CreateHeap(copyFunction Copy, freeFunction Free, printFunction Print, equalFunction Equal, int capacity, char *id) {
    if (!Copy || !Free || !Print || !Equal || !capacity || !id) {
        return NULL;
    }
    MaxHeap heap;
    heap = (MaxHeap) malloc(sizeof(struct t_MaxHeap));
    if (!heap) {
        return NULL;
    }
    heap->id = (char *) malloc(sizeof(char) * strlen(id) + 1);
    if (!heap->id) {
        destroyHeap(heap);
        return NULL;
    }
    strcpy(heap->id, id);

    heap->max_heap_arr = (element *) malloc(sizeof(element) * capacity);
    if (!heap->max_heap_arr) {
        destroyHeap(heap);
        return NULL;
    }

    heap->copyFunc = Copy;
    heap->freeFunc = Free;
    heap->printFunc = Print;
    heap->equalFunc = Equal;
    heap->size = 0;
    heap->max_ele = capacity;

    return heap;
}

status destroyHeap(element heap) {
    MaxHeap Heap;
    Heap = (MaxHeap) heap;
    if (!heap) { return failure; }
    for (int i = 0; i < Heap->size; i++) {
        if (Heap->max_heap_arr[i] != NULL) {
            Heap->freeFunc(Heap->max_heap_arr[i]);
        }
    }
    free(Heap->max_heap_arr);
    free(Heap->id);
    free(heap);
    return success;
}

///Help function for the insertion function
void HeapIncrease(MaxHeap heap, int heap_size, element new_ele) {
    if (!heap || !new_ele) return;
    heap->size++;
    heap->max_heap_arr[heap_size] = new_ele;

    int Parent = parent(heap_size);
    while (heap_size > 0 && heap->equalFunc(heap->max_heap_arr[Parent], heap->max_heap_arr[heap_size]) == -1) {
        element temp_ele;
        temp_ele = heap->max_heap_arr[Parent];
        heap->max_heap_arr[Parent] = heap->max_heap_arr[heap_size];
        heap->max_heap_arr[heap_size] = temp_ele;
        heap_size = parent(heap_size);
    }
}

status insertHeap(element new_ele, MaxHeap heap) {
    if (!heap || !new_ele || heap->size >= heap->max_ele) { return failure; }
    if (heap->size < heap->max_ele) {
        HeapIncrease(heap, heap->size, new_ele);
        return success;
    } else { return failure; }
}

status printHeap(element heap) {
    MaxHeap pHeap;
    pHeap = (MaxHeap) heap;
    if (!pHeap) { return failure; }
    int i = 1;

    MaxHeap copy = copyHeap(pHeap);
    printf("%s:\n", pHeap->id);
    if (copy->size > 0) {
        while (copy->size > 0) {
            printf("%d. ", i);
            copy->printFunc(PopMaxHeap(copy));
            i++;
        }
    } else {
        printf("No elements.\n\n");
    }
    destroyHeap(copy);
    return success;
}

element copyHeap(MaxHeap heap) {
    if (!heap) { return NULL; }
    MaxHeap copy = CreateHeap(heap->copyFunc, heap->freeFunc, heap->printFunc,
                              heap->equalFunc, heap->max_ele, heap->id);
    if (!copy) { return NULL; }
    for (int i = 0; i < heap->size; i++) {
        element cpy_ele = heap->max_heap_arr[i];
        insertHeap(cpy_ele, copy);
    }
    return copy;
}

element PopMaxHeap(MaxHeap heap) {
    if (heap == NULL || heap->size == 0) { return NULL; }
    element max = heap->max_heap_arr[0];
    if (!max) { return NULL; }
    heap->max_heap_arr[0] = heap->max_heap_arr[heap->size - 1];
    heap->max_heap_arr[heap->size - 1] = NULL;  ///check
    heap->size--;
    Max_Heapify(heap, 0);
    return max;
}

element TopMaxHeap(MaxHeap heap) {
    if (!heap || heap->size == 0) { return NULL; }
    return heap->max_heap_arr[0];
}

char *getHeapId(element heap) {
    if (!heap) { return NULL; }
    MaxHeap Heap = (MaxHeap) heap;
    if (!Heap) { return NULL; }
    return (char *) Heap->id;
}

int getHeapCurrentSize(MaxHeap heap) {
    if (!heap) { return 0; }
    return heap->size;
}
