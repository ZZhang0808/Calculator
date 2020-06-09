/****************************************************************************

                                                Zifan Zhang
                                                CSE 12, Fall 19
                                                10/29/19
                                                cs12fa19gt
                              Assignment 5

File Name:      list.c
Description:    The List and Node structs are defined. The List object contains 
		data fields such as occupancy and list_count, and also contains 
		pointers to functions which manipulate the objects stored in the
		list. The Node objects form a doubly-linked list, where the pre 
		pointer points to the previous item in the list and the next 
		pointer points to the next item in the list. The list is 
		circular, so the next pointer of the last item points to the 
		first item of the list, and the pre pointer of the first item 
		points to the last item in the list. We keep track of the last 
		item in the list with the end data field of List.
****************************************************************************/
/*---------------------------------------------------------------------------
1. end: {pre = 0x6040b0, next = 0x604070, data = 0x6040d0}
2. Node 1 data: 1
3. Node 1 pre: 0x6040f0
4. Node 1 next: 0x6040b0
5. Node 2 data: 2
6. Node 2 pre: 0x604070
7. Node 2 next: 0x6040f0
8. Node 3 data: 3
9. Node 3 pre: 0x6040b0
10. Node 3 next: 0x604070	
  ---------------------------------------------------------------------------*/
#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "list.h"

/*---------------------------------------------------------------------------
// struct Node
//
// Description: Form a doubly-linked list, where the pre pointer points to the 
// 		previous item in the list and the next pointer points to the 
// 		next item in the list.
// 
// Data Fields:
//     data (void *)  - holds the data stored in the current node
//     pre (Node *)  - the previous node on the list
//     next (Node *) - the next node on the list
//
// Public functions:
//     None
//     
  ---------------------------------------------------------------------------*/
typedef struct Node {
        struct Node * pre;      /* how to access the prior Node */
        struct Node * next;     /* how to access the next Node */
        void * data;            /* the data to store */
} Node;

/*---------------------------------------------------------------------------
// struct List
//
// Description: The List object contains data fields such as occupancy and 
// 		list_count, and also contains pointers to functions which 
// 		manipulate the objects stored in the list. The list is 
// 		circular, so the next pointer of the last item points to the 
// 		first item of the list, and the pre pointer of the first item 
// 		points to the last item in the list. We keep track of the last 
// 		item in the list with the end data field of List.
// 
// Data Fields:
//     end (Node *)  - The end Node of the list
//     list_count (long *)  - Stores the list number of this_list
//     occupancy (long *) - Stores the number of items in this_list
//     (*copy_func) (void *) - a function that copies elements
//     (*delete_func) (void) - a function that deletes elements
//     (*write_func) (FILE *) - a function that writes elements
//
// Public functions:
//     None
//
  ---------------------------------------------------------------------------*/
typedef struct List {
        Node * end;             /* the end of the list */
        long list_count;        /* which list is it */
        long occupancy;
        void *(*copy_func) (void *);
        void (*delete_func) (void *);
        FILE *(*write_func) (void *, FILE *);
} List;

/* private Node function declarations */
static long check_to_go_forward (List * this_list, long where);
static void delete_Node (Node **, void (*delete_func) (void *));
static Node * insert_Node (Node *, void *, void *(*copy_func) (void *)); 
static void locate (List * this_list, long where);
static Node * new_Node (void *, void *(*copy_func) (void *));
static void * remove_Node (Node *);
static void * view_Node (Node *); 
static FILE * write_Node (Node *, FILE *, FILE *(*write_func) (void *, FILE *));

/* catastrophic error messages */
static const char ADNEXT_NONEXIST[] = 
                "Advance next from non-existent list!!!\n";
static const char ADNEXT_EMPTY[] = 
                "Advance next from empty list!!!\n";
static const char ADPRE_NONEXIST[] = 
                "Advance pre from non-existent list!!!\n";
static const char ADPRE_EMPTY[] = 
                "Advance pre from empty list!!!\n";
static const char CHECK_NONEXIST[] =
                "Checking direction on non-existent list!!!\n";
static const char DELETE_NONEXIST[] =
                "Deleting from non-existent list!!!\n";
