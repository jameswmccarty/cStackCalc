#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

/**
 * Adds an item to the top of a stack
 */
char push(stack* stk, token *t) {

	element *new; /* new element to push to stack */	
	if(stk==NULL) /* can't do anything */
	{
		return -1;
	}
	new = (element *) malloc(sizeof(struct _element));
	if(new==NULL) /* malloc fails */
	{
		printf("Error: malloc() failed.\n");
		return -1;
	}
	else
	{
		/* set elements data */
		new->item = t;
		/* set items prev pointer to top of current stack */
		new->prev = stk->top;
		/* put this item on top of the stack */
		stk->top = new;
	}
	return 0;

}

/* pop an item off the stack */

token * pop(stack* stk) {
	
	/* pointer to current top of stack */
	element *current;
	token *temp; /* data inside of this element */

	if(stk->top==NULL) /* stack is empty */
	{
		return NULL;
	}
	else
	{
		/* get current top of stack */
		current = stk->top;
		/* set new top of stack to item below current */
		stk->top=current->prev;
		/* get the data from the current element */
		temp = current->item;
		/* free the old element */
		free(current);
		return temp;
	}
}

/* return the top element without popping the stack */

token * peek(stack* stk) {
	if(stk->top == NULL) /* stack is empty */
	{
		return NULL;
	}
	return stk->top->item;
}

/* print the stack all the way to the bottom */
void print(stack* stk) {
	/* pointer to current top of stack */
	element *current;
	int i = 0;
	if(stk == NULL) /*can't print*/
	{
		return;
	}
	current = stk->top;
	while(current != NULL) 
	{
		print_helper(i++, current->item);
		current = current->prev;
	}
	printf("----------\n");
}


void print_helper(int i, token *t) {
	printf("%d :\t%f", i, t->val);
	fputc('\n', stdout);
}

/* clear all items from a stack */

void clear(stack* stk) {
	
	/* pointer to current top of stack */
	element *current;
	if(stk==NULL) /*can't free*/
	{
		return;
	}			
	while(stk->top != NULL) 
	{
		current = stk->top->prev;
		free(stk->top);
		stk->top = current;
	}

}


