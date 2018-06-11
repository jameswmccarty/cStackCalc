#include<stdlib.h>
#include<stdio.h>
#include<malloc.h>
#include "stack.h"

#define SPACE ' '
#define TAB '\t'
#define MAXELEM 0x100

/* enums and structs */

token *P[MAXELEM] = { 0 };
token *T[MAXELEM] = { 0 };
	
token_t is_op(char c)
{
	switch(c) {
		case '+':
			return PLUS;
		case '-':
			return MINUS;
		case '*':
			return MULTIPLY;
		case '/':
			return DIVIDE;
		case '%':
			return MODULO;
		default:
			return 0;
	}
}

int set_precedence(token_t type) {
	switch(type) {
		case PLUS:
		case MINUS:
			return 1;
		case MULTIPLY:
		case DIVIDE:
		case MODULO:
			return 2;
		default:
			return 0;
	}
}

token* build_token(token_t type, double value)
{
	token *new;

	new = (token *) malloc(sizeof(token));
	if(new == NULL) {
		printf("malloc() failed in build_token().\n");
		exit(EXIT_FAILURE);
	}
	new->type = type;
	new->val = value;
	new->oo = set_precedence(type);

	return new;
}

int is_numeric(char c)
{
	if(c == '.' || /* period  */
		( c >= 48 && c <= 57 )) /* 0 - 9 */
		return 1;
	if(c == 'e' || c == 'E') /* exponents */
		return 2;
	if(c == '-') /* negative exponent */
		return 3;
	return 0;
}

double modulus (double a, double b)
{
  int cast;
  cast = (int) (a / b);
  return a - ((double) cast * b);
}

void initstack(stack* stack)
{
	(*stack).top = NULL;
	(*stack).result = 0.0;
}

void tokenize(char *eqn) {
	int i = 0;
	int neg_exp = 0;
	int t_idx = 0;

	while(eqn[i]) {
		if(eqn[i] == LPAREN) 
			T[t_idx++] = build_token(LPAREN, 0.);
		else if(eqn[i] == RPAREN)
			T[t_idx++] = build_token(RPAREN, 0.);
		else if(is_op(eqn[i]))
			T[t_idx++] = build_token(is_op(eqn[i]), 0.);
		else if(eqn[i] == SPACE || eqn[i] == TAB); /* ignore white space */
		else if(is_numeric(eqn[i])) {
			T[t_idx++] = build_token(OPERAND, atof(&eqn[i]));
			while(1) { /* skip the same chars that atof() will cover */
				if(!is_numeric(eqn[i]))
					break;
				if(is_numeric(eqn[i++])==2)
					neg_exp = 1;
				if(is_numeric(eqn[i])==3 && !neg_exp)
					break;
				neg_exp = 0;				 
			}
			i--;
		}
		else {
			fputc('\n', stdout);
			printf("Error - Didn't recognize symbol: ");
			fputc(eqn[i], stdout);
			fputc('\n', stdout);
			exit(EXIT_FAILURE);
		}
		i++;
		if(t_idx >= MAXELEM) {
			printf("Error: Input buffer is full.\n");
			exit(EXIT_FAILURE);
			/* TODO dynamic expansion */
		}
	}
	printf("\n");
}

void make_postfix()
{
	stack tmpstack;
	int p_idx = 0;
	int t_idx = 0;
	token *current;
	initstack(&tmpstack);
	while(T[t_idx]) /* while not at end of equation */
	{
		current = T[t_idx];

		if(current->type == OPERAND) { /* if we have an operand */
			P[p_idx++] = current;
		}
		else if(current->type == LPAREN) /* nested */
			push(&tmpstack, current);
		else if(current->type == RPAREN) /* un-nest */
		{
			/* Found a right parenthesis with empty stack */
			if(peek(&tmpstack) == NULL) {
				printf("Error: Unmatched Right parenthesis.\n");
				exit(EXIT_FAILURE);	
			}
			while(peek(&tmpstack) != NULL && peek(&tmpstack)->type != LPAREN)
				P[p_idx++] = pop(&tmpstack);
			(void) pop(&tmpstack);
		}
		else if(current->type > OP_LO && current->type < OP_HI) {
			if(peek(&tmpstack) == NULL || peek(&tmpstack)->type == LPAREN)
				push(&tmpstack, current);
			else {
				while(peek(&tmpstack) != NULL && peek(&tmpstack)->type != LPAREN
					&& current->oo <= peek(&tmpstack)->oo)
					P[p_idx++] = pop(&tmpstack);
				push(&tmpstack, current); 
			}
		}
		t_idx++;
	}

	while(peek(&tmpstack) != NULL) {
		P[p_idx++] = pop(&tmpstack);
		/* Left paren on stack by itself */
		if(P[p_idx-1]->type == LPAREN && 
			peek(&tmpstack) == NULL) {
			printf("Error: Unmatched Left parenthesis.\n");
			exit(EXIT_FAILURE);		
		}
	}
	clear(&tmpstack);
}

double postfix_eval() {
	stack tmpstack;
	int p_idx = 0;
	double temp = 0.;
	double l, r;
	token_t op;
	token *throw;
	initstack(&tmpstack);

	while(P[p_idx]) { /* while not at end of equation */
		if(P[p_idx]->type == OPERAND) { /* if we have an operand */
			push(&tmpstack, P[p_idx++]);
		}
		if(P[p_idx] != NULL && P[p_idx]->type > OP_LO && P[p_idx]->type < OP_HI) { /* got operator */
			op = P[p_idx++]->type;
			throw = pop(&tmpstack);
			if(throw == NULL || throw->type != OPERAND) {
				printf("Error: Malformed input.  Expected number, got something else.\n");
				exit(EXIT_FAILURE);			
			}
			r = throw->val; free(throw);
			throw = pop(&tmpstack);
			if(throw == NULL || throw->type != OPERAND) {
				printf("Error: Malformed input.  Expected number, got something else.\n");
				exit(EXIT_FAILURE);			
			}
			l = throw->val; free(throw);			
			switch(op) {
				case PLUS:
					temp = l + r;
				break;
				case MINUS:
					temp = l - r;
				break;
				case MULTIPLY:
					temp = l * r;
				break;
				case DIVIDE:
					if(r == 0.0)
						printf("Caught divide by Zero.\n");
					temp = l / r;
				break;
				case MODULO:
					temp = modulus(l, r);
				break;
				default: /* error*/
					printf("Got some invalid op.\n");
			}

			push(&tmpstack, build_token(OPERAND, temp));
		}
	}

	throw = pop(&tmpstack);
	if(throw != NULL)
		temp = throw->val;
	free(throw);

	while(peek(&tmpstack) != NULL)
		free(pop(&tmpstack));
	clear(&tmpstack);
	return temp;
}

int main(int argc, char **argv) {

	int t_idx = 0;

	if(argc != 2)
	{
		printf("usage: %s <equation>\n", argv[0]);
		exit(1);
	}
	tokenize(argv[1]);
	make_postfix();
	printf("Result: %f\n", (float) 	postfix_eval());
	while(T[t_idx])
		free(T[t_idx++]);
	return 0;
}