static const char DELETE_NONEXISTNODE[] =
                "Deleting a non-existent node!!!\n";
static const char ISEMPTY_NONEXIST[] =
                "Is empty check from non-existent list!!!\n";
static const char INSERT_NONEXIST[] =
                "Inserting to a non-existent list!!!\n";
static const char REMOVE_NONEXIST[] =
                "Removing from non-existent list!!!\n";
static const char REMOVE_EMPTY[] =
                "Remove from empty list!!!\n";
static const char VIEW_NONEXIST[] = 
                "Viewing a non-existent list!!!\n";
static const char VIEW_NONEXISTNODE[] = 
                "Viewing a non-existent node!!!\n";
static const char VIEW_EMPTY[] =
                "Viewing an empty list!!!\n";
static const char WRITE_NONEXISTFILE[] =
                "Writing to a non-existent file!!!\n";
static const char WRITE_NONEXISTLIST[] =
                "Writing from a non-existent list!!!\n";
static const char WRITE_MISSINGFUNC[] =
                "Don't know how to write out elements!!!\n";
static const char WRITE_NONEXISTNODE[] =
                "Writing from a non-existent node!!!\n";

/* debug messages */
static const char ADNEXT[] = "[List %ld - Advancing next]\n";
static const char ADPRE[] = "[List %ld - Advancing pre]\n";
static const char INSERT[] = "[List %ld - Inserting node]\n";
static const char REMOVE[] = "[List %ld - Removing node]\n";
static const char VIEW[] = "[List %ld - Viewing node]\n";
static const char LIST_ALLOCATE[] = "[List %ld has been allocated]\n";
static const char LIST_DEALLOCATE[] = "[List %ld has been deallocated]\n";

static int debug_on = FALSE;    /* allocation of debug flag */
static long list_counter = 0;   /* the number of lists allocated so far */

/*---------------------------------------------------------------------------
Function Name:		set_debug_off
Purpose:    		turn debug mode off
Description:           	set debug_on to false
Input:                	void
Output:               	None.
Result:               	debug_on set to false.  Nothing returned.
Side Effects:       	None.
---------------------------------------------------------------------------*/
void set_debug_off (void) {
        /* YOUR CODE GOES HERE */
	debug_on = FALSE;
}

/*---------------------------------------------------------------------------
Function Name:      	set_debug_on
Purpose:            	turn debug mode on
Description:           	set debug_on to true
Input:                 	void
Output:              	None.
Result:              	debug_on set to true.  Nothing returned.
Side Effects:          	None.
---------------------------------------------------------------------------*/
void set_debug_on (void) {
        /* YOUR CODE GOES HERE */
	debug_on = TRUE;
}

/*---------------------------------------------------------------------------
Function Name:        	advance_next_List
Purpose:              	Shifts the elements of the list backward by one
Description:          	Calling this function causes the end pointer of 
			this_list to move forward by one Node. This effectively
			shifts the elements of the list backwards by one.
Input:                	this_list: a pointer to the list we with to shift
Output:               	None
Result:              	Elements of the list shifted backwards by one. Nothing 
			returned.
Side Effects:       	None.
---------------------------------------------------------------------------*/
void advance_next_List (List * this_list) {
        /* YOUR CODE GOES HERE */
	if(this_list)
	{
		if(this_list -> occupancy != 0)
		{
			/* change the end Node of this_list to its 
			 * previous Node */
			this_list -> end = this_list -> end -> next;
			/* print debug message is debug mode is on */
			if(debug_on)
				fprintf(stderr, ADNEXT, this_list->list_count);
		}
		/* print error message if this_list is empty */
		else
			fprintf(stderr, ADNEXT_EMPTY);
	}
	/* print error message if this_list doesn't exist */
	else	
		fprintf(stderr, ADNEXT_NONEXIST);
}

