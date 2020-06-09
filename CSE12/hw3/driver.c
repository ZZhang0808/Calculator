/****************************************************************************

                                                Zifan Zhang
                                                CSE 12, Fall 19
                                                10/15/19
                                                cs12fa19gt
                              Assignment 3

File Name:      driver.c
Description:    This program tests stack.c
****************************************************************************/
/****************************************************************************
1. Set a breakpoint in new_Stack. What is the address returned by malloc()?
	0x604010

2. What is the address returned by new_Stack?
	0x604028

3. Is this address on the stack, heap, text, or data?
	This address is on the heap.

4. Now set a breakpoint in delete_Stack. What is the the value of spp? Is
this value on the stack, heap, text, or data?
	The value of spp is 0x7fffffffd9c8,this value is on the stack.

5. What is the value of *spp? Is this an address on the stack, heap, text
or data?
	The value of *spp is 0x604028, this is an address on the heap

6. What is the value of the parameter being passed to free()?
	The value of the parameter being passed to free() is 0x604010.

7. Is this parameter to free the same address that was returned from malloc
(Your answer to question 1)? If it isn't, then the address you are passing
is incorrect, and you should modify your code to fix this problem.
	Yes.
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mylib.h"
#include "stack.h"

int main (int argc, char * const * argv) {

        Stack * main_Stack = 0;         /* the test stack */
        unsigned long amount;        /* numbers of items possible go on stack */
        long command;                   /* stack command entered by user */
        long item = 0;                  /* item to go on stack */
        char option;                    /* the command line option */
        long status;                    /* return status of stack functions */
        
        /* initialize debug states */
        debug_off ();

        /* check command line options for debug display */
        while ((option = getopt (argc, argv, "x")) != EOF) {
                
                switch (option) {
                        case 'x': debug_on (); break;
                }
        }

        while (1) {
                command = 0;            /* initialize command, need for loops */
                writeline ("\nPlease enter a command:", stdout);
                writeline ("\n\t(a)llocate, (d)eallocate, ", stdout);
                writeline ("p(u)sh, (p)op, (t)op, (i)sempty, (e)mpty, ",stdout);
                writeline ("\n\tis(f)ull, (n)um_elements, (w)rite to stdout, "
                                                                , stdout);
                writeline ("(W)rite to stderr.\n", stdout);
                writeline ("Please enter choice:  ", stdout);
                command = getchar ();
                if (command == EOF)     /* are we done? */
                        break;
                clrbuf (command);       /* get rid of extra characters */

                switch (command) {      /* process commands */

                /* YOUR CODE GOES HERE */
		case 'a':		/* allocate a stack */
			writeline ("Please enter the number of objects to be \
					able to store: ", stdout);
			amount = decin();
			clrbuf(amount);/* get rid of extra characters */
			main_Stack = new_Stack(amount);
			break;

		case 'd':		/* delete the stack */
			delete_Stack(&main_Stack);
			writeline("Stack is deleted.\n", stdout);
			break;

		case 'e':		/* empty the stack */
			empty_Stack (main_Stack);
			writeline("Stack is empty.\n", stdout);
			break;


                case 'f':               /* isfull */
                        if (isfull_Stack (main_Stack))
                                writeline ("Stack is full.\n",stdout);
                        else
                                writeline ("Stack is not full.\n", stdout);
                        break;

                case 'i':               /* isempty */
                        if (isempty_Stack (main_Stack))
                                writeline ("Stack is empty.\n", stdout);
                        else
                                writeline ("Stack is not empty.\n", stdout);
                        break;

                case 'n':               /* get_occupancy */
                        writeline ("Number of elements on the stack is:  ",
                                                                    stdout);
                        decout (get_occupancy (main_Stack));
                        newline ();
                        break;

                case 'p':               /* pop */
                        status = pop (main_Stack, &item);
                        if (! status)
                                fprintf (stderr,"\nWARNING:  pop FAILED\n");
                        else {
                                writeline (
                                        "Number popped from the stack is:  ",
                                                                    stdout);
                                decout (item);
                                newline ();
                        }
                        break;

                case 't':               /* top */
                        status = top (main_Stack, &item);
                        if (! status)
                                fprintf (stderr,"\nWARNING:  top FAILED\n");
                        else {
                                writeline (
                                        "Number at top of the stack is:  ",
                                                                    stdout);
                                decout (item);
                                newline ();
                        }
                        break;

                case 'u':               /* push */
                        writeline (
                                "\nPlease enter a number to push to stack:  ",
                                                                    stdout);
                        item = decin ();
                        clrbuf (0);     /* get rid of extra characters */
                        status = push (main_Stack, item);
                        if (! status)
                                fprintf(stderr,"\nWARNING:  push FAILED\n");
                        break;

                case 'w':               /* write */
                        writeline ("\nThe Stack contains:\n", stdout);
                        write_Stack (main_Stack, stdout);
                        break;

                case 'W':               /* write */
                        writeline ("\nThe Stack contains:\n", stdout);
                        write_Stack (main_Stack, stderr);
                        break;
                }

                if (item == EOF) /* check if done */
                        break;
        }

        if (main_Stack)
                delete_Stack (&main_Stack);     /* deallocate stack */
        newline ();
        return 0;
}
