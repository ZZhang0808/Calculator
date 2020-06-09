/**
 * Assignment SEVEN
 * Tree.java
 * CSE 12, Fall 19
 * 11/12/2019
 * This pogram implement a Binary Tree to allow variable 
		assignment to the calculator. 
 *
 * @author Zifan Zhang (cs12fa19gt)
 */ 
//=========================================================================
// public class Tree<Whatever extends Base> extends Base
//
// Description: Implements the binary tree that stores objects and can look
// 		up for an object fast 
//
// Data Fields:
//     debug (boolean)  - debugging state
//     occupancy (long) - number of items stored in the tree
//     root (TNode) - the TNode without parent in hash table
//     treeName(String) - the name of the tree
//
// Public functions:
//     debugOn - turns on debugging for this HashTable
//     debugOff - Turns off debugging for this HashTable
//     Tree Constructor - Allocates and initializes the memory associated 
//     				with a binary tree.
//     isEmpty - Indicates that if the tree is empty
//     insert - Inserts the element in the binary tree. Returns true or false 
//     		indicating success of insertion.
//     lookup - Looks up the element in the binary tree. Returns pointer to the
//     		element if found, null otherwise.
//     remove - Removes the matching data from the binary tree. Returns a 
//     		pointer to the data if found, null otherwise.
//==========================================================================
public class Tree<Whatever extends Base> extends Base {

        /* data fields */
        private TNode root;
        private long occupancy; 
        private String treeName;

        /* debug flag */
        private static boolean debug;

        /* debug messages */
        private static final String ALLOCATE = " - Allocating]\n";
        private static final String AND = " and ";
        private static final String COMPARE = " - Comparing ";
        private static final String INSERT = " - Inserting ";
        private static final String TREE = "[Tree ";
	private static final String END = "]\n";

	/**
         * Tree
         * Allocates and initializes the memory associated with a binary tree
         *
         * @param  name   The name of the binary table
         */
        public Tree (String name) {

                /* YOUR CODE GOES HERE */
		/* print debug message */
		if(debug)
			System.out.print(TREE + name + ALLOCATE);
		/* initialization */
		treeName = name;
		root = null;
		occupancy = 0;
        }
	/**
	 * debugOff
         * This function sets debug mode off
         */
        public static void debugOff () {

                /* YOUR CODE GOES HERE */
		debug = false;
        }
	/**
	 * debugOn
         * This function sets debug mode off
         */
        public static void debugOn () {

                /* YOUR CODE GOES HERE */
		debug = true;
        }

        /**
         * Returns the tree's name
         * @return name of the tree
         */
        public String getName() {
                return treeName;
        }
	/**
	 * isEmpty
         * This function indicates if the binary tree is empty
	 *
	 * <return> true if the tree is empty false other wise
         */
	public boolean isEmpty () {
		
		/* YOUR CODE GOES HERE */
		if(occupancy > 0)
			return false;
		return true;
	}
	/**
	 * insert
         * Performs insertion into the binary tree
         *
         * @param   element        The element to insert.
         * @return  true or false indicating success or failure of insertion
         */
        public boolean insert (Whatever element) {

                /* YOUR CODE GOES HERE */
		/* local variable to hold a pointer to the TNode working on */
		TNode temp;
		/* if the tree is empty just insert the element */
		if(isEmpty())
		{
			/* set the root of the tree to the newly inserted
			 * TNode */
			root = new TNode(element);
			/* print debug message */
			if(debug)
				System.out.print(TREE + treeName + INSERT + 
						element.getName() + END);
			return true;
		}
		/* start working on root by assigning root to temp; */
		temp = root;
		/* while loop that do the insertion */
		while(true)
		{
			/* print debug message */
			if(debug)
				System.out.print(TREE + treeName + COMPARE + 
						element.getName() + AND + 
						temp.data.getName() + END);
			/* if the element is already on the tree, update it */
			if(element.equals(temp.data)) {
				temp.data = element;
				/* if the element is flagged as deleted, unflag
				 * it */
				if(temp.hasBeenDeleted)
					temp.hasBeenDeleted = false;
				break;
			}
			/* if element is greater than the working TNode
			 * work on the right of the current working TNode */
			if(element.isGreaterThan(temp.data)) {
				/* if right exists work on that TNode */
				if(temp.right != null)
					temp = temp.right;
				/* otherwise insert element at
				 * right */
				else
				{
					temp.right = new TNode(element);
					temp.right.parent = temp;
					break;
				}
			}
			/* other wise work on the left TNode */
			else
			{
				/* if left exists work on left */
				if(temp.left != null)
					temp = temp.left;
				/* otherwise insert element at left */
				else
				{
					temp.left = new TNode(element);
					temp.left.parent = temp;
					break;
				}
			}
		}
		/* print debug message */
		if(debug)
			System.out.print(TREE + treeName + INSERT + 
					element.getName() + END);
		/* update height and balance for every TNode checked */
		while(temp != root) {
			temp.updateHeight();
			temp.updateBalance();
			temp = temp.parent;
		}
		root.updateHeight();
		root.updateBalance();
		return true;
        }
	/**
	 * lookup
         * This function will lookup the element in the binary tree. If
               found a pointer to the element is returned.  If the element
               is not found, NULL will be returned to the user.
         *
         * @param   element  The element to insert or to lookup.
         * @return  the element if found, else NULL
         */
        public Whatever lookup (Whatever element) {

                /* YOUR CODE GOES HERE */
		/* local variable to hold a pointer to the TNode working on */
		TNode temp;
		/* return null if the tree is empty */
		if(isEmpty())
			return null;
		/* start working on root by assigning root to temp; */
		temp = root;
		/* a while loop to find the element on the tree */
		while(true)
		{
			/* print debug message */
			if(debug)
				System.out.print(TREE + treeName + COMPARE + 
						element.getName() + AND + 
						temp.data.getName() + END);
			/* if the element is fount */
			if(element.equals(temp.data)) {
				/* if the TNode wasn't deleted return it
				 * else return null */
				if(temp.hasBeenDeleted)
					break;
				return temp.data;
			}
			/* if the element is greater than temp, check the
			 * right of temp */
			if(element.isGreaterThan(temp.data)) {
			/* if right of temp exist assign right to temp */
				if(temp.right != null)
					temp = temp.right;
				/* else return null */
				else
				{
					break;
				}
			}
			/* else check the left of temp */
			else
			{
				/* if left of temp exist assign left to temp */
				if(temp.left != null)
					temp = temp.left;
				/* else return null */
				else
				{
					break;
				}
			}
		}
                return null;    // not there
        }
	/**
	 * remove
         * This function will remove the element from the binary tree.  If
               found a pointer to the element is returned.  If the element
               is not found, NULL will be returned to the user.
         *
         * @param   element  The element to remove.
         * @return  the element if found, else NULL
         */
        public Whatever remove (Whatever element) {

                /* YOUR CODE GOES HERE */
		/* local variable to hold a pointer to the TNode working on */
		TNode temp;
		/* return null if the tree is empty */
		if(isEmpty())
			return null;
		/* start working on root by assigning root to temp; */
		temp = root;
		/* a while loop to find the element on the tree */
		while(true)
		{
			/* print debug message */
			if(debug)
				System.out.print(TREE + treeName + COMPARE + 
						element.getName() + AND + 
						temp.data.getName() + END);
			/* if the element is found */
			if(element.equals(temp.data)) {
				/* if the element was already deleted ignore */
				if(temp.hasBeenDeleted)
					break;
				/*else assign its hasBeenDeleted flag to true*/
				temp.hasBeenDeleted = true;
				occupancy--;
				return temp.data;
			}
			/* if the element is greater than temp, check right of
			 * temp. if it exists assign temp to right else the
			 * element is not on the Tree */
			if(element.isGreaterThan(temp.data)) {
				if(temp.right != null)
					temp = temp.right;
				else
				{
					break;
				}
			}
			/* else check left of temp. if it exists assign temp to 
			 * left else the element is not on the Tree */
			else
			{
				if(temp.left != null)
					temp = temp.left;
				else
				{
					break;
				}
			}
		}
                return null;    // not there
        }