/*---------------------------------------------------------------------------
Function Name:        	advance_pre_List
Purpose:              	Shifts the elements of the list forwards by one
Description:          	Calling this function causes the end pointer of 
			this_list to move backward by one Node. This effectively
			shifts the elements of the list forwards by one.
Input:                	this_list: a pointer to the list we with to shift
Output:               	None
Result:              	Elements of the list shifted forwards by one. Nothing 
			returned.
Side Effects:       	None.
---------------------------------------------------------------------------*/
void advance_pre_List (List * this_list) {
        /* YOUR CODE GOES HERE */
	if(this_list)
	{
		if(!isempty_List(this_list))
		{
			/* change the end Node of this_list to its 
			 * previous Node */
			this_list -> end = this_list -> end -> pre;
			/* print debug message is debug mode is on */
			if(debug_on)
				fprintf(stderr, ADPRE, this_list -> list_count);
		}
		/* print error message if this_list is empty */
		else
			fprintf(stderr, ADPRE_EMPTY);
	}
	/* print error message if this_list doesn't exist */
	else	
		fprintf(stderr, ADPRE_NONEXIST);
}

/*---------------------------------------------------------------------------
Function Name:      	check_to_go_forward
Purpose:            	This function checks whether it would be more efficient
			to reach item number where in the list by looping 
			forwards from the end of the list (true is returned) or
		       	backwards from the end of the list (false is returned)
Description:           	This function checks whether it would be more efficient
			to reach item number where in the list by looping 
			forwards from the end of the list (true is returned) or
		       	backwards from the end of the list (false is returned)
Input:                 	this_list: a pointer to the list we wish to check
			where: the number of the item in the list
Output:              	True to loop forward, false for loop backward
Result:              	Result of looping direction returned
Side Effects:          	None.
---------------------------------------------------------------------------*/
static long check_to_go_forward (List * this_list, long where) {
        /* YOUR CODE GOES HERE */
	/* print error message if this_list doesn't exist */
	if(!this_list){
		fprintf(stderr, CHECK_NONEXIST);
		return 0;
	}
	/* return true if where is less than half of occupancy of this_list */
	if((this_list -> occupancy / 2 + 1) >= where)
		return 1;
	/* return false other wise */
	return 0;
}

/*---------------------------------------------------------------------------
Function Name:      	delete_List
Purpose:            	delete a list
Description:           	This destructor function deallocates all memory 
			associated with the list, including the memory 
			associated with all of the nodes in the list. It sets 
			the list pointer in the calling function to NULL.
Input:                 	lpp: a double pointer to the list that is to be deleted
Output:              	None.
Result:              	target list deleted.  Nothing returned.
Side Effects:          	None.
---------------------------------------------------------------------------*/
void delete_List (List ** lpp) {
        /* YOUR CODE GOES HERE */
	List * this_list; /* dereference lpp so program is more readable */
	Node * working; /* holds the working Node */
	Node * next; /* holds the next working Node */
	this_list = * lpp; /* dereference lpp and store in this_list */
	next = this_list->end; /* assign next Node to the end Node */
	if(!this_list)	/* print error message if this_list doesn't exist */
		fprintf(stderr, DELETE_NONEXIST);
	else{
		/* loop through each Node in the list and delete them */
		while(!isempty_List(this_list))
		{
			working = next;
			next = working -> next;
			/* call delete_Node to delete Node */
			delete_Node(&working, this_list->delete_func);
			/* decrement occupancy after deleting a Node */
			this_list -> occupancy --;
		}
		/* if debug mode is on print debugging messages */
		if(debug_on)
			fprintf(stderr, LIST_DEALLOCATE, 
					this_list -> list_count);
		/* free the memory of the list */
		free(*lpp);
		/* assign the double list pointer to NULL */
		lpp = NULL;
		/* decrement list_counter after deleting the list */
		list_counter--;	
	}
}

