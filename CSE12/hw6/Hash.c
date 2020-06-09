/**************************************************************************** 
 
                                                                Zifan Zhang 
                                                         	CSE 12, Fall 19 
                                                                11/5/2019 
                                                             	cs12fa19gt 
                		Assignment SIX
 
File Name:  	Hash.c
Description:    This pogram implement a Symbol Table to allow variable 
		assignment to the calculator. 
 ****************************************************************************/
//=========================================================================
// class HashTable : public Base
//
// Description: Implements the Hash table that stores objects and can look
// 		up for an object fast 
//
// Data Fields:
//     counter (int)  - number of the HashTable so far
//     debug (boolean)  - debugging state
//     occupancy (long) - number of items stored in table
//     size (long) - size of hash table
//     table (Base**) - he Hash able itself ==> array of Base
//     table_count (int) - which hash table it is
//     index (int) - last location checked in hash table
//
// Public functions:
//     set_Debug - turns on and off debugging for this HashTable
//     HashTable Constructor - Allocates and initializes the memory associated 
//     				with a hash table.
//     HashTable Destructor - Called when de-allocating this HashTable.
//     insert - Inserts the element in the hash table. Returns true or false 
//     		indicating success of insertion.
//     locate - Locates the index in the table where the insertion is to be 
//     performed, an item is found, or an item is determined not to be there. 
//     Sets the variable index to the last location checked
//     lookup - Looks up the element in the hash table. Returns pointer to the 
//     element if found, null otherwise.
//==========================================================================
#include <cstdlib>
#include <string.h>
#include "Hash.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

using namespace std;

/* debug messages */
static const char DEBUG_ALLOCATE[] = " - Allocated]\n";
static const char DEBUG_INSERT[] = " - Insert]\n";
static const char DEBUG_LOCATE[] = " - Locate]\n";
static const char DEBUG_LOOKUP[] = " - Lookup]\n";
static const char FULL[] = " is full...aborting...]\n";
static const char HASH[] = "[Hash Table ";
static const char HASH_VAL[] = "[Hash value is: ";
static const char INSERT_BUMP[] = "[bumping to next location...]\n";
static const char PROCESSING[] = "[Processing ";
static const char TRYING[] = "[Trying index ";

static const char END[] = "]\n";
static const char HASH_VAL_END[] = "]";


long HashTable :: debug = 0;
int HashTable :: counter = 0;


void HashTable :: Set_Debug (long option)

/***************************************************************************
% Routine Name : HashTable :: Set_Debug (public)
% File :         Hash.c
% 
% Description :  This function sets debug mode on or off
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% option             true should set debug mode on, false should set debug
%                    mode off.
***************************************************************************/

{
        /* YOUR CODE GOES HERE */
	debug = option;
}


HashTable :: HashTable (long sz) : size (sz),
        table_count(++counter), occupancy (0), table (new Base *[sz])
/***************************************************************************
% Routine Name : HashTable :: HashTable (public)
% File :         Hash.c
% 
% Description :  This function allocates an initializes the memory
%                associated with a hash table.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% size               The number of elements for the table...MUST BE PRIME!!!
***************************************************************************/

{
        /* YOUR CODE GOES HERE */

        /* initialize all table elements */
	for(int i = 0; i < sz; i++)
	{
		table[i] = 0;
	}


}	/* end: HashTable */


HashTable :: ~HashTable (void)
/***************************************************************************
% Routine Name : HashTable :: ~HashTable  (public)
% File :         Hash.c
% 
% Description :  deallocates memory associated with the Hash Table.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
        /* YOUR CODE GOES HERE */

	/* call function to delete individual elements */
	for(int i = 0; i < size; i++)
	{
		delete(table[i]);
	}
	/* delete table itself */
	delete(table);
}	/* end: ~HashTable */


long HashTable :: Insert (Base * element, long recursiveCall)
/***************************************************************************
% Routine Name : HashTable :: Insert (public)
% File :         Hash.c
% 
% Description : This function will insert the element in the hash table.
%		If the element cannot be inserted, false will be returned.
                If the element can be inserted, the element is inserted and 
		true is returned. Duplicate insertions will cause the existing
		element to be deleted, and the duplicate element to take its
		place.
%
% Parameters descriptions :
%  
% name               description
% ------------------ ------------------------------------------------------
% element            The element to insert.
% recursiveCall      Whether the call to this function is recursive or not.
% <return>           1 or 0 indicating success or failure of insertion
***************************************************************************/

