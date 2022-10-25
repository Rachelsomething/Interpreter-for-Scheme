#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "value.h"
#include "talloc.h"
#include <assert.h>
// Create a new NULL_TYPE value node.
Value *makeNull() {
    Value *value = talloc(sizeof(Value));
    value -> type = NULL_TYPE;
    return value;
}

// Create a new CONS_TYPE value node.
Value *cons(Value *newCar, Value *newCdr){
    Value *cons = talloc(sizeof(Value));
    cons -> type =  CONS_TYPE;
    //put in the value
    (cons->c).car= newCar;
    (cons->c).cdr= newCdr;

    return cons;
}

// Display the contents of the linked list to the screen in some kind of
// readable format
void display(Value *list){
    assert(list != NULL);
    Value *helper = list;
    while (helper -> type != NULL_TYPE){
        switch ((helper -> c).car-> type) {
            case INT_TYPE:
                printf("%i", (helper -> c).car -> i);
                break;
            case DOUBLE_TYPE:
                printf("%f", (helper -> c).car -> d);
                break;
            case STR_TYPE:
                printf("%s", (helper -> c).car -> s);
                break;
            default:
                break;
        }
        helper = (helper -> c).cdr;
    }
}

// Return a new list that is the reverse of the one that is passed in. All
// content within the list should be duplicated; there should be no shared
// memory whatsoever between the original list and the new one.
//
// FAQ: What if there are nested lists inside that list?
// ANS: There won't be for this assignment. There will be later, but that will
// be after we've got an easier way of managing memory.
Value *reverse(Value *list){
    assert(list != NULL);
    Value *newList = makeNull();
    while (list -> type != NULL_TYPE) {
        newList = cons((list -> c).car, newList);
        list = (list -> c).cdr;
    }
    return newList;
}

// Utility to make it less typing to get car value. Use assertions to make sure
// that this is a legitimate operation.
Value *car(Value *list){
    assert(list != NULL);
    return (list -> c).car;   
}

// Utility to make it less typing to get cdr value. Use assertions to make sure
// that this is a legitimate operation.
Value *cdr(Value *list){
    assert(list != NULL);
    return (list -> c).cdr;
}

// Utility to check if pointing to a NULL_TYPE value. Use assertions to make sure
// that this is a legitimate operation.
bool isNull(Value *value) {
    if (value->type == NULL_TYPE) {
        return true;
    }
    else {
        return false;
    }
}

// Measure length of list. Use assertions to make sure that this is a legitimate
// operation.
int length(Value *value) {
    int length = 0;
    while(value -> type!= NULL_TYPE){
        length++;
        value = (value->c).cdr;
    }
    return length;
};


//Capstone work

Value *append(Value *list1, Value *list2) {
    assert(list1 != NULL);
    Value *head = list1;
    while (list1->type != NULL_TYPE) {
        list1 = cdr(list1);
    }
    Value *helper = list1;
    list1 = list2;
    free(helper); //freeing NULL_TYPE at the end of list1
    

    return head;
    
}

Value *list(Value *item1, Value *item2) {
    assert(item1 != NULL);
    assert(item2 != NULL);
    Value *newList = cons(item1, cons(item2, makeNull()));
    return newList;

}