/*---------------------------------------------------------------------------
Function Name:      	insert
Purpose:            	insert a number to a list at a selected location
Description:           	Inserts the element into this_list at location where. 
Input:                 	this_list: a pointer to the list we wish to check
			element: a pointer to the object we wish to insert in
			the list
			where: the place in the list where element should be
			stored afer the call to insert
Output:              	Return 1 for success, 0 for failed to insert
Result:              	Success or failed returned.
Side Effects:          	None.
---------------------------------------------------------------------------*/
long insert (List * this_list, void * element, long where) {
        /* YOUR CODE GOES HERE */
	Node * temp; /* temporary copy of the end Node */
	/* print error message if this_list doesn't exist */
	if(!this_list)
	{
		fprintf(stderr, INSERT_NONEXIST);
		return 0;
	}
	/* print debug message if debug mode is on */
	if(debug_on)
		fprintf(stderr, INSERT, this_list -> list_count);
	/* if this_list is not empty, call insert_Node to incorporate the
	 * Node into the list */
	if(!isempty_List(this_list))
	{
		/* the end Node to temp */
		temp = this_list -> end;
		/* if insert from the end, also insert after the first element
		 * but set the new element to the end after. Else insert after
		 * the given location */
		/* locate the Node to insert after */
		locate(this_list, where?where:1);
		/* call insert_Node to insert the Node after the located Node */
		insert_Node(this_list -> end, element, this_list -> copy_func);
		/* after insertion increment occupancy of this_list */
		this_list -> occupancy ++;
		/* change the end Node back */
		this_list -> end = temp;
		/* if the element was inserted to the end, set it to the end */
		if(where == 0)
			this_list -> end = temp -> next;
	}
	/* if this_list is empty, just add a Node to this_list and set its pre
	 * and next to itself and end of this_list to that Node */
	else
	{
		this_list -> end = new_Node(element, this_list -> copy_func);
		this_list -> end -> next = this_list -> end;
		this_list -> end -> pre = this_list -> end;
		this_list -> occupancy ++;
	}
	/* return true if successfully inserted */	
	return 1;
	
}
 
/*---------------------------------------------------------------------------
Function Name:      	isempty_list
Purpose:            	checks to see if the list is empty
Description:            checks to see if this_list is empty
Input:                 	this_list: a pointer to the list we wish to check
Output:              	True if the list is empty, false if not
Result:              	Emptyness of the list returned.
Side Effects:          	None.
---------------------------------------------------------------------------*/
long isempty_List (List * this_list) {
        /* YOUR CODE GOES HERE */
	if(!this_list)	/* print error message if this_list doesn't exist */
		fprintf(stderr, ISEMPTY_NONEXIST);
	/* return true if occupancy of this_list is 0 */
	else if(this_list -> occupancy == 0)
		return 1;
	/* else return false */
	return 0;
}

/*---------------------------------------------------------------------------
Function Name:      	locate
Purpose:            	locate the item on the list at a given location
Description:           	This function is used to eliminate code duplication in 
			the list functions by finding the location at which we 
			wish to insert, remove, or view. locate should be used 
			to locate the Node at location where.
Input:                 	this_list: a pointer to the list we with to check
			where: the number of item in the list
Output:              	None.
Result:              	Item located.  Nothing returned.
Side Effects:          	None.
---------------------------------------------------------------------------*/
static void locate (List * this_list, long where) {
        /* YOUR CODE GOES HERE */
	int index; /* index of the number of loops passed */
	/* check weather looping forward or backward is more efficient */
	if(check_to_go_forward(this_list, where))
	{
		/* advance next until reaches the target Node */
		for(index = 0; index < where - 1; index++)
		{
			advance_next_List(this_list);
		}
	}
	else
	{
		/* advance pre until reaches the target Node */
		for(index = 0; index <= this_list -> occupancy - where; index++)
		{
			advance_pre_List(this_list);
		}
	}
}

/*---------------------------------------------------------------------------
Function Name:      	new_List
Purpose:            	create a new list object
Description:           	Constructor method for List object, allocate memory
			for the list as well as initialize it.
Input:                 	copy_func: a pointer to the function which makes copies
			of the elements stored in this_list
			delete_func: a pointer to the function which frees the
			memory associated with elements soted in thes_list
			write_func: a pointer to the function which writes 
			elements in this_list
Output:              	A pointer to the new list.
Result:              	New list created, a pointer to it returned.
Side Effects:          	None.
---------------------------------------------------------------------------*/
List * new_List (
        void *(*copy_func) (void *),
        void (*delete_func) (void *),
        FILE *(*write_func) (void *, FILE *)) {

        /* YOUR CODE GOES HERE */
	/* allocate */
	List * this_List = (List *)malloc(sizeof(List));
	/* allocate debug message */
	if(debug_on)
		fprintf(stderr, LIST_ALLOCATE, ++list_counter);
	/* initialize */
	this_List -> end = NULL;
	this_List -> occupancy = 0;
	this_List -> list_count = list_counter;
	this_List -> copy_func = copy_func;
	this_List -> delete_func = delete_func;
	this_List -> write_func = write_func;
	/* return the pointer to the created list */
	return this_List;
}

