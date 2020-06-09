/**************************************************************************** 
 
                                                                Zifan Zhang 
                                                         	CSE 12, Fall 19 
                                                                11/12/2019 
                                                             	cs12fa19gt 
                		Assignment SEVEN
 
File Name:  	Tree.c
Description:    This pogram implement a binary tree to allow variable 
		assignment to the calculator. 
 ****************************************************************************/ 
//=========================================================================
// class Tree : public Base
//
// Description: Implements the binary tree that stores objects and can look
// 		up for an object fast 
//
// Data Fields:
//     debug_on (boolean)  - debugging state
//     tree_name (char *) - name of the tree
//     root (TNode *) - the TNode that doesn't have a parent
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
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string.h>
#include "Tree.h"
using namespace std;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

static const char ALLOCATE[] = " - Allocating]\n";
static const char DEALLOCATE[] = " has been deallocated]\n";
static const char AND[] = " and ";
static const char COMPARE[] = " - Comparing ";
static const char INSERT[] = " - Inserting ";
static const char TREE[] = "[Tree ";
static const char END[] = "]\n";
//=========================================================================
// struct TNode
//
// Description: Implements the tree node that stores an element and can be
// 		stored on a binary tree
// Data Fields:
//     data (Base *) - the data to store in the TNode
//     left (TNode *) - the left child TNode 
//     right (TNode *) - the right child TNode
//     parent (TNode *) - the parent TNode
//     occupancy (long) - keeps counts of TNodes on the Tree
//     hasBeenDeleted (long) - indicates if the TNode has been deleted
//     balance (long) - left chile's height - right child's height
//     height (long) - 1 + height of tallest child, or 0 for leaf
//
// Public functions:
//     TNode Constructor - Allocates and initializes the memory associated 
//     				with a TNode.
//     TNode Destructor - Called when de-allocating this TNode.
//     delete_AllTNode - delete all TNodes on the Tree
//     updateBalance - updates the balance for a TNode
//     updateHeight - updates the height for a TNode
//==========================================================================
struct TNode {
	Base * data;
	TNode * left, * right, *parent;
	static long occupancy;
        unsigned long hasBeenDeleted;

	// left child's height - right child's height
	long balance;

	// 1 + height of tallest child, or 0 for leaf
	long height;
/***************************************************************************
% Routine Name : TNode (Base * element)
% File :         Tree.c
% 
% Description :  This function allocate and initial memory for TNode
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% element		the data to be stored in the TNode
***************************************************************************/
	TNode (Base * element) : data (element), left (0), right (0),
	        parent (0), hasBeenDeleted (FALSE), balance (0), height(0) {
		// your code goes here
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
		// your code goes here
		delete (data);
	}
        ostream & Write (ostream & stream) const {
                stream << "at height:  " << height << "  with balance:  "
			<< balance << "  ";
		return data->Write (stream) << "\n";
        }
	ostream & Write_AllTNodes (ostream & stream) const {
		if (left)
			left->Write_AllTNodes (stream);
		if (!hasBeenDeleted)
			Write (stream);
		if (right)
			right->Write_AllTNodes (stream);

		return stream;
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
		// your code goes here 
		// do nothing if height of the TNode is 0
		if(height > 0)
		{
			//if left TNode exist delete all TNodes on the left
			if(left)
			{
				left->delete_AllTNodes();
				delete left;
				left = 0;
			}
			//if right TNode exist delete all TNodes on the right
			if(right)
			{
				right->delete_AllTNodes();
				delete right;
				right = 0;
			}
		}

	}
/***************************************************************************
% Routine Name : updateBalance (void)
% File :         Tree.c
% 
% Description :  This function updates balance of a TNode
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
***************************************************************************/
	void updateBalance(void) {
		/* if the element doesn't exist set its height to -1 */
		long leftH = (!left) ? -1 : left->height;
		long rightH = (!right) ? -1 : right->height;
		/* calculate the new balance */
		balance = leftH - rightH;
	}
/***************************************************************************
% Routine Name : updateHeight (void)
% File :         Tree.c
% 
% Description :  This function updates height of a TNode
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
***************************************************************************/
	void updateHeight(void) {
		long leftH = (!left) ? 0 : left->height;
		long rightH = (!right) ? 0 : right->height;
		/* calculate the new height */
		height = ((leftH > rightH) ? leftH : rightH) + 1;
	}
};

// initialization of static data fields of Tree
long TNode :: occupancy = 0;
bool Tree :: debug_on = 0;
/***************************************************************************
% Routine Name : Tree :: Set_Debug (bool option)
% File :         Tree.c
% 
% Description :  This function sets the debug state of the tree
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% option		the state of debug
***************************************************************************/
void Tree :: Set_Debug (bool option) {

        // your code goes here
	debug_on = option;
}

Tree :: Tree (const char * name) : root (0), tree_name (strdup (name))
/***************************************************************************
% Routine Name : Tree :: Tree  (public)
% File :         Tree.c
% 
% Description :  Initializes root pointer to NULL, and occupancy to 0.
***************************************************************************/

{
	// your code goes here
	if(debug_on)
		cerr << TREE << name << ALLOCATE;
	TNode :: occupancy = 0;
}