{
        /* print debug message */
	if(debug)
	{
		cerr << HASH << table_count << DEBUG_INSERT;
	}
	/* preset index o -1 if this function wasn't called recursively */
	if(!recursiveCall)
	{
		index = -1;
	}
	/* locate hash on the table */
	Locate(element);
	/* if table[index] is empty insert element at table[index] */
	if(!table[index])
	{
		table[index] = element;
		occupancy++;
		return true;
	}
	/* if table[index] and element have the same key update table[index] */
	if(*table[index] == *element)
	{
		table[index] = element;
		return true;
	}
	/* return false if table is full and print error message */
	if(occupancy == size)
	{
		cerr << HASH << table_count << FULL;
		return false;
	}
	/* if element's key is greater table[index]'s use bully algorithm to
	 * push table[index] to the next spot */
	if(*table[index] < *element)
	{
		if(debug)
		{
			cerr << INSERT_BUMP;
		}
		Base * temp = table[index];
		table[index] = element;
		return Insert(temp, true);
	}
	/* print error message indicates that the table is full */
	return FALSE;
}


const Base * HashTable :: Locate (const Base * element) const
/***************************************************************************
% Routine Name : HashTable :: Locate (private)
% File :         Hash.c
% 
% Description : This function will locate the location in the
                table for the insert or lookup.
%
% Parameters descriptions :
%  
% name               description
% ------------------ ------------------------------------------------------
% element            The element needing a location
% <return>           item found, or null if not found

***************************************************************************/

{
        /* YOUR CODE GOES HERE */
	long ascii; /* stores hash code of element */
	long increment; /* stores increment value of element */
	/* print debug message */
	if(debug)
	{
		cerr << HASH << table_count << DEBUG_LOCATE;
		cerr << PROCESSING << (const char *)*element << END;
	}
	/* assign ascii the hash code of element */
	ascii = (long)*element;
	/* print debug message */
	if(debug)
	{
		cerr << HASH_VAL << ascii << END;
	}
	/* assign increment */
	increment = (ascii % (size - 1)) + 1;
	/* process index if it had been set to -1 */
	if(index == -1)
	{
		index = (ascii % size) - increment;
	}
	/* loop to find the location that matches element's key */
	for(int i = 0; i < size; i++)
	{
		/* increment index */
		index += increment;
		/* prevent index from out of bound */
		if(index >= size)
		{
			index -= size;
		}
		/* print debug message */
		if(debug)
		{
			cout << TRYING << index << END;
		}
		/* return null if table[index] doesn't exist or key is less
		 * than element's */
		if(!table[index] || *table[index] < *element)
		{
			break;
		}
		/* return table[index] if its key matches element's */
		if(*element == *table[index])
		{
			return table[index];
		}
	}
	return 0;
}


const Base * HashTable :: Lookup (const Base * element) const
/***************************************************************************
% Routine Name : HashTable :: Lookup (public)
% File :         Hash.c
% 
% Description : This function will lookup the element in the hash table.  If
%               found a pointer to the element is returned.  If the element
%               is not found, NULL will be returned to the user.
%
% Parameters descriptions :
%  
% name               description
% ------------------ ------------------------------------------------------
% element            The element to insert or to lookup.
% <return>           A pointer to the element if found, else NULL
***************************************************************************/

{
        /* YOUR CODE GOES HERE */
	/* print debug message */
	if(debug)
		cerr << HASH << table_count << DEBUG_LOOKUP;
	/* reset index */
	index = 0;
	/* return the element if found */
	if(Locate(element)){
		return table[index];
	}
	/* else null */
	return 0;
}


ostream & HashTable :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : HashTable :: Write (public)
% File :         hash.c
% 
% Description : This funtion will output the contents of the hash table
%               to the stream specificed by the caller.  The stream could be
%               cerr, cout, or any other valid stream.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% stream             A reference to the output stream.
% <return>           A reference to the output stream.
***************************************************************************/

{
	stream << "Hash Table " << table_count << ":\n"
		<< "size is " << size << " elements, "
		<< "occupancy is " << occupancy << " elements.\n";

	/* go through all table elements */
	for (long index = 0; index < size; index++)
		if (table[index])
			table[index]->Write(stream << "at index "
			<< index << ":  ") << "\n";
	return stream;
}	/* end: Write */

