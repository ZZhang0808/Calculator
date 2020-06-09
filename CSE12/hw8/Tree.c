/**************************************************************************** 
 
                                                                Zifan Zhang 
                                                         	CSE 12, Fall 19 
                                                                11/26/2019 
                                                             	cs12fa19gt 
                		Assignment EIGHT
 
File Name:  	Tree.c
Description:    This pogram implement a binary tree to allow variable 
		assignment to the calculator. 
 ****************************************************************************/
#include <cstdlib>
#include <string>
#include "Tree.h"
using namespace std;
//=========================================================================
// class Tree : 
//
// Description: Implements the binary tree that stores objects and can look
// 		up for an object fast 
//
// Data Fields:
//     debug (int)  - debugging state
//     tree_count (long) - number of the trees exist
//     root (TNode *) - the TNode that doesn't have a parent
//     occupancy (long) - number of items stored
//
// Public functions:
//     set_Debug - turns on and off debugging for this Tree
//     Tree Constructor - Allocates and initializes the memory associated 
//     				with binary tree.
//     Tree Destructor - Called when de-allocating this Tree.
//     insert - Inserts the element in the binary tree. Returns true or false 
//     		indicating success of insertion.
//     lookup - Looks up the element in the binary tree. Returns pointer to the 
//     		data if found, null otherwise.
//     remove - remove the element from the binary tree. Returns pointer to the
//     		data of the element if deleted, null otherwise
//==========================================================================
// messages
static const char TAIL[] = "]\n";
static const char AND[] = " and ";
static const char CHECK[] = " - Checking ";
static const char COMPARE[] = " - Comparing ";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char INSERT[] = " - Inserting ";
static const char REPLACE[] = " - Replacing ";
static const char UPDATE[] = " - Updating ";

template <class Whatever>
int Tree<Whatever>::debug = 0;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define THRESHOLD 2

template <class Whatever>
ostream & operator << (ostream &, const TNode<Whatever> &);
//=========================================================================
// struct TNode
//
// Description: Implements the tree node that stores an element and can be
// 		stored on a binary tree
// Data Fields:
//     data (Whatever) - the data to store in the TNode
//     left (TNode *) - the left child TNode 
//     right (TNode *) - the right child TNode
//     occupancy (long) - keeps counts of TNodes on the Tree
//     balance (long) - left chile's height - right child's height
//     height (long) - 1 + height of tallest child, or 0 for leaf
//     tree_count (long) - keep the number of trees 
//
// Public functions:
//     TNode Constructor - Allocates and initializes the memory associated 
//     				with a TNode.
//     TNode Destructor - Called when de-allocating this TNode.
//     delete_AllTNode - delete all TNodes on the Tree
//     Insert - Inserts an element into the binary tree. Returns true or false 
//     		indicating success of insertion.
//     ReplaceAndRemoveMin - Called when removing a TNode with 2 children,
//     		replaces that TNode with the minimum TNode in its right subtree
//     		to maintain the Tree structure
//     Remove - Removes the matching data from the binary tree. Returns
//     		true or false indicating success of removal
//     SetHeightAndBalance - Updates the height and balance of the current TNode
//==========================================================================
template <class Whatever>
struct TNode {
        long balance;
        Whatever data;
        long height;
        TNode<Whatever> * left;
        long & occupancy;
        TNode<Whatever> * right;
        unsigned long & tree_count;
/***************************************************************************
% Routine Name : TNode (const Whatever & element, Tree<Whatever> & theTree)
% File :         Tree.c
% 
% Description :  This function allocate and initial memory for TNode
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% element		the data to be stored in the TNode
% theTree		a reference to the empty Tree where the root TNode is
%			about to be created
***************************************************************************/  
        TNode (const Whatever & element, Tree<Whatever> & theTree)
                : balance (0), data (element), height (0), left (0), 
                occupancy (theTree.occupancy), right (0), 
                tree_count (theTree.tree_count) {
                /* YOUR CODE GOES HERE */
			occupancy++;
        }
/***************************************************************************
% Routine Name : TNode (const Whatever & element, TNode<Whatever> & parentTNode)
% File :         Tree.c
% 
% Description :  This function allocate and initial memory for TNode
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% element		the data to be stored in the TNode
% parentTNode		a reference to the parent of the TNode about to be
%			created
***************************************************************************/ 
        TNode (const Whatever & element, TNode<Whatever> & parentTNode)
        : balance (0), data (element), height (0), left (0), 
                occupancy (parentTNode.occupancy), right (0), 
                tree_count (parentTNode.tree_count) {
                /* YOUR CODE GOES HERE */
			occupancy++;
        }
/***************************************************************************
% Routine Name : ~TNode (void)
% File :         Tree.c
% 
% Description :  This function delete memory of TNode
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
***************************************************************************/
        ~TNode (void) {
                /* YOUR CODE GOES HERE */
		occupancy--;
        }
/***************************************************************************
% Routine Name : delete_AllTNodes (void)
% File :         Tree.c
% 
% Description :  This recursive function delete memory of all TNodes in the tree
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
***************************************************************************/
        void delete_AllTNodes (void) {
                /* YOUR CODE GOES HERE */
		if(left)
			left->delete_AllTNodes();
		if(right)
			right->delete_AllTNodes();
		delete(this);
        }
/***************************************************************************
% Routine Name : Insert (const Whatever & element, TNode<Whatever> *& 
%			PointerInParent)
% File :         Tree.c
% 
% Description :  This function inserts an element into the binary tree. 
%		Returns true or false indicating success of insertion.
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% element		the data to be stored in the TNode
% parentTNode		a reference to the parent of the TNode about to be
%			inserted
***************************************************************************/
        unsigned long Insert (const Whatever & element, 
                              TNode<Whatever> *& PointerInParent);

