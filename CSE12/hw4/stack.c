/****************************************************************************

                                                Zifan Zhang
                                                CSE 12, Fall 19
                                                10/15/19
                                                cs12fa19gt
                              Assignment 3

File Name:      stack.c
Description:    Contains the Stack structure and the member functions of the
		Stack class.
****************************************************************************/
#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "stack.h"

#define STACK_POINTER_INDEX (-1)        /* Index of last used space */
#define STACK_SIZE_INDEX (-2)           /* Index of size of the stack */
#define STACK_COUNT_INDEX (-3)          /* Index of which stack allocated */
#define STACK_OFFSET 3  /* offset from allocation to where user info begins */

/* catastrophic error messages */
static const char DELETE_NONEXIST[] = "Deleting a non-existent stack!!!\n";
static const char EMPTY_NONEXIST[] = "Emptying a non-existent stack!!!\n";
static const char INCOMING_NONEXIST[] = 
                        "Incoming parameter does not exist!!!\n";
static const char ISEMPTY_NONEXIST[] = 
                        "Isempty check from a non-existent stack!!!\n";
static const char ISFULL_NONEXIST[] = 
                        "Isfull check from a non-existent stack!!!\n";
static const char NUM_NONEXIST[] = 
                        "get_occupancy check from a non-existent stack!!!\n";
static const char POP_NONEXIST[] = "Popping from a non-existent stack!!!\n";
static const char POP_EMPTY[] = "Popping from an empty stack!!!\n"; 
static const char PUSH_NONEXIST[] = "Pushing to a non-existent stack!!!\n";
static const char PUSH_FULL[] = "Pushing to a full stack!!!\n";
static const char TOP_NONEXIST[] = "Topping from a non-existent stack!!!\n";
static const char TOP_EMPTY[] = "Topping from an empty stack!!!\n";
static const char WRITE_NONEXIST_FILE[] = 
                        "Attempt to write using non-existent file pointer!!!\n";
static const char WRITE_NONEXIST_STACK[] = 
                        "Attempt to write to a non-existent stack!!!\n";

/* Debug messages. HEX messages used for negative numbers on the stack. */
static const char ALLOCATED[] = "[Stack %ld has been allocated]\n";
static const char DEALLOCATE[] = "[Stack %ld has been deallocated]\n";
static const char HEXPOP[] = "[Stack %ld - Popping 0x%lx]\n";
static const char HEXPUSH[] = "[Stack %ld - Pushing 0x%lx]\n";
static const char HEXTOP[] = "[Stack %ld - Topping 0x%lx]\n";
static const char POP[] = "[Stack %ld - Popping %ld]\n";
static const char PUSH[] = "[Stack %ld - Pushing %ld]\n";
static const char TOP[] = "[Stack %ld - Topping %ld]\n";

/* static variable allocation */
static int debug = FALSE; /* allocation of debug flag */
static int stack_counter = 0; /* number of stacks allocated so far */

/* Debug state methods */
void debug_off (void) {
        debug = FALSE;
}

void debug_on (void) {
        debug = TRUE;
}
 
