/**************************
** Datatype definitions  **
**************************/

/* The element struct. Has a pointer to a char */

typedef enum {
	OP_LO=1,
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE,
	MODULO,
	OP_HI,
	LPAREN = '(',
	RPAREN = ')',
	OPERAND
} token_t;

typedef struct {
	token_t type;
	double val;
	int oo;
} token;

typedef struct _element {
	struct _element *prev; /* location of item below this one in the stack */
	token *item;
} element;

/* The stack struct. Has a top pointer. */
typedef struct _stack {
	double result;
	element *top;
} stack;

/* Pushing */
char push(stack* stk, token *t);

/* Popping */
token * pop(stack* stk);

/* Free */
void clear(stack* stk);

/* View but don't pop */
token * peek(stack* stk);

/* Pretty print */
void print_helper(int i, token *t);
void print(stack* stk);