        // OPTIONAL TNode :: Lookup => uncomment if implementing recursively
        // unsigned long Lookup(Whatever & element) const;

        void ReplaceAndRemoveMin (TNode<Whatever> & targetTNode, 
                TNode<Whatever> *& PointerInParent) {
                /* YOUR CODE GOES HERE */
		if(Tree<Whatever>::debug) 
			cerr << TREE << tree_count << CHECK << 
				(const char *)data << TAIL;
		if (left) {
			left -> ReplaceAndRemoveMin (targetTNode, left);
			SetHeightAndBalance(PointerInParent);
		}
		else {
			if(Tree<Whatever>::debug) 
				cerr << TREE << tree_count << REPLACE 
					<< (const char *)data << TAIL;
			targetTNode.data = this->data;
			if(right)
				PointerInParent = right;
			else
				PointerInParent = 0;
			delete(this);
		}
        }
/***************************************************************************
% Routine Name : Remove (TNode<Whatever> & elementTNode, TNode<Whatever> *& 
%			PointerInParent, long fromSHB = FALSE)
% File :         Tree.c
% 
% Description :  This function removes the matching data from the binary tree. 
%		Returns true or false indicating success of removal.
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% elementTNode		reference to the TNode containing data to remove
% parentTNode		a reference to the parent of the TNode about to be
%			inserted
***************************************************************************/
        unsigned long Remove (TNode<Whatever> & elementTNode, 
                TNode<Whatever> *& PointerInParent,
                long fromSHB = FALSE) {
                /* indicate if removal successed */
		long suc;
		// print debug message
		if(Tree<Whatever>::debug) 
			cerr << TREE << tree_count << COMPARE << (const char *)
				elementTNode.data << AND << (const char *)
				PointerInParent->data << TAIL;
		// if the matching TNode is found remove it
		if(elementTNode.data == data) {
			// carry out the data stored in the TNode to caller
			elementTNode.data = data;
			if(right){
				// if the TNode has 2 children call 
				// ReplaceAndRemoveMin
				if(left) {
					right->ReplaceAndRemoveMin
						(*this, right);
					if(!fromSHB)
						SetHeightAndBalance
							(PointerInParent);
					return 1;
				}
				else
					PointerInParent = right;
			}
			else
				PointerInParent = left;
			// if the TNode has only 1 child set the child to
			// PointerInParent, if it has no child, just delete
			// itself
			delete(this);
			return 1;
		}
		// if the working TNode's data is smaller than the target
		// TNode, search the TNode right of working TNode
		else if(data < elementTNode.data) {
			if(!right)
				return 0;
			suc = right->Remove(elementTNode, right, fromSHB);
		}
		// else search the left TNode
		else {
			if(!left)
				return 0;
			suc = left->Remove(elementTNode, left, fromSHB);
		}
		// update height and balance if the caller is not SHB
		if(!fromSHB)
			SetHeightAndBalance(PointerInParent);
		return suc;
        }
/***************************************************************************
% Routine Name : void SetHeightAndBalance (TNode<Whatever> *& PointerInParent)
% File :         Tree.c
% 
% Description :  Updates the height and balance of the current TNode
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% PointerInParent	a reference to TNode pointer in the parent TNode used
%			to get to the current TNode
***************************************************************************/
        void SetHeightAndBalance (TNode<Whatever> *& PointerInParent) {
                /* YOUR CODE GOES HERE */       
		// process the heights of children
		long lHeight = (left) ? left -> height : -1;
		long rHeight = (right) ? right -> height : -1;
		// update height and balance
		height = (lHeight > rHeight) ? lHeight + 1 : rHeight + 1;
		balance = lHeight - rHeight;
		// if balance is greater than THRESHOLD reinsert the current
		// TNode
		if (abs(balance) > THRESHOLD) {
			Whatever temp = this -> data;
			Remove (*this, PointerInParent, 1);
			PointerInParent -> Insert (temp, PointerInParent);
		}
        }