/* start of true stack code */
/*---------------------------------------------------------------------------
Function Name:                delete_Stack
Purpose:                      Delete a stack
Description:                  This destructor function deallocates all memory 
			      associated with the stack and sets its pointer in 
			      the calling function to NULL
Input:                        spp: The pointer points to a pointer that points
			      to the memory address of the stack.
Output:                       None.
Result:                       Stack deleted.  Nothing returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/
void delete_Stack (Stack ** spp) {
    /* YOUR CODE GOES HERE */
	/* checks if the stack exists */

	if (*spp == 0) {
                fprintf (stderr, DELETE_NONEXIST);
        }
	else
	{
		free(*spp - STACK_OFFSET);
		stack_counter--;
		if(debug)
			fprintf(stderr, DEALLOCATE, (*spp)[STACK_COUNT_INDEX]);
		*spp = 0;
	}
}
/*---------------------------------------------------------------------------
Function Name:                empty_Stack 
Purpose:                      Empty a stack
Description:                  Effectively empties the stack of all its elements. 
Input:                        this_Stack: The pointer points to the stack
			      that needs to be emptied.
Output:                       None.
Result:                       Stack emptied.  Nothing returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/
void empty_Stack (Stack * this_Stack) {
    /* YOUR CODE GOES HERE */
	/* checks if the stack exists */

	if (this_Stack == NULL) {
                fprintf (stderr, EMPTY_NONEXIST);
        }
	else
		this_Stack[STACK_POINTER_INDEX] = STACK_POINTER_INDEX;
}
/*---------------------------------------------------------------------------
Function Name:                isempty_Stack
Purpose:                      Find if the stack is empty
Description:                  Returns true if this_Stack is empty, and false 
		 	      if it is not.
Input:                        this_Stack: The pointer points to the stack
			      want to know if it is empty.
Output:                       The stack is empty or not empty.
Result:                       The emptiness of the stack is returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/
long isempty_Stack (Stack * this_Stack) {
    /* YOUR CODE GOES HERE */
	/* checks if the stack exists */
	if (this_Stack == NULL) {
                fprintf (stderr, ISEMPTY_NONEXIST);
                return 0;
        }
	return this_Stack[STACK_POINTER_INDEX] == STACK_POINTER_INDEX;
}
/*---------------------------------------------------------------------------
Function Name:                isfull_Stack
Purpose:                      Find if the stack is full
Description:                  Returns true if this_Stack is full, and false if 
			      it is not.
Input:                        this_Stack: The pointer points to the stack
			      want to know if it is empty.
Output:                       Fullness of the stack
Result:                       Fullness of the stack is returned
Side Effects:                 None.
---------------------------------------------------------------------------*/
long isfull_Stack (Stack * this_Stack) {
    /* YOUR CODE GOES HERE */
	/* checks if the stack exists */

	if (this_Stack == NULL) {
                fprintf (stderr, ISFULL_NONEXIST);
                return 0;
        }
	return this_Stack[STACK_POINTER_INDEX]==this_Stack[STACK_SIZE_INDEX]-1;
}
/*---------------------------------------------------------------------------
Function Name:                new_Stack
Purpose:                      Allocate memory for a new stack
Description:                  This constructor function allocates and 
			      initializes a new Stack object. It allocates 
			      memory to hold stacksize number of longs, 
			      initializes the stack infrastructure, and returns 
			      a pointer to the first storage space in the stack.
Input:                        stacksize: the size of the stack
Output:                       The address of the stack
Result:                       The stack is created and its addresss returned
Side Effects:                 None.
---------------------------------------------------------------------------*/
Stack * new_Stack (unsigned long stacksize) {
    /* YOUR CODE GOES HERE */
	void * memory = malloc((stacksize + STACK_OFFSET) * sizeof(long));
	/*delcare calling object */
	Stack * this_Stack = (Stack*)memory + STACK_OFFSET;
	/* initialize */
	this_Stack[STACK_COUNT_INDEX] = ++stack_counter;
	this_Stack[STACK_POINTER_INDEX] = STACK_POINTER_INDEX;
	this_Stack[STACK_SIZE_INDEX] = stacksize;
	if(debug)
		fprintf(stderr, ALLOCATED, this_Stack[STACK_COUNT_INDEX]);
	return this_Stack;
}
/*---------------------------------------------------------------------------
Function Name:                get_occupancy
Purpose:                      This function gets the number of elements in 
			      the stack.
Description:                  Returns the number of elements in this_Stack.
Input:                        this_Stack: The pointer points to the stack
			      you want to know the number of its elements.
Output:                       Number of elements in this_Stack
Result:                       Number of elements in this_Stack returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/
long get_occupancy (Stack * this_Stack) {
    /* YOUR CODE GOES HERE */
	/* checks if the stack exists */

	if (this_Stack == NULL) {
                fprintf (stderr, NUM_NONEXIST);
                return 0;
        }
	return this_Stack[STACK_POINTER_INDEX] + 1;
}
/*---------------------------------------------------------------------------
Function Name:                pop
Purpose:                      Take out the top item in the stack
Description:                  Removes an item from the top of the stack, and 
			      sends it back through the output parameter item. 
			      Returns true upon success and false upon failure.
Input:                        this_Stack: The pointer points to the stack
			      you want to pop from.
			      item: the address you want to store the top
			      element of this_Stack
Output:                       True of false
Result:                       Successfully pop the top item in this_Stack,
			      and true is returned, or unsuccessfully pop
			      and false is returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/
long pop (Stack * this_Stack, long * item) {
    /* YOUR CODE GOES HERE */
	/* checks if the stack exists */

	if (this_Stack == NULL) {
                fprintf (stderr, POP_NONEXIST);
                return 0;
        }
	/* checks if the stack is empty */

	if(!isempty_Stack(this_Stack))
	{
		*item = this_Stack[this_Stack[STACK_POINTER_INDEX]--];
		if(debug)
		{
			if(*item >= 0)
				fprintf(stderr, POP, this_Stack[STACK_COUNT_INDEX], *item);
			else
				fprintf(stderr, HEXPOP, this_Stack[STACK_COUNT_INDEX], *item);
		}
		return 1;
	}
	writeline(POP_EMPTY, stderr);
	return 0;
}
/*---------------------------------------------------------------------------
Function Name:                push
Purpose:                      Push item to the top of the stack
Description:                  Adds item to the top of this_Stack.
Input:                        this_Stack: The pointer points to the stack
			      you want to push to.
			      item: The item you want to push.
Output:                       True or false
Result:                       Successfully push and return true, or 
			      unseccessfully push and return false.
Side Effects:                 None.
---------------------------------------------------------------------------*/
long push (Stack * this_Stack, long item) {
    /* YOUR CODE GOES HERE */
	/* checks if the stack exists */

	if (this_Stack == NULL) {
                fprintf (stderr, PUSH_NONEXIST);
                return 0;
        }
	/* checks if the stack is full */

	if(!isfull_Stack(this_Stack))
	{
		this_Stack[++this_Stack[STACK_POINTER_INDEX]] = item;
		if (item >= 0 && debug)
			fprintf(stderr, PUSH, this_Stack[STACK_COUNT_INDEX], item);
		else if (debug)
			fprintf(stderr, HEXPUSH, this_Stack[STACK_COUNT_INDEX], item);
		return 1;
	}
	else
	{
		writeline(PUSH_FULL, stderr);
		return 0;
	}
	
}
/*---------------------------------------------------------------------------
Function Name:                top
Purpose:                      Find the value of the top item of the stack
Description:                  Sends back the item on the top of the stack 
			      through the output parameter item, but does not 
			      remove it from the stack. Returns true upon 
			      success and false upon failure.
Input:                        this_Stack: a pointer to the stack from which to 
			      top.
			      item: a pointer to a long which will store the 
			      topped item.
Output:                       True or false
Result:                       Successfully topped and returned true, or
			      unsuccessfully topped and returned false.
Side Effects:                 None.
---------------------------------------------------------------------------*/
long top (Stack * this_Stack, long * item) {
    /* YOUR CODE GOES HERE */
	/* checks if the stack exists */
	if (this_Stack == NULL) {
                fprintf (stderr, TOP_NONEXIST);
                return 0;
        }
	/* checks if the stack is empty */

	if(!isempty_Stack(this_Stack))
	{
		*item = this_Stack[this_Stack[STACK_POINTER_INDEX]];
		if (*item >= 0 && debug)
			fprintf(stderr, TOP, this_Stack[STACK_COUNT_INDEX], *item);
		else if(debug)
			fprintf(stderr, HEXTOP, this_Stack[STACK_COUNT_INDEX], *item);
		return 1;
	}
	writeline(TOP_EMPTY, stderr);
	return 0;
}

FILE * write_Stack (Stack * this_Stack, FILE * stream) {

        long index = 0;         /* index into the stack */

        if (this_Stack == NULL) {
                fprintf (stderr, WRITE_NONEXIST_STACK);
                return stream;
        }

        if (stream == NULL) {
                fprintf (stderr, WRITE_NONEXIST_FILE);
                return stream;
        }
                
        if (stream == stderr)
                fprintf (stream, "Stack has %ld items in it.\n",
                        get_occupancy (this_Stack));

        for (index = STACK_COUNT_INDEX + STACK_OFFSET;
                index < get_occupancy (this_Stack); index++) {

                if (stream == stderr)
                        fprintf (stream, "Value on stack is |0x%lx|\n",
                                this_Stack[index]);
                else {
                        if (this_Stack[index] < 0)
                                fprintf (stream, "%c ",
                                        (char) this_Stack[index]);
                        else
                                fprintf (stream, "%ld ", this_Stack[index]);
                }
        }

        return stream;
}
