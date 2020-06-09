/****************************************************************************

                                                Zifan Zhang
                                                CSE 12, Fall 19
                                                10/22/19
                                                cs12fa19gt
                              Assignment 4

File Name:      calc.c
Description:    Program that will simulate a top-of-the-line calculator that 
		will perform such advanced commands as addition, subtractions,
	       	multiplication, division, exponentiation, and factorial! The 
		program will accept mathematical expressions in "infix" notation
	       	convert it to its "postfix" equivalent and then evaluate this 
		expression.
****************************************************************************/
#include <ctype.h>
#include <stdio.h>
#include "calc.h"
#include "mylib.h"
#include "stack.h"

#define CHARACTER(word) ((char) ((word) & 0x000000FF))
#define INDEX(word) (((word) & 0x0000FF00) >> 8)
#define PRIORITY(word) ((word) & 0x0000FE00)
#define SIGN_BIT (1L << ((sizeof (long) << 3) - 1))
#define BYTE 8

static char operators[] = "()+-*/^ !";


static long add (long, long);
static long divide (long, long);
static long exponent (long, long);
static long fact (long, long);
static long mult (long, long);
static long setupword (int);
static long sub (long, long);

static long (*functions[]) (long, long) =
    { NULL, NULL, add, sub, mult, divide, exponent, NULL, fact };

/*---------------------------------------------------------------------------
Function Name:                eval
Purpose:                      Evaluate an expression
Description:                  Utilizing 2 Stacks, evaluate mathematical 
			      expressions from "postfix" notation. Refer to the
			      calc.c.empty for the algorthm.
Input:                        stack1: pointer to stacks containing "postfix"
			      expressions to evaluate.
Output:                       Successfully evaluate the expression
Result:                       Expression evaluated
Side Effects:                 None.
---------------------------------------------------------------------------*/
long eval (Stack * stack1) {
     /* YOUR CODE GOES HERE */
	Stack * stack2;/* stores the reverse of stack1 */
	long item; /* stores items on the stacks */
	long num1; /* a number to be processed */
	long num2;/* another number to be processed */
	stack2 	= new_Stack(CALCSTACKSIZE); /* initialize stack2 */
	/* reverse stack1 onto stack2 */
	while(!isempty_Stack(stack1))
	{
		pop(stack1, &item);
		push(stack2, item);
	}
	/* keep evaluating until stack2 is empty */
	while(!isempty_Stack(stack2))
	{
		pop(stack2, &item);
		/* if the item popped from stack2 is number push it on stack1 */
		if((item >= 0))
			push(stack1, item);
		/* if the item popped from stack2 is operator, call the
		 * corresponding function to operate numbers on stack1 */
		else
		{
			pop(stack1, &num1);
			/* if the operator is not '!' pop another number from
			 * stack1 */
			if(CHARACTER(item) != '!')
				pop(stack1, &num2);
			push(stack1, functions[INDEX(item)](num1, num2));
		}
	}
	pop(stack1, &item); /* store the final result on item */
	delete_Stack(&stack2); /* delete stack2 */
	return item;
}

/*---------------------------------------------------------------------------
Function Name:                intopost
Purpose:                      Convert "infix" mathematical expressions into 
			      their "postfix" equivalents.
Description:                  Utilizing 2 Stacks, convert "infix" mathematical 
			      expressions entered by the user into their 
			      "postfix" equivalents. 
Input:                        stack1: pointer to empty stack to store "postfix"
			      expressions.
Output:                       Successfully convert expression to postfix
Result:                       Expression converted
Side Effects:                 None.
---------------------------------------------------------------------------*/

long intopost (Stack * stack1) {
     /* YOUR CODE GOES HERE */
	Stack * stack2; /* declare a stack for operating the algorithm */
	long character; /* stores characters obtained from stdin */
	long topvalue; /* stores the top value of stack2 */
	stack2 	= new_Stack(CALCSTACKSIZE); /* initializing stack2 */
	character = getchar(); 
	/* keep getting char from stdin until user enters new line */
	while (character != '\n')
	{
		/* return EOF if user input is EOF */
		if (character == EOF)
		{
			delete_Stack(&stack2);
			return EOF;
		}
		/* ignore spaces user entered */
		if (character == ' ')
		{
			character = getchar();
			continue;
		}
		/* if user enters an operator, call setupword to process it */
		if (!isdigit(character))
			character = setupword(character);
		/* if user enters a digit push it to stack1 */
		if (isdigit(character))
		{
			ungetc(character, stdin);
			push (stack1, decin());
		}
		/* if user enters '(' push it to stack2 */
		else if (character == setupword('('))
			push (stack2, character);
		/* if user enters ')' keep popping items from stack2 and 
		 * push to stack1, until '(' is popped from stack2 */
		else if (character == setupword(')'))
		{
			while (character != setupword('('))
			{
				pop (stack2, &character);
				push (stack1, character);
			}
			pop (stack1, &character);
		}
		/* if user enters other operators */
		else
		{
			/*  keep popping items from stack2, until stack2 is 
			 *  empty or stack2's top symbol has a 
			 * lower priority than the character entered. Then push
			 * the character onto stack2. */
			if(!isempty_Stack(stack2))
			{
				top(stack2, &topvalue);
				while(PRIORITY(character) <= PRIORITY(topvalue))
				{
					push (stack1, topvalue);
					pop (stack2, &topvalue);
					if(isempty_Stack(stack2))
						break;
					top (stack2, &topvalue);
				}
			}
			push (stack2, character);
		}
		character = getchar();
	}
	/* pop anything remained on stack2 and push to stack1 */
	while(!isempty_Stack(stack2))
	{
		pop (stack2, &character);
		push (stack1, character);
	}
	delete_Stack(&stack2); /* delete stack2 */
	return 1;
}

