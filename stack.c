/* Stack demo using a linked list */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "stack.h"
// Create a new stack
list *newStack() {
    list *new = malloc(sizeof(list));
    new->first = NULL;
    return new;
}

// Check if stack is empty
bool isEmpty(list *stack) {
    return stack->first == NULL;
}

// Add an item
void push(list *stack, state n) {
    cell *new = malloc(sizeof(cell));
    *new = (cell) { n, stack->first };
    stack->first = new;
}

// Stop the program with a message
void fail(char *message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}

// Return the first item
state top(list *stack) {
    if (stack->first == NULL) fail("top of empty stack");
    return stack->first->item;
}

// Remove and return an item
state pop(list *stack) {
    cell *first = stack->first;
    if (first == NULL) fail("pop of empty stack");
    stack->first = first->next;
    state n = first->item;
    free(first);
    return n;
}
