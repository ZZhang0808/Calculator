/**
 * Assignment SIX
 * HashTable.java
 * CSE 12, Fall 19
 * 11/5/2019
 * This pogram implement a Symbol Table to allow variable 
		assignment to the calculator. 
 *
 * @author Zifan Zhang (cs12fa19gt)
 */ 

//=========================================================================
// public class HashTable extends Base
//
// Description: Implements the Hash table that stores objects and can look
// 		up for an object fast 
//
// Data Fields:
//     counter (int)  - number of the HashTable so far
//     debug (boolean)  - debugging state
//     occupancy (long) - number of items stored in table
//     size (int) - size of hash table
//     table (Base) - he Hash able itself ==> array of Base
//     tableCount (int) - which hash table it is
//     index (int) - last location checked in hash table
//
// Public functions:
//     setDebugOn - turns on debugging for this HashTable
//     setDebugOff - Turns off debugging for this HashTable
//     HashTable Constructor - Allocates and initializes the memory associated 
//     				with a hash table.
//     insert - Inserts the element in the hash table. Returns true or false 
//     		indicating success of insertion.
//     locate - Locates the index in the table where the insertion is to be 
//     performed, an item is found, or an item is determined not to be there. 
//     Sets the variable index to the last location checked
//     lookup - Looks up the element in the hash table. Returns pointer to the 
//     element if found, null otherwise.
//==========================================================================

public class HashTable extends Base {

        /* counters, flags and constants */
        private static int counter = 0;         // number of HashTables so far
        private static boolean debug;           // allocation of debug states
        protected static final short NULL = 0;  // in case you want to use NULL

        /* data fields */
        private long occupancy;         // number of items stored in table
        private int size;               // size of hash table
        private Base table[];   // the Hash Table itself ==> array of Base
        private int tableCount; // which hash table it is 

        /* initialized by Locate function */
        private int index;      // last location checked in hash table

        /* debug messages */
        private static final String DEBUG_ALLOCATE = " - Allocated]\n";
        private static final String DEBUG_INSERT = " - Insert]\n";
        private static final String DEBUG_LOCATE = " - Locate]\n";
        private static final String DEBUG_LOOKUP = " - Lookup]\n";
        private static final String FULL = " is full...aborting...]\n";
        private static final String HASH = "[Hash Table ";
        private static final String HASH_VAL = "[Hash value is: ";
        private static final String INSERT_BUMP = 
                                        "[bumping to next location...]\n";
        private static final String PROCESSING = "[Processing "; 
        private static final String TRYING = "[Trying index "; 

	private static final String END = "]\n";
	private static final String HASH_VAL_END = "]";

        /**
	 * setDebugOff
         * This function sets debug mode off
         */
        public static void setDebugOff () {

                /* YOUR CODE GOES HERE */
		debug = false;
        }       

        /**
	 * setDebugOn
         * This function sets debug mode on
         */
        public static void setDebugOn () {

                /* YOUR CODE GOES HERE */
		debug = true;
        }       
        
        /**
         * HashTable
         * Allocates and initializes the memory associated with a hash
         * table.
         *
         * @param  sz   The number of elements for the table...MUST BE PRIME!
         */
        public HashTable (int sz) {

                /* YOUR CODE GOES HERE */
		/* initialization */
		counter++;
		occupancy = 0;
		size = sz;
		table = new Base[sz];
		tableCount = counter;
		index = 0;
		/* print debug message */
		if(debug)
			System.out.print(HASH + tableCount + DEBUG_ALLOCATE);
        }

        /**
	 * insert
         * Performs insertion into the tabble via delegation to the
         * private insert method.
         *
         * @param   element        The element to insert.
         * @return  true or false indicating success or failure of insertion
         */
        public boolean insert (Base element) {
		//call insert recursively
                return insert (element, false);		
	}

