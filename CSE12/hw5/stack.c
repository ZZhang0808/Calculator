/****************************************************************************

                                                Zifan Zhang
                                                CSE 12, Fall 19
                                                10/29/19
                                                cs12fa19gt
                              Assignment 5

File Name:      stack.c
Description:    Contains the Stack structure and the member functions of the
		Stack class.
****************************************************************************/
#include <stdio.h>
#include "list.h"
#include "stack.h"

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
	/* call delete_List to delete the stack */
        delete_List (spp);
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
long isempty_Stack (Stack * this_stack) {
	/* call isempty_List to check if the list is empty */
        return isempty_List (this_stack);
}

/*---------------------------------------------------------------------------
Function Name:                new_Stack
Purpose:                      Allocate memory for a new stack
Description:                  This constructor function allocates and 
			      initializes a new Stack object. It allocates 
			      memory to hold stacksize number of longs, 
			      initializes the stack infrastructure, and returns 
			      a pointer to the first storage space in the stack.
Input:                        copu_function: a function that copies element
Output:                       The address of the stack
Result:                       The stack is created and its addresss returned
Side Effects:                 None.
---------------------------------------------------------------------------*/
Stack * new_Stack (void *(*copy_func) (void *),
        void (*delete_func) (void *),
        FILE *(*write_func) (void *, FILE *)) {
	/* call new_List to create a new list and use it as the base of 
	 * the stack */
        return new_List (copy_func, delete_func, write_func);
}

/*---------------------------------------------------------------------------
Function Name:                pop
Purpose:                      Take out the top item in the stack
Description:                  Removes an item from the top of the stack, and 
			      sends it back through the output parameter item. 
			      Returns the item's data.
Input:                        this_Stack: The pointer points to the stack
			      you want to pop from.
Output:                       The popped item's data
Result:                       Successfully pop the top item in this_Stack,
			      and its data is returned
Side Effects:                 None.
---------------------------------------------------------------------------*/
void * pop (Stack * this_stack) {
	/* call remove_List to remove the end Node of the list and return
	 * its data to simulate pop operation of a stack */
        return remove_List (this_stack, END);
}

/*---------------------------------------------------------------------------
Function Name:                push
Purpose:                      Push item to the top of the stack
Description:                  Adds item to the top of this_Stack.
Input:                        this_Stack: The pointer points to the stack
			      you want to push to.
			      element: The item you want to push.
Output:                       True or false
Result:                       Successfully push and return true, or 
			      unseccessfully push and return false.
Side Effects:                 None.
---------------------------------------------------------------------------*/
long push (Stack * this_stack, void * element) {
	/* call insert to insert an element at the end of the list to 
	 * simulate push operation of a stack */
        return insert (this_stack, element, END);
}

/*---------------------------------------------------------------------------
Function Name:                top
Purpose:                      Find the value of the top item of the stack
Description:                  Sends back the item on the top of the stack 
			      through the output parameter item, but does not 
			      remove it from the stack. Returned its data 
Input:                        this_Stack: a pointer to the stack from which to 
			      top.
Output:                       The data in the top item of the stack
Result:                       Successfully topped and the data of the top
			      item of the stack returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/
void * top (Stack * this_stack) {
	/* call view to view the top item of the list to simulate top 
	 * operation of a stack */
        return view (this_stack, END);
}

/*---------------------------------------------------------------------------
Function Name:                write_Stack
Purpose:                      Writes each item stored in this_stack
Description:                  Prints each item stored on this_stack to stream
Input:                        this_Stack: a pointer to the stack from which to 
			      top.
			      stream: where to print the output to
Output:                       stream: where the output was printed to
Result:                       Items on this_stack printed, output stream 
			      returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/
FILE * write_Stack (Stack * this_stack, FILE * stream) {
	/* call write_List to write each item on the list */
        return write_List (this_stack, stream);
}