Tree :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
	// your code goes here
	// if root exist call delete_AllTNodes to delete all TNodes
	if(root)
		root->delete_AllTNodes();
	delete(root);
	// print debug message
	if(debug_on)
		cerr << TREE << tree_name << DEALLOCATE;
	// deallocates tree_name
	free((char *)tree_name);
}	/* end: ~Tree */
/***************************************************************************
% Routine Name : Tree :: IsEmpty()  (public)
% File :         Tree.c
% 
% Description :  return true if the tree is empty, false otherwise
***************************************************************************/
unsigned long Tree :: IsEmpty () const {
	
	//your code goes here
	return TNode :: occupancy == 0;
}
/***************************************************************************
% Routine Name : Tree :: Insert (Base * element)
% File :         Tree.c
% 
% Description :  This function inserts element onto the Tree. Return true if
% 			successfully inserted
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% element		the element to be inserted
***************************************************************************/
unsigned long Tree :: Insert (Base * element) {

	// your code goes here
	// local variable to hold a pointer to the TNode working on
	TNode * temp;
	// if the tree is empty just insert the element
	if(IsEmpty())
	{
		// set the root of the tree to the newly inserted TNode
		root = new TNode(element);
		// print debug message
		if(debug_on)
			cerr << TREE << tree_name << INSERT << 
					*element << END;
		return true;
	}
	// start working on root by assigning root to temp
	temp = root;
	// while loop that do the insertion
	while(true)
	{
		//print debug message
		if(debug_on)
			cerr << TREE << tree_name << COMPARE << 
					*element << AND << 
					*temp->data << END;
		// if the element is already on the tree, update it
		if(*element == *temp->data) {
			temp->data = element;
			// if the element is flagged as deleted, unflag it
			if(temp->hasBeenDeleted)
				temp->hasBeenDeleted = 0;
			break;
		}
		// if element is greater than the working TNode, work on
		// the right of the working TNode
		if(*element > *temp->data) {
			// if right exists work on that TNode
			if(temp->right)
				temp = temp->right;
			// otherwise insert element at right
			else
			{
				temp->right = new TNode(element);
				temp->right->parent = temp;
				break;
			}
		}
		// otherwise work on the left TNode
		else
		{
			// if left exists work on left
			if(temp->left)
				temp = temp->left;
			// otherwise insert element at left
			else
			{
				temp->left = new TNode(element);
				temp->left->parent = temp;
				break;
			}
		}
	}
	// print debug message
	if(debug_on)
		cerr << TREE << tree_name << INSERT << *element << END;
	// update height and balance for every TNode checked
	while(temp != root) {
		temp->updateHeight();
		temp->updateBalance();
		temp = temp->parent;
	}
	root->updateHeight();
	root->updateBalance();
	return true;
}
/***************************************************************************
% Routine Name : const Base * Tree :: Lookup (const Base * element) const
% File :         Tree.c
% 
% Description :  This function looks up an element from the Tree. Return the 
%			element if found, otherwise return null
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% element		the element to be looked for
***************************************************************************/
const Base * Tree :: Lookup (const Base * element) const {

	// your code goes here
	// local variable to hold a pointer to the TNode working on
	TNode * temp;
	// return null if the tree is empty
	if(IsEmpty())
		return 0;
	// start working on root by assigning root to temp
	temp = root;
	// a while loop to find the element on the tree
	while(1)
	{
		// print debug message
		if(debug_on)
			cerr << TREE << tree_name << COMPARE << 
				*element << AND << *temp->data << END;
		// if the element is found
		if(*element == *temp->data) {
			// if the element was deleted, return null
			if(temp->hasBeenDeleted)
				break;
			// otherwise return the element
			return temp->data;
		}
		// if the element is greater than temp, check the right
		if(*element > *temp->data) {
			// if right exists assign right to temp
			if(temp->right)
				temp = temp->right;
			// else the element is not on the Tree, return null
			else
			{
				break;
			}
		}
		// else check the left
		else
		{
			// if left of temo exist assign left to temp
			if(temp->left)
				temp = temp->left;
			// else the element is not on the Tree, return null
			else
			{
				break;
			}
		}
	}
	return 0;
}
/***************************************************************************
% Routine Name : Tree :: Remoe (Base * element)
% File :         Tree.c
% 
% Description :  This function removes an element from the Tree. Return its
% 			data if it is on the Tree, else return null
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% element		the element to be removed
***************************************************************************/
Base * Tree :: Remove (const Base * element) {
        // your code goes here
	// local variable to hold a pointer to the TNode working on
	TNode * temp;
	// return null if the tree is empty
	if(IsEmpty())
		return 0;
	// start working on root by assigning root to temp
	temp = root;
	// a while loop to find the element on the tree
	while(1)
	{
		// print debug message
		if(debug_on)
			cerr << TREE << tree_name << COMPARE << 
				*element << AND << *temp->data << END;
		// if the element is found
		if(*element == *temp->data) {
			// if the element was already deleted ignore it
			if(temp->hasBeenDeleted)
				break;
			// else assign its hasBeenDeleted flag to true
			temp->hasBeenDeleted = 1;
			TNode :: occupancy--;
			return temp->data;
		}
		// if the element is greater than temp, check right of temp.
		// if it exists, assign temp to right, else the element is
		// not on the Tree
		if(*element > *temp->data) {
			if(temp->right)
				temp = temp->right;
			else
			{
				break;
			}
		}
		// else check left of temp. If it exists assign temp to left,
		// else the element is not on the Tree
		else
		{
			if(temp->left)
				temp = temp->left;
			else
			{
				break;
			}
		}
	}
	return 0;
}

ostream & Tree :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree :: Write (public)
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
        stream << "Tree " << tree_name << ":\n"
		<< "occupancy is " << TNode :: occupancy << " elements.\n";

	return (root) ? root->Write_AllTNodes (stream) : stream;
}       /* end: Write */