/*---------------------------------------------------------------------------
Function Name:      	remove_List
Purpose:            	Remove an item from list
Description:           	Removes an element in this_list at location where
Input:                 	this_list: a pointer to the list from which we with
			to remove
 			where: the place in the number of the element in the
			list we wish to remove
Output:              	A pointer to the data
Result:              	Item removed, the pointer to the removed data returned.
Side Effects:          	None.
---------------------------------------------------------------------------*/
void * remove_List (List * this_list, long where) {
        /* YOUR CODE GOES HERE */
	Node * temp; /* temporary copy of end Node */
	void * data; /* holds the data stored in the Node */
	if (!this_list) {
		/* print error message if this_list doesn't exist */
                fprintf (stderr, REMOVE_NONEXIST);
                return NULL;
        }
	/* print error message if this_list is empty */
	if(isempty_List(this_list)){
		fprintf (stderr, VIEW_EMPTY);
		return NULL;
	}
	/* print debug message if debug mode is on */
	if(debug_on)
		fprintf(stderr, REMOVE, this_list -> list_count);
	/* if the Node removing is the end Node, copy the previous Node
	 * else copy the end Node */
	temp = (where == 0 || where == this_list -> occupancy)?
		this_list -> end -> pre: this_list -> end;
	/* locate the Node we are removing */
	locate(this_list, where + 1);
	/* remove the Node and save its data to data */
	data = remove_Node(this_list -> end);
	/* change the end Node back */
	this_list -> end = temp;
	/* decrement the occupancy */
	this_list -> occupancy--;
	/* if the list is empty change the end Node to NULL */
	if(isempty_List(this_list))
		this_list -> end = NULL;
	/* return the data */
	return data;
}

/*---------------------------------------------------------------------------
Function Name:      	view
Purpose:            	View the data stored in the list at a given loction
Description:           	Returns a pointer to the object stored at location 
			where for viewing
Input:                 	this_list: a pointer to the list we wish to check
			where: the place in the list which holds the elements 
			to view
Output:              	A pointer to the element
Result:              	The pointer to the element returned.
Side Effects:          	None.
---------------------------------------------------------------------------*/
void * view (List * this_list, long where) {
        /* YOUR CODE GOES HERE */
	Node * temp;
	void * data;
	/* print error message if this_list doesn't exist */
	if (!this_list) {
                fprintf (stderr, VIEW_NONEXIST);
                return NULL;
        }
	/* print error message if this_list is empty */
	if(isempty_List(this_list)){
		fprintf (stderr, VIEW_EMPTY);
		return NULL;
	}
	/* print debug message if debug mode is on */
	if(debug_on)
		fprintf(stderr, VIEW, this_list -> list_count);
       	temp = this_list -> end;
	locate(this_list, where + 1);
	data = view_Node(this_list -> end);
	this_list -> end = temp;
	return data;
}

/*---------------------------------------------------------------------------
Function Name:      	write_List
Purpose:            	write the elements stored on the list
Description:           	Writes the elements of this_list forwards, starting
			with the first item. The list is printed to filestream
			stream.
Input:                 	stream: stdout or stderr
			this_list: a pointer to the list we wish to write
Output:              	stream
Result:              	Elements of the list written, stream returned.
Side Effects:          	None.
---------------------------------------------------------------------------*/
FILE * write_List (List * this_list, FILE * stream) {
        long count;             /* to know how many elements to print */
        Node * working;                 /* working node */
	/* print error message if stream doesn't exist */
        if (!stream) {
                fprintf (stderr, WRITE_NONEXISTFILE);
                return NULL;
        }
	/* print error message if this_list doesn't exist */
        if (!this_list) {
                fprintf (stderr, WRITE_NONEXISTLIST);
                return NULL;
        }
	/* if we are printing to stderr, print number of items on the list */
        if (stream == stderr)
                fprintf (stream, "List %ld has %ld items in it.\n",
                        this_list->list_count, this_list->occupancy);
	/* if this_list doesn't have a write_func, print error message */
        if (!this_list->write_func) {
                fprintf (stream, WRITE_MISSINGFUNC);
                return stream;
        }
	/* assign the working Node to the first Node of the list if list 
	 * is not empty */
        if (this_list->occupancy >= 1)
                working = this_list->end->next;
	/* loop through each Node in the list and print them out */
        for (count = 1; count <= this_list->occupancy; count++) {
		/* if printing to stderr print the Node number as well */
                if (stream == stderr)
                        fprintf (stream, "\nelement %ld:  ", count);
                write_Node (working, stream, this_list->write_func);
                working = working->next;
        }
        /* return the stream */
        return stream;
}