        /**
         * Creates a string representation of this tree. This method first
         * adds the general information of this tree, then calls the
         * recursive TNode function to add all nodes to the return string 
         *
         * @return  String representation of this tree 
         */
        public String toString () {
                String string = "Tree " + treeName + ":\noccupancy is ";
                string += occupancy + " elements.";

                if(root != null)
                        string += root.writeAllTNodes();

                return string;
        }

        private class TNode {

                public Whatever data;
                public TNode left, right, parent;
                public boolean hasBeenDeleted;

                /* left child's height - right child's height */
                public long balance;
                /* 1 + height of tallest child, or 0 for leaf */
                public long height;

		/**
        	* This function will allocate and initialize memory for a new
		* TNode object
       		*
       		* @param   element  The element to be stored in the TNode.
     	    	* @return  a pointer of the new TNode
         	*/
                public TNode (Whatever element) {

                        /* YOUR CODE GOES HERE */
			/* initialization */
			left = null;
			right = null;
			parent = null;
			data = element;
			balance = 0;
			height = 0;
			hasBeenDeleted = false;
			occupancy++;
                }

                /**
                 * Creates a string representation of this node. Information
                 * to be printed includes this node's height, its balance,
                 * and the data its storing.
                 *
                 * @return  String representation of this node 
                 */

                public String toString () {
                        return "at height:  " + height + "  with balance: " +
                                balance + "  " + data;
                }

                /**
                 * Writes all TNodes to the String representation field. 
                 * This recursive method performs an in-order
                 * traversal of the entire tree to print all nodes in
                 * sorted order, as determined by the keys stored in each
                 * node. To print itself, the current node will append to
                 * tree's String field.
                 */
                public String writeAllTNodes () {
                        String string = "";
                        if (left != null)
                                string += left.writeAllTNodes ();
                        if (!hasBeenDeleted) 
                                string += "\n" + this;          
                        if (right != null)
                                string += right.writeAllTNodes ();

                        return string;
                }

		/**
                 * updateBalance
		 *
		 * This function update the balance of a TNode
                 */
		private void updateBalance() {
			/* if the element doesn't exist set its height to -1 */
			long leftH = (left == null) ? -1 : left.height;
			long rightH = (right == null) ? -1 : right.height;
			/* calculate the new balance */
			balance = leftH - rightH;
		}
		/**
                 * updateHeight
		 *
		 * This function update the height of a TNode
                 */
		private void updateHeight() {
			long leftH = (left == null) ? 0 : left.height;
			long rightH = (right == null) ? 0 : right.height;
			/* calculate the new height */
			height = ((leftH > rightH) ? leftH : rightH) + 1;
		}
        }
}
