//
// Created by ise on 12/11/21.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BattleByCategory.h"
#include "Defs.h"
#include "LinkedList.h"
#include "MaxHeap.h"

struct battle_s {
    copyFunction copyFunc;
    freeFunction freeFunc;
    printFunction printFunc;
    equalFunction equalFunc;
    getCategoryFunction getcategoryFunc;
    getAttackFunction getAttackFunc;
    LinkedList Categories;
    char **cat_list;
    int capacity;
    int numOfCategories;

};

/// Help function declaration - check for equal id's///
int equalCategory(element heap, element category);


Battle createBattleByCategory(int capacity, int numberOfCategories, char *categories, equalFunction equalElement,
                              copyFunction copyElement, freeFunction freeElement, getCategoryFunction getCategory,
                              getAttackFunction getAttack, printFunction printElement) {
    if (!capacity || !numberOfCategories || !categories || !equalElement || !copyElement || !freeElement ||
        !getCategory || !getAttack || !printElement) { return NULL; }
    //init new battle
    Battle battles = (Battle) malloc(sizeof(struct battle_s));
    if (!battles) {
        return NULL; }

    battles->Categories = NULL;
    battles->Categories = createLinkedList((element) copyHeap, destroyHeap, printHeap, equalCategory);
    if (!battles->Categories) {
        destroyBattleByCategory(battles);
        return NULL;
    }
    battles->cat_list = (char **) malloc(sizeof(char *) * numberOfCategories);
    if (!battles->cat_list) {
        destroyBattleByCategory(battles);
        return NULL;
    }

    char line[buff];
    strcpy(line, categories);
    char *word = strtok(line, ",");
    int i = 0;
    while (word != NULL) {
        battles->cat_list[i] = (char *) malloc((strlen(word)) + 1);
        if (battles->cat_list[i] == NULL) {
            destroyBattleByCategory(battles);
            return NULL;
        }
        strcpy(battles->cat_list[i], word);


        MaxHeap new_cat = CreateHeap(copyElement, freeElement, printElement,
                                     equalElement, capacity, word);
        if (!new_cat) {
            destroyBattleByCategory(battles);
            return NULL;
        }
        if (appendNode(battles->Categories, new_cat) == failure) {
            destroyBattleByCategory(battles);
            return NULL;
        }

        word = strtok(NULL, ",");  //get the next word(type) after comma(',')
        i++;
    }
    battles->capacity = capacity;
    battles->copyFunc = copyElement;
    battles->freeFunc = freeElement;
    battles->equalFunc = equalElement;
    battles->printFunc = printElement;
    battles->getcategoryFunc = getCategory;
    battles->getAttackFunc = getAttack;
    battles->numOfCategories = numberOfCategories;

    return battles;
}

status destroyBattleByCategory(Battle b) {
    if (!b) { return failure; }
    for (int i = 0; i < b->numOfCategories; i++) {
        if (b->cat_list[i] != NULL)
            free(b->cat_list[i]);
    }

    free(b->cat_list);
    if (b->Categories != NULL)
        destroyList(b->Categories);
    free(b);
    return success;
}

///the function will search for Object type in the system, if found it will insert it///
status insertObject(Battle b, element elem) {
    if (!b || !elem) { return failure; }
    char *elem_id = b->getcategoryFunc(elem);
    element heap = searchByKeyInList(b->Categories, elem_id);
    if (heap == NULL) {
        return failure;
    } else if (getHeapCurrentSize(heap) < b->capacity) {
        if (insertHeap(elem, heap) == success)
            return success;
        else { return failure; }
    }
    return failure;
}

///function to print all object by category///
void displayObjectsByCategories(Battle b) {
    if (!b) { return; }
    displayList(b->Categories);
}

///the function will remove the maximum element fromm specific category
element removeMaxByCategory(Battle b, char *category) {
    if (!b || !category) { return NULL; }
    element cat_heap = searchByKeyInList(b->Categories, category);
    if (!cat_heap || getHeapCurrentSize(cat_heap) < 1) { return NULL; }
    return PopMaxHeap(cat_heap);
}

int getNumberOfObjectsInCategory(Battle b, char *category) {
    if (!b || !category) { return 0; }
    element cat_heap = searchByKeyInList(b->Categories, category);
    return getHeapCurrentSize(cat_heap);
}

///this function will get the system data structure - battle and an element from user
///the function will choose the object with the highest chance to beat the given element
///and will print the results to the screen
element fight(Battle b, element elem) {
    int attackA = 0;  //for getAttackFunc (called by address)
    int attackB = 0;  //for getAttackFunc

    int reducedAttack_A = 0; //for printf (the size of Attack per fight)
    int reducedAttack_B = 0; //for print  (the size of Attack per fight)

    int curr_fight_Result = 1000;
    int temp_max;

    element typeB = NULL;   // temp element to hold current heap(category)
    element pokemonB_result; // the element who is the most powerful against element given

    int category_length = b->numOfCategories;
    element ListofCategories = b->Categories;

    for (int i = 0; i < category_length; i++) {
        if (getNumberOfObjectsInCategory(b, b->cat_list[i]) > 0 ) {

            element categoryToFind = b->cat_list[i];
            element current_Heap = searchByKeyInList(ListofCategories, categoryToFind);
            typeB = TopMaxHeap(current_Heap);
            temp_max = b->getAttackFunc(elem, typeB, &attackA, &attackB);
            if (temp_max < curr_fight_Result) {
                pokemonB_result = typeB;   //the pokemon I Have the best chance of winning
                curr_fight_Result = temp_max;  //update the value
                reducedAttack_A = attackA;
                reducedAttack_B = attackB;
            }
        }

    }

    if (typeB == NULL || pokemonB_result == NULL) {
        printf("There are no Pokemons to fight against you.\n");
        return NULL;
    }
    printf("The final battle between:\n");
    b->printFunc(elem);
    printf("In this battle his attack is :%d\n\n", reducedAttack_A);
    printf("against ");
    b->printFunc(pokemonB_result);
    printf("In this battle his attack is :%d\n\n", reducedAttack_B);


    if (curr_fight_Result == 0) {
        printf("IT IS A DRAW.\n");
        return pokemonB_result;
    } else { printf("THE WINNER IS:\n"); }
    if (curr_fight_Result > 0) {
        b->printFunc(elem);
        return elem;
    } else {
        b->printFunc(pokemonB_result);
        return pokemonB_result;
    }

}


int equalCategory(element heap, element category) {
    MaxHeap Heap = (MaxHeap) heap;
    char *cat2 = (char *) category;
    char *cat1 = (char *) getHeapId(Heap);
    return strcmp(cat1, cat2);
}