/*---------------------------------------------------------------------------
Function Name:      	write_reverse_List
Purpose:            	write the elements stored on the list backwards
Description:           	Writes the elements of this_list backwards, starting
			with the last item. The list is printed to filestream
			stream.
Input:                 	stream: stdout or stderr
			this_list: a pointer to the list we wish to write
Output:              	stream
Result:              	Elements of the list written, stream returned.
Side Effects:          	None.
---------------------------------------------------------------------------*/
FILE * write_reverse_List (List * this_list, FILE * stream) {
        /* YOUR CODE GOES HERE */
	long count;             /* to know how many elements to print */
        Node * working;                 /* working node */
	/* print error message if stream doesn't exist */
        if (!stream) {
                fprintf (stderr, WRITE_NONEXISTFILE);
                return NULL;
        }
	/* print error message if this_list doesn't exist */
        if (!this_list) {
                fprintf (stderr, WRITE_NONEXISTLIST);
                return NULL;
        }
	/* if we are printing to stderr, print number of items on the list */
        if (stream == stderr)
                fprintf (stream, "List %ld has %ld items in it.\n",
                        this_list->list_count, this_list->occupancy);
	/* if this_list doesn't have a write_func, print error message */
        if (!this_list->write_func) {
                fprintf (stream, WRITE_MISSINGFUNC);
                return stream;
        }
	/* assign the working Node to the end Node of the list */
	working = this_list->end;
	/* loop through each Node in the list and print them out */
        for (count = this_list -> occupancy; count > 0; count--) {
		/* if printing to stderr print the Node number as well */
                if (stream == stderr)
                        fprintf (stream, "\nelement %ld:  ", count);
                write_Node (working, stream, this_list->write_func);
                working = working->pre;
        }
        /* return the stream */
        return stream;


}

/*---------------------------------------------------------------------------
Function Name:      	delete_Node
Purpose:            	delete a node and free memory associated with it
Description:           	Delete a node and free all memory associated with it
			as well as the Node's data
Input:                 	npp: a double pointer to the Node that is being deleted
			delete_func: a pointer to the function which frees
			the memory associated with element stored in the Node
Output:              	None.
Result:              	Node deleted.  Nothing returned.
Side Effects:          	None.
---------------------------------------------------------------------------*/
static void delete_Node (Node ** npp, void (*delete_func) (void *)) {

        /* does the node exist??? */
        if (!npp || !*npp) {
                fprintf (stderr, DELETE_NONEXISTNODE);
                return;
        }

        /* call function to delete element */
        if (delete_func && (*npp)->data)
                (*delete_func) (&((*npp)->data));

        /* delete element */
        free (*npp);

        /* assign node to NULL */
        *npp = NULL;
}

/*---------------------------------------------------------------------------
Function Name:      	insert_Node
Purpose:            	insert an element to a list at given location
Description:           	Creates a new node to hold element, or, if copy_func is
			non-NULL, a copy of element. This new node is then 
			incorporated into the list at the location AFTER 
			this_Node. Returns a pointer to the new node that has 
			been inserted into the list.
Input:                 	thi_Node: a pointer to the Node after which we wish
			to insert the new node
			element: a pointer to the object we wish to store in
			the new node
			copy_func: the pointer to the copy function passed into
			the list constructor or NULL if no copy needs to be 
			made.
Output:              	the pointer to the new Node
Result:              	Element inserted to the list, pointer to the new Node
			returned
Side Effects:          	None.
---------------------------------------------------------------------------*/
static Node * insert_Node (Node * this_Node, void * element, 
        void * (*copy_func) (void *)) {

        /* YOUR CODE GOES HERE */
	Node *thisnew_Node; /* a new Node to be inserted */
	/* initialize thisnew_Node */
       	thisnew_Node = new_Node(element, copy_func); 
	thisnew_Node -> next = this_Node -> next;
	thisnew_Node -> pre = this_Node;
	this_Node -> next = thisnew_Node;
	thisnew_Node -> next -> pre = thisnew_Node;
	/* return the pointer to thisnew_Node */
	return thisnew_Node;
}								