        ostream & Write_AllTNodes (ostream & stream) const {
                if (left)
                        left->Write_AllTNodes (stream);
                stream << *this;
                if (right)
                        right->Write_AllTNodes (stream);

                return stream;
        }
};

/* YOUR DEBUG SETTERS GO HERE */
/***************************************************************************
% Routine Name : void Tree<Whatever>::Set_Debug_Off()
% File :         Tree.c
% 
% Description :  This function sets debug flag to 0
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
***************************************************************************/
template <class Whatever>
void Tree<Whatever> :: Set_Debug_Off() {
	Tree<Whatever>::debug = 0;
}
/***************************************************************************
% Routine Name : void Tree<Whatever>::Set_Debug_On()
% File :         Tree.c
% 
% Description :  This function sets debug flag to 1
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
***************************************************************************/
template <class Whatever>
void Tree<Whatever> :: Set_Debug_On() {
	Tree<Whatever>::debug = 1;
}
/***************************************************************************
% Routine Name : Insert (const Whatever & element, TNode<Whatever> *& 
%			PointerInParent)
% File :         Tree.c
% 
% Description :  This function inserts an element into the binary tree. 
%		Returns true or false indicating success of insertion.
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% element		the data to be stored in the TNode
% parentTNode		a reference to the parent of the TNode about to be
%			inserted
***************************************************************************/
template <class Whatever>
unsigned long TNode<Whatever> :: Insert (const Whatever & element, 
                                         TNode<Whatever> *& PointerInParent) {
        /* indicate if removal successed */
	long suc;
	// print debug message
	if(Tree<Whatever>::debug) {
		cerr << TREE << tree_count << COMPARE << (const char *)element 
			<< AND << (const char *)PointerInParent->data << TAIL;
	}
	// if element already exists in the tree, update its data
	if(element == PointerInParent->data) {
		if(Tree<Whatever>::debug) {
			cerr << TREE << tree_count << INSERT << 
				(const char *)element << TAIL;
		}
		PointerInParent->data = element;
		return 1;
	}
	// if the element is greater than data of the current TNode
	// search right TNode and its descendants recursively
	// until an empty space is found
	else if(PointerInParent->data < element) {
		if(!right)
		{
			if(Tree<Whatever>::debug) 
				cerr << TREE << tree_count << INSERT << 
					(const char *)element << TAIL;
			right = new TNode<Whatever> (element, *this);
			suc = 1;
		}
		else
			suc = right->Insert(element, right);
	}
	// else search left TNode and its descendants recursively
	// for a space to insert
	else {
		if(!left)
		{
			if(Tree<Whatever>::debug) {
				cerr << TREE << tree_count << INSERT << 
					(const char *)element << TAIL;
			}
			left = new TNode<Whatever> (element, *this);
			suc = 1;
		}
		else 
			suc = left->Insert(element, left);
	}
	// print debug message
	if(Tree<Whatever>::debug)
		cerr << TREE << tree_count << UPDATE << 
			(const char *)data << TAIL;
	// when the TNode is inserted, update height and balance for all TNodes
	// searched
	this -> SetHeightAndBalance (PointerInParent);
	return suc;
}

