#pragma once
#include <stdint.h>

// Note: All functions that take a list or a node
// require those pointers to be non-NULL and for 
// node to be members of the list passed with them
// otherwise the behavior of the function is undefined. 

/// Pointer to a LinkedList structure 
typedef struct LinkedList *LinkedListPtr;

/// Change this typedef to the type you want the list to contain.
/// This is the only line you should need to edit. If you update
/// this library from upstream, re-apply your change here.
typedef int ValueType;

/// LinkedList node 
typedef struct Node Node;

/// Optional value, valid indicates if value is populated,
/// if valid is 0 value is undefined and should not be used. 
typedef struct {
    int valid;
    ValueType value;
} OptionalValue;

/// Returns the value the node holds
ValueType llNodeGetValue(Node *node);

/// Updates the value contained in the node
void llNodeSetValue(Node *node, ValueType val);

/// Creates a new linked list and returns a pointer to it,
/// returns a null pointer if creation fails
LinkedListPtr llCreateLinkedList(void);

/// Frees a linked list must be called to properly free 
/// memory of the list
/// NOTE: This function invalidates the given 
/// pointer and pointers to all nodes in the list
void llFreeLinkedList(LinkedListPtr ptr);

/// Adds a new node to the list that contains the given value. 
/// returns the position of the node or NULL if the operation failed.
Node* llAppend(LinkedListPtr list, ValueType val);

/// Removes the given node from the list.
/// returns the value the node contained. 
/// NOTE: this function invalidates the 
/// given node pointer.
ValueType llRemove(LinkedListPtr list, Node *node);

/// Searchs the list for the first node that contains the given value,
/// returns the node. Takes a compare function pointer
/// that is used to compare values, this function must return 0 if the 
/// given values are equal and a different number if they are not. 
Node* llContains(LinkedListPtr list, ValueType val, int (*compare)(ValueType, ValueType));

/// Iterate over the list and call the given function for each value;
void llIterate(LinkedListPtr list, void (*fp)(ValueType));

/// Returns the number of values in the list. 
uint32_t llCount(LinkedListPtr list); 


