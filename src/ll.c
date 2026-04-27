#include <stdlib.h>
#include "ll.h"

// A node in the list
struct Node {
    ValueType value;
    struct Node *next;
    struct Node *prev;
};

/// A linked list structure for storing data
typedef struct LinkedList {
    uint32_t count;
    Node *first;
    Node *last;
} LinkedList;

// creates list
LinkedListPtr llCreateLinkedList(void) {
    LinkedListPtr ptr = malloc(sizeof(LinkedList));
    if (ptr == NULL) { return NULL; }
    
    ptr->count = 0;
    ptr->first = NULL;
    ptr->last = NULL;

    return ptr;
}

/// Free a given linked list
void llFreeLinkedList(LinkedListPtr ptr) {
    Node *currentNode = ptr->first;
    while(currentNode != NULL) {
        Node *nextNode = currentNode->next;
        free(currentNode);
        currentNode = nextNode;
    }
    free(ptr);
}

/// Add a new value to the list 
Node* llAppend(LinkedListPtr list, ValueType val) {
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL) { return NULL; }
    
    // Assign new node properties
    newNode->next = NULL;
    newNode->value = val;
    
    if (list->last == NULL) {
        list->first = newNode;
        list->last = newNode;
        newNode->prev = NULL;
    } else {
        list->last->next = newNode;
        newNode->prev = list->last;
        list->last = newNode;
    }
    list->count++;
    
    return newNode;
}

/// remove a node from the list
ValueType llRemove(LinkedListPtr list, Node *node) {
    // Capture a copy of the nodes value to return;
    ValueType retVal = node->value;
  
    // Update list first/last and nodes next/prev 
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->first = node->next;
    }

    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->last = node->prev;
    }

    // reduce the list counter and free the node
    list->count--;
    free(node);

    return retVal;
}

/// Check if list contains value and return the node if it does
Node* llContains(LinkedListPtr list, ValueType val, int (*compare)(ValueType, ValueType)) {
    Node *current = list->first;

    while (current != NULL) {
        if (compare(current->value, val) == 0) {
            return current;
        } 
        current = current->next;
    }

    return NULL;
}

/// Iterator 
void llIterate(LinkedListPtr list, void (*fp)(ValueType)) {
    Node *currentNode = list->first;

    while(currentNode != NULL) {
        fp(currentNode->value);
        currentNode = currentNode->next;
    }
}

/// Returns the list count 
uint32_t llCount(LinkedListPtr list) {
    return list->count;
}


/// Returns the value the node holds
ValueType llNodeGetValue(Node *node) {
    return node->value;
}

/// Updates the value contained in the node
void llNodeSetValue(Node *node, ValueType val) {
    node->value = val;
}
