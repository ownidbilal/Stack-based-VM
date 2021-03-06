/* 
 * A simple machine simulator with a simple language that contains
 * a stack and 7 instructions. Borrowed from a stack overflow 
 * answer.
 */

#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

typedef struct stack stack;
typedef struct stack_element stack_element;

struct stack_element {
    int64_t         number;
    stack_element   *next;
};

/* 
 * A stack is basically a singly linked list with a pointer pointing
 * at the top most element. 
 */
struct stack {
    stack_element   *top;
    uint64_t        element_count;
};

stack_element *create_stack_element(int64_t numb)
{
    stack_element *new_element = malloc(sizeof(stack_element));
    new_element->number = numb;
    new_element->next = NULL;
    return new_element;
}

stack *initialize_stack(void)
{
    stack *stk = malloc(sizeof(stack));

    /* make sure no garbage value is assigned by compiler */
    stk->top = NULL;
    stk->element_count = 0;
    return stk;
}

void push(stack *stk, int64_t numb)
{
    stack_element *new_element = create_stack_element(numb);

    if (stk->top != NULL) 
        new_element->next = stk->top;
    
    stk->top = new_element;
    stk->element_count++;
}

int64_t pop(stack *stk)
{
    int64_t save_numb = 0;

    if (stk->top != NULL) {
        stack_element *marked_for_deletion = stk->top; /* Avoid memory leak */
        save_numb = marked_for_deletion->number; /* Save marked's number */

        stk->top = stk->top->next;
        free(marked_for_deletion);

        stk->element_count--;
    }
    
    return save_numb;
}

/*
 * Add and pop top two elements of stack and push the sum.
 */
void add(stack *stk)
{
    if (stk->top == NULL) {
        printf("Stack is empty\n");
        return;
    }

    push(stk, pop(stk) + pop(stk));
}

/*
 * Push a copy of what's at the top of the stack back onto the stack
 */
void dup(stack *stk)
{
    if (stk->top == NULL) {
        printf("Stack is empty\n");
        return;
    }

    push(stk, stk->top->number);
}

/* 
 * Print starts from top of stack
 */
void print_stack(stack *stk)
{
    stack_element *traverser = stk->top;

    while (traverser != NULL) {
        printf("%ld\n", traverser->number);
        traverser = traverser->next;
    }
}

/*
 * Print the top most element of the stack
 */
void print_top(stack *stk)
{
    if (stk->top == NULL) {
        printf("Stack is empty\n");
        return;
    }

    printf("%ld\n", stk->top->number);
}

/*
 * Jump to a line with number `line_number`. The number is the number of
 * traversals from top. It will return the stack_element where number of
 * traversals equals the `line_number`.
 */
stack_element *jump(stack *stk, uint64_t line_number)
{
    if (stk->top == NULL) {
        printf("Stack is empty\n");
        return NULL;
    }

    if (stk->element_count < line_number) {
        printf("Line number exceeds number of elements in stack\n");
        return NULL;
    }

    if (line_number == 0) {
        printf("0 is an illegal line number\n");
        return NULL;
    }

    stack_element *traverser = stk->top;
    
    while (line_number-- != 1)
        traverser = traverser->next;

    return traverser;
}

/*
 * Examine the top of the stack. If it's 0, continue, else, jump to line
 * number.
 */
stack_element *ifeq(stack *stk, uint64_t line_number)
{
    if (stk->top == NULL) {
        printf("Stack is empty\n");
        return NULL;
    }

    if (stk->top->number != 0)
        return jump(stk, line_number);

    return NULL;
}

void free_stack(stack *stk)
{
    stack_element *traverser;
    
    while (stk->top) {
        traverser = stk->top;
        stk->top = stk->top->next;
        free(traverser);
    }
    
    free(stk);
}

#endif /* STACK_H */
