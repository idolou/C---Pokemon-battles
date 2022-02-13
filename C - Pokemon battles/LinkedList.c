//
// Created by ise on 12/11/21.
//
#include "LinkedList.h"
#include <string.h>



///struct for each node in the list, will hold the data of the element and a pointer to the next node///
struct t_Node{
    element node_ele;
    struct t_Node *next;
};


///struct for the generic LinkedList itself, will get all the necessary functions///
///will hold the size of the list(length) and 2 pointers for head and tail///

struct t_LinkedList{
    copyFunction copyFunc;
    freeFunction freeFunc;
    printFunction printFunc;
    equalFunction equalFunc;
    int length;
    Node *head;
    Node *tail;
};


LinkedList createLinkedList(copyFunction Cfunc, freeFunction Ffunc,
                            printFunction Pfunc, equalFunction eFunc){
    if(!Cfunc || !Ffunc|| !Pfunc|| !eFunc)
        return NULL;
    LinkedList List;
    List = (LinkedList) malloc(sizeof(struct t_LinkedList));
            if(!List){
                return NULL;}
    List->copyFunc = Cfunc;
    List->freeFunc = Ffunc;
    List->printFunc = Pfunc;
    List->equalFunc = eFunc;
    List->head = NULL;
    List->tail = NULL;
    List->length = 0;

    return List;
}

status destroyList(LinkedList List){
    if(!List)
        return failure;
    Node *curr;
    curr = List->head;
    while (curr) {
        Node *next_node = curr->next;
            List->freeFunc(curr->node_ele);
                free(curr);
        curr = next_node;
    }
    free(List);
    return success;
}

///Util func to create new node. appendNode func will use this func
Node* initNode(element ele){
    if(!ele)
        return  NULL;
    Node* node;
    node = (Node*) malloc(sizeof(Node));
    if( !node ){
        return NULL;}
    node->node_ele = ele;
    node->next = NULL;
    return node;
}

status appendNode(LinkedList List, element node){
    if(!List || !node)
        return failure;
    Node* newNode;
    newNode = initNode(node);
    if(!newNode){
        destroyList(List);
        return failure;}
    //if it is the first element added to the list
    if (List->head == NULL){
        List->head = newNode;
        List->tail = newNode;
        newNode->next = NULL;
        List->length++;
    }
    //append the node to the end of the list and update head and tail
    else
    {
        List->tail->next = newNode;
        List->tail = newNode;
        newNode->next = NULL;
        List->length++;
    }
    return success;
}


status deleteNode(LinkedList List, element node){
    Node * Dnode = (Node*) node;
    if(!List || !Dnode)
        return failure;
    Node * prev = NULL;
    Node * current = List->head;

    //in the case list is empty
    if(current == NULL)
        return failure;

    //in case we want to delete head of list
    if(List->equalFunc(current->node_ele, Dnode->node_ele) == 0){
        List->head = current->next;
        List->freeFunc(current->node_ele); //will call free function to free the memory
        free(current);
        current = NULL;
        List->length--;
        return success;
    }
    while(current!=NULL){
        if(List->equalFunc(current->node_ele, Dnode->node_ele) == 0){
            prev->next = current->next;
            List->freeFunc(current->node_ele);
            free(current);
            List->length--;
            return success;
        }
        prev = current;
        current = current->next;
    }
    return failure;
}

void displayList(LinkedList list){
    if (!list)
        return;
    Node * current;
    current = list->head;
    while (current != NULL){
        list->printFunc(current->node_ele);
        current = current->next;
    }
}

element searchByKeyInList(LinkedList list, element ele){
    if(!list || !ele)
        return NULL;
    Node * current;
    current = list->head;
    while(current != NULL){
        if(list->equalFunc((current->node_ele), ele)==0)
            return (element)current->node_ele;
        current = current->next;
    }
    return NULL;
}