        /**
         * Inserts the element in the hash table.
         * If the element cannot be inserted, false will be returned.  
         * If the element can be inserted, the element is inserted and true is
         * returned. Duplicate insertions will cause the existing element
         * to be deleted, and the duplicate element to take its place.
         *
         * @param   element        The element to insert.
         * @param   recursiveCall  whether or not this function was called 
	 * 				recursively
         * @return  true or false indicating success or failure of insertion
         */
        private boolean insert (Base element, boolean recursiveCall) {

                /* YOUR CODE GOES HERE */
		//print debug message
		if(debug)
			System.out.print(HASH + tableCount + DEBUG_INSERT);
		//preset index to 0 if this function was not called recursively
		if(!recursiveCall)
			index = -1;
		//locate the element on table
		locate(element);
		//if table[index] is empty then insert the element at index
		if(table[index] == null)
		{
			table[index] = element;
			occupancy++;
			return true;
		}
		//if table[index] and element have the same key, update
		//table[index]
		if(table[index].equals(element))
		{
			table[index] = element;
			return true;
		}
		//if table is full return false and error message
		if(occupancy == size)
		{
			System.out.print(HASH + tableCount + FULL);
			return false;
		}
		//if element's key is greater than table[index]'s use bully
		//algorithm to push table[index] to the next spot
		if(table[index].isLessThan(element))
		{
			if(debug)
				System.out.print(INSERT_BUMP);
			Base temp = table[index];
			table[index] = element;
			return insert(temp, true);
		}
		//print error message indicates that the table is full
                return false;
        }

        /**
         * This function will locate the location in the
                table for the insert or lookup.
         *
         * @param   element  The element needing a location
         * @return  item found, or null if not found
         */
        private Base locate (Base element) {

                /* YOUR CODE GOES HERE */
		int ascii; //stores hash code of element
		int increment; //stores increment value of element
		//print debug message
		if(debug)
		{
			System.out.print(HASH + tableCount + DEBUG_LOCATE);
			System.out.print(PROCESSING + element.getName() + END);
		}
		//assign ascii the hash code of element
		ascii = element.hashCode();
		//print debug message
		if(debug)
			System.out.println(HASH_VAL + ascii + HASH_VAL_END);
		//assign increment
		increment = (ascii % (size - 1)) + 1;
		//process index if it had been set to -1
		if(index == -1)
			index = (ascii % size) - increment;
		//loop to find the location that matches element's key
		for(int i = 0; i < size; i++)
		{
			//increment index
			index += increment;
			//prevent index from out of bound
			if(index >= size)
				index -= size;
			//print debug message
			if(debug)
				System.out.print(TRYING + index + END);
			//return null if table[index] doesn't exist or
			//key of table[index] is less than element's
			if(table[index] == null || 
					table[index].isLessThan(element))
				break;
			//return table[index] if its key matches element's
			if(element.equals(table[index]))
				return table[index];
		}
                return null;
        }

        /**
         * This function will lookup the element in the hash table.  If
               found a pointer to the element is returned.  If the element
               is not found, NULL will be returned to the user.
         *
         * @param   element  The element to insert or to lookup.
         * @return  the element if found, else NULL
         */
        public Base lookup (Base element) {

                /* YOUR CODE GOES HERE */
		//print debug message
		if(debug)
			System.out.print(HASH + tableCount + DEBUG_LOOKUP);
		//reset index
		index = 0;
		//locate element on table
		locate(element);
		//return null if element is not on the table
		if(table[index] == null)
			return null;
		//return table[index] if found
		if(table[index].equals(element))
			return table[index];
		//else null
                return null;
        }

        /**
         * Creates a string representation of the hash table. The method 
         * traverses the entire table, adding elements one by one
         * according to their index in the table. 
         *
         * @return  String representation of hash table
         */
        public String toString () {

                String string = "Hash Table " + tableCount + ":\n";
                string += "size is " + size + " elements, "; 
                string += "occupancy is " + occupancy + " elements.\n";

                /* go through all table elements */
                for (int index = 0; index < size; index++) {

                        if (table[index] != null) {
                                string += "at index " + index + ":  ";
                                string += "" + table[index] + "\n"; 
                        }
                }

                return string;
        }
}
