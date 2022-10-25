#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "value.h"

/*
////
Linkedlist code using malloc
////
*/

// Utility to make it less typing to get car value. Use assertions to make sure
// that this is a legitimate operation.
Value *carN(Value *list){
    return (list -> c).car;   
}

// Utility to make it less typing to get cdr value. Use assertions to make sure
// that this is a legitimate operation.
Value *cdrN(Value *list){
    return (list -> c).cdr;
}

bool isNullN(Value *value) {
    if (value->type == NULL_TYPE) {
        return true;
    }
    else {
        return false;
    }
}

Value *makeNullN() {
    Value *value = malloc(sizeof(Value));
    value -> type = NULL_TYPE;
    return value;
}

// Create a new CONS_TYPE value node.
Value *consN(Value *newCar, Value *newCdr){
    Value *cons = malloc(sizeof(Value));
    cons -> type =  CONS_TYPE;
    //put in the value
    (cons->c).car= newCar;
    (cons->c).cdr= newCdr;

    return cons;
}
/*
////
Linkedlist code using malloc
////
*/

Value *head;

// Replacement for malloc that stores the pointers allocated. It should store
// the pointers in some kind of list; a linked list would do fine, but insert
// here whatever code you'll need to do so; don't call functions in the
// pre-existing linkedlist.h. Otherwise you'll end up with circular
// dependencies, since you're going to modify the linked list to use talloc.
void *talloc(size_t size){
    if (head == NULL){
        head = makeNullN();
    }
    //create the CAR element.
    void *pointer = malloc(size);
    Value* pointerValue = malloc(sizeof (Value));
    pointerValue -> type = PTR_TYPE;
    pointerValue -> p = pointer;
    //create the CONS_TYPE
    head = consN(pointerValue, head);

    return pointer;
}

// Free all pointers allocated by talloc, as well as whatever memory you
// allocated in lists to hold those pointers.
void tfree() {
    while (head -> type == CONS_TYPE) {
        Value* helper = cdrN(head);
        free(carN(head)->p);
        free(carN(head));
        free(head);
        head = helper;
    }
    free(head);
    head = NULL;
}

// Replacement for the C function "exit", that consists of two lines: it calls
// tfree before calling exit. It's useful to have later on; if an error happens,
// you can exit your program, and all memory is automatically cleaned up.
void texit(int status){
    tfree();
    exit(status);
}