template <class Whatever>
ostream & operator << (ostream & stream, const TNode<Whatever> & nnn) {
        stream << "at height:  :" << nnn.height << " with balance:  "
                << nnn.balance << "  ";
        return stream << nnn.data << "\n";
}
/***************************************************************************
% Routine Name : unsigned long Tree<Whatever>::Insert (const Whatever & element)
% File :         Tree.c
% 
% Description :  This function inserts an element into the binary tree. 
%		Returns true or false indicating success of insertion.
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% element		the data to be stored in the TNode
***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: Insert (const Whatever & element) {
        /* YOUR CODE GOES HERE */
	// if the tree is empty, insert a TNode at root
	if(!root) {
		if(Tree<Whatever>::debug) {
			cerr << TREE << tree_count << INSERT << 
				(const char *)element << TAIL;
		}
		root = new TNode<Whatever> (element, *this);
		return 1;
	}
	// else call TNode's Insert function
	return root->Insert(element, root);
}
/***************************************************************************
% Routine Name : unsigned long Tree<Whatever>::Lookup (Whatever & element) const
% File :         Tree.c
% 
% Description :  This function look up an element from the binary tree. 
%		Returns true or false indicating success of look up.
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% element		the data to be looked for in the TNode
***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: Lookup (Whatever & element) const {

        /* YOUR CODE GOES HERE */
	// return false if the tree is empty
	if(!root)
		return 0;
	// else set a working TNode equals to root
	TNode<Whatever> * working = root;
	// print debug message
	if(Tree<Whatever>::debug) 
		cerr << TREE << tree_count << COMPARE << (const char *)element 
			<< AND << (const char *)working->data << TAIL;
	// use a while loop to search down the tree for the element
	while(!(working->data == element)) {	
		// if data of working TNode is smaller than elment search right
		if(working->data < element) {
			if(working->right)
				working = working->right;
			else
				return 0;
		}
		// else search left
		else {
			if(working->left)
				working = working->left;
			else
				return 0;
		}
		// print debug message
		if(Tree<Whatever>::debug) 
			cerr << TREE << tree_count << COMPARE << 
				(const char *)element 
				<< AND << (const char *)working->data << TAIL;
	}
	// if the element is found, send out its data and return true
	element = working->data;
	return 1;
}

/***************************************************************************
% Routine Name : unsigned long Tree<Whatever>::Remove (Whatever & element)
% File :         Tree.c
% 
% Description :  This function removes an element from the binary tree. 
%		Returns true or false indicating success of removal.
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% element		the data to be removed in the TNode
***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: Remove (Whatever & element) {
        /* YOUR CODE GOES HERE */
	// stores the success of removal
	long suc;
	// if the tree is empty return false
	if(!root)
		return 0;
	// else set working TNode to the current TNode and call TNode's 
	// Remove method
	TNode<Whatever> elementTNode = TNode<Whatever>(element, *this);
	suc = root->Remove(elementTNode, root, 0);
	element = elementTNode.data;
	return suc;
}

template <class Whatever>
Tree<Whatever> :: Tree (void): occupancy (0), root (NULL)
/***************************************************************************
% Routine Name : Tree<Whatever> :: Tree  (public)
% File :         Tree.c
%
% Description :  Guarantee initialization of occupancy and root. It also
%                initializes the tree_count using a static counter.
***************************************************************************/

{

        static long counter;
        tree_count = ++counter;

        if (debug) {
		cerr << TREE << tree_count << ALLOCATE;
	}
}

template <class Whatever>
Tree<Whatever> :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree<Whatever> :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/
{
        /* YOUR CODE GOES HERE */
	if(root) {
		root->delete_AllTNodes();
		root = 0;
	}
	tree_count--;
	if(debug) {
		cerr << TREE << tree_count << DEALLOCATE;
	}
}

template <class Whatever>
ostream & Tree<Whatever> :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree<Whatever> :: Write (public)
% File :         Tree.c
% 
% Description : This function will output the contents of the Tree table
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
        stream << "Tree " << tree_count << ":\n"
                << "occupancy is " << occupancy << " elements.\n";

        return (root) ? root->Write_AllTNodes (stream) : stream;
}