/*---------------------------------------------------------------------------
Function Name:                add
Purpose:                      Add two numbers
Description:                  Takes in two numbers, return the sum of the two
			      numbers.
Input:                        augend: the number to which an addend is added.
			      augend: a number which is added to another.
Output:                       The sum of the two numbers
Result:                       The sum of the two numbers returned
Side Effects:                 None.
---------------------------------------------------------------------------*/

static long add (long augend, long addend) {
    return augend + addend;
}

/*---------------------------------------------------------------------------
Function Name:                divide
Purpose:                      Divede an number from another number
Description:                  Takes in two numbers, return the quotient of the 
			      two numbers.
Input:                        divisor: a number by which another number is to be 
			      divided.
			      dividend: a number to be divided by another number
Output:                       The quotient of the two numbers
Result:                       The quotient of the two numbers returned
Side Effects:                 None.
---------------------------------------------------------------------------*/

static long divide (long divisor, long dividend) {
    return dividend / divisor;
}

/*---------------------------------------------------------------------------
Function Name:                exponent
Purpose:                      Raising base to the power exponent
Description:                  Takes in two numbers, return base to the power
			      exponent.
Input:                        power: a number says how many times to multiply
			      base to itself
			      base: the number being multiplied
Output:                       The power of base
Result:                       The power of base returned
Side Effects:                 None.
---------------------------------------------------------------------------*/

static long exponent (long power, long base) {
     /* YOUR CODE GOES HERE */
	int index; /* index number used to calculate the power */
	long result = 1; /* stores the result of the power */
	/* calculate the power */
	for(index = 0; index < power; index++)
	{
		result *= base;
	}
	return result;

}

/*---------------------------------------------------------------------------
Function Name:                fact
Purpose:                      Fine facotrial of a number
Description:                  Takes in a number and calculate its factorial.
Input:                        xxx: the number being multiplied
			      ignored: ignore this parameter
Output:                       The factorial of xxx
Result:                       The factorial of xxx returned
Side Effects:                 None.
---------------------------------------------------------------------------*/

static long fact (long xxx, long ignored) {
     /* YOUR CODE GOES HERE */
	long result = 1; /* to store the result of factorial */
	/* calculate the facotrial of xxx */
	while (xxx > 1)
	{
		result *= xxx;
		xxx--;
	}
	return result;
}

/*---------------------------------------------------------------------------
Function Name:                mult
Purpose:                      Multiply two numbers
Description:                  Takes in two numbers, return the product of the 
			      two numbers.
Input:                        factorx: the number to be multiplied
			      factory: a number which is multiplied to another.
Output:                       The product of the two numbers
Result:                       The product of the two numbers returned
Side Effects:                 None.
---------------------------------------------------------------------------*/

static long mult (long factorx, long factory) {
    return factorx * factory;
}

/*---------------------------------------------------------------------------
Function Name:                setupword
Purpose:                      Construct a long number to represent an operator
Description:                  Constructor funtion for longs representing 
			      operators to be stored on your Stacks. The
			      representation for the operators should have 
			      everything associated with that operator: a 
			      distinction from numbers, the index in the 
			      functions array corresponding to that operator, 
			      and the priority of the operator and the ASCII 
			      code.
Input:                        character: the operator to be processed
Output:                       The long representative of the operator
Result:                       The long representative of the operator returned
Side Effects:                 None.
---------------------------------------------------------------------------*/

static long setupword (int character) {
     /* YOUR CODE GOES HERE */
	long index = 0; /* look for operator */
	/* find index of parameter operator */
	while (operators[index])
	{
		if (character == operators[index])
			break;
		index++;
	}
	return SIGN_BIT | (index << BYTE) | character;
}

/*---------------------------------------------------------------------------
Function Name:                sub
Purpose:                      subtract one number from another
Description:                  Takes in two numbers, return the difference of the 
			      two numbers.
Input:                        subtrahend: the number to which an minuend is 
			      subtracted
			      augend: a number which is subtracted from another.
Output:                       The sum of the two numbers
Result:                       The sum of the two numbers returned
Side Effects:                 None.
---------------------------------------------------------------------------*/

static long sub (long subtrahend, long minuend) {
    return minuend - subtrahend;
}