/*---------------------------------------------------------------------------
Function Name:      	new_Node
Purpose:            	Create a new Node object
Description:           	Constructor method of Node object, allocate memory
			for a new Node and initialize it.
Input:                 	element: a pointer to the data to be stored in the Node
			copy_func: a pointer to the function to copy element,
			it is Null when it is not needed
Output:              	A pointer to the newly created Node
Result:              	New Node created, its pointer returned
Side Effects:          	None.
---------------------------------------------------------------------------*/
static Node* new_Node (void * element, void * (*copy_func) (void *)) {

        /* allocate memory */
        Node *this_Node = (Node *) malloc (sizeof (Node));

        /* initialize memory */
        this_Node->next = this_Node->pre = NULL;
        this_Node->data = (copy_func) ? (*copy_func) (element) : element;

        return this_Node;
}

/*---------------------------------------------------------------------------
Function Name:      	remove_Node
Purpose:            	remove a Node from the list
Description:           	"Unlinks" this_Node from the list by arranging the 
			pointers of the surrounding Nodes so they no longer 
			point to this_Node. The memory associated with the Node
		       	object is freed, but the Node's data is not deleted. 
			A pointer to the data is returned.
Input:                 	this_Node: a pointer to the Node we wish to remove from
			the list
Output:              	A pointer to the data stored in this_Node
Result:              	this_Node removed from the list, data stored in
			this_Node returned
Side Effects:          	None.
---------------------------------------------------------------------------*/
static void * remove_Node (Node * this_Node) {
        /* YOUR CODE GOES HERE */ 
	void * data; /* holds data stored in this_Node */
	/* arranging the pointers of the surrounding Nodes so they no longer
	 * point to this_Node */
	this_Node -> next -> pre = this_Node -> pre;
	this_Node -> pre -> next = this_Node -> next;
	/* store this_Node's data to data */
	data = this_Node -> data;
	/* delete this_Node afte arranging the pointers of the surrounding Nodes
	 * and takes out the pointer to the data */
	delete_Node(&this_Node, NULL);
	/* return the data stored in this_Node */
	return data;
}

/*---------------------------------------------------------------------------
Function Name:      	view_Node
Purpose:            	view the data stored in a Node
Description:           	Returns a pointer to this_Node's data
Input:                 	this_Node: a pointer to the Node whose data we wish
			to view
Output:              	A pointer to the data stored in this_Node
Result:              	data stored in this_Node returned
Side Effects:          	None.
---------------------------------------------------------------------------*/
static void * view_Node (Node * this_Node) {
        /* YOUR CODE GOES HERE */
	/* print error message if this_Node doesn't exist */
	if(!this_Node)
		fprintf(stderr, VIEW_NONEXISTNODE);
	/* if the node exist, return its data */
	return this_Node -> data;
}

/*---------------------------------------------------------------------------
Function Name:      	set_debug_on
Purpose:            	turn debug mode on
Description:           	set debug_on to true
Input:                 	void
Output:              	None.
Result:              	debug_on set to true.  Nothing returned.
Side Effects:          	None.
---------------------------------------------------------------------------*/
static FILE* write_Node (Node * this_Node, FILE * stream,
        FILE * (*write_func) (void *, FILE *)) {

        if (!stream) {
                fprintf (stderr, WRITE_NONEXISTFILE);
                return NULL;
        }

        if (!this_Node) {
                fprintf (stream, WRITE_NONEXISTNODE);
                return stream;
        }

        if (!write_func) {
                fprintf (stream, WRITE_MISSINGFUNC);
                return stream;
        }

        return (*write_func) (this_Node->data, stream);
}
