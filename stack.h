/* Stack demo using a linked list */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct point{
    long double x,y;
};
typedef struct point point;

struct state{
    point pos;
    long double angle;
};
typedef struct state state;

// The structure for one stack item
struct cell {
    state item;
    struct cell *next;
};
typedef struct cell cell;

// The structure for the while stack
struct list {
    struct cell *first;
};
typedef struct list list;

// Create a new stack
list *newStack();

// Check if stack is empty
bool isEmpty(list *stack);

// Add an item
void push(list *stack, state n);

// Return the first item
state top(list *stack);

// Remove and return an item
state pop(list *stack);
