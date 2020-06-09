/**************************************************************************** 
 
                                                                Zifan Zhang 
                                                         	CSE 12, Fall 19 
                                                                12/6/2019 
                                                             	cs12fa19gt 
                		Assignment EIGHT
 
File Name:  	Tree.c
Description:    This pogram implement a binary tree to allow variable 
		assignment to the calculator. 
 ****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "Tree.h"
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
// debug messages
static const char ALLOCATE[] = " - Allocating]\n";
static const char COST_READ[] = "[Cost Increment (Disk Access): Reading ";
static const char COST_WRITE[] = "[Cost Increment (Disk Access): Writing ";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char TREE[] = "[Tree ";
static const char TAIL[] = "]\n";

template <class Whatever>
int Tree<Whatever>::debug_on = 0;

template <class Whatever>
long Tree<Whatever>::cost = 0;

template <class Whatever>
long Tree<Whatever>::operation = 0;

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
//     left (offset) - the left child TNode 
//     right (offset) - the right child TNode
//     balance (long) - left chile's height - right child's height
//     height (long) - 1 + height of tallest child, or 0 for leaf
//     this_position (offset) - current position
//
// Public functions:
//     TNode Constructor - Allocates and initializes the memory associated 
//     				with a TNode.
//     TNode Destructor - Called when de-allocating this TNode.
//     Insert - Inserts an element into the binary tree. Returns true or false 
//     		indicating success of insertion.
//     ReplaceAndRemoveMin - Called when removing a TNode with 2 children,
//     		replaces that TNode with the minimum TNode in its right subtree
//     		to maintain the Tree structure
//     Remove - Removes the matching data from the binary tree. Returns
//     		true or false indicating success of removal
//     SetHeightAndBalance - Updates the height and balance of the current TNode
//     Write - updata node to disk
//     Read - read node from disk
//==========================================================================
template <class Whatever>
struct  TNode {
// friends:

// data fields:
	Whatever data;
	long height;
	long balance;
	offset left;
	offset right;
	offset this_position;	// current position

// function fields:
	TNode () : height (0), balance (0), left (0), right (0), 
		this_position (0) {}

	// to declare the working TNode in Tree's Remove
	TNode (Whatever & element) : data (element), height (0), balance (0),
		left (0), right (0), this_position (0) {}
	
	TNode (Whatever &, fstream *, long &);	// to add new node to disk
	TNode (const offset &, fstream *);	// to read node from disk
	
	unsigned long Insert (Whatever &, fstream *, long &, offset &);
	// optional recursive Lookup declaration would go here
	void Read (const offset &, fstream *);	// read node from disk
	unsigned long Remove (TNode<Whatever> &, fstream *, long &, offset &,
		long fromSHB = FALSE);
	void ReplaceAndRemoveMin (TNode<Whatever> &, fstream *, offset &);
	void SetHeightAndBalance (fstream *, offset &);
	void Write (fstream *) const;		// update node to disk

	ostream & Write_AllTNodes (ostream &, fstream *) const;
};
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
	Tree<Whatever>::debug_on = 0;
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
	Tree<Whatever>::debug_on = 1;
}
/***************************************************************************
% Routine Name : unsigned long Tree<Whatever>::Insert (Whatever & element)
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
unsigned long Tree<Whatever> :: Insert (Whatever & element) {
	/* YOUR CODE GOES HERE */
	// if the tree is empty, insert a TNode at root
	IncrementOperation();
	if(occupancy==0) {
		TNode<Whatever> (element, fio, occupancy);
		return 1;
	}
	// else call TNode's Insert function
	else {
		TNode<Whatever> readRootNode (root, fio);
		return readRootNode.Insert (element, fio, occupancy, root);
	}
}
/***************************************************************************
% Routine Name : void TNode<Whatever>::ReplaceAndRemoveMin (TNode<Whatever & 
%			targetTNode, fstream * fio, offset & PositionInParent)
% File :         Tree.c
% 
% Description :  Called when removing a TNode with 2 children, replaces that 
%			TNode with the minimum TNode in its right subtree to 
%			maintain the Tree structure. 
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% targetTNode		a reference to the TNode to remove that has 2 children
% fio			the filestream corresponding to the datafile where
%			the Tree is stored on disk
% PositionInParent	a reference to the TNode position in the parent TNode
%			used to get to the current TNOde's offset in the data
***************************************************************************/
template <class Whatever>
void TNode<Whatever> :: ReplaceAndRemoveMin (TNode<Whatever> & targetTNode, 
	fstream * fio, offset & PositionInParent) {
	/* YOUR CODE GOES HERE */
	if (left) { // if the node has a left child keep searching left
		TNode<Whatever> lChild(left, fio);
		lChild.ReplaceAndRemoveMin (targetTNode, fio, left);
		SetHeightAndBalance(fio, PositionInParent);
	}
	else { // replace targetTNode's data with this data and remove this
		// node from the tree
		targetTNode.data = data;
		if(right)
			PositionInParent = right;
		else
			PositionInParent = 0;
	}
}
/***************************************************************************
% Routine Name : unsigned long TNode<Whatever>::Remove (TNode<Whatever> & 
%			elementTNode, fstream * fio, long & occupancy,
%			offset & PositionInParent, long fromSHB = FALSE)
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
% fio			the filestream corresponding to the datafile where
%			the Tree is stored on disk
% occupancy		a reference to the occupancy of the tree
% PositionInParent	a reference to the TNode position in the parent TNode
%			used to get to the current TNOde's offset in the data
% fromSHB		keep track of whether or not Remove was called from SHB
***************************************************************************/
template <class Whatever>
unsigned long TNode<Whatever> :: Remove (TNode<Whatever> & elementTNode,
	fstream * fio, long & occupancy, offset & PositionInParent,
	long fromSHB) {
	/* YOUR CODE GOES HERE */
        /* indicate if removal successed */
	long suc;
	// if the matching TNode is found remove it
	if(elementTNode.data == data) {
		// carry out the data stored in the TNode to caller
		elementTNode.data = data;
		if(right){
			// if the TNode has 2 children call 
			// ReplaceAndRemoveMin
			if(left) {
				TNode<Whatever> rChild (right, fio);
				rChild.ReplaceAndRemoveMin
					(*this, fio, right);
			}
			else { // remove this node from tree give its only
			       // only child to its parent
				PositionInParent = right;
				occupancy--;
				return 1;
			}
		}
		else { // remove this node from tree and give its only child
		       // to its parent
			PositionInParent = left;
			occupancy--;
			return 1;
		}
		// if the TNode has only 1 child set the child to
		// PointerInParent, if it has no child, just delete
		// itself
		occupancy--;
		suc = 1;
	}
	// if the working TNode's data is smaller than the target
	// TNode, search the TNode right of working TNode
	else if(data < elementTNode.data) {
		if(!right)
			return 0;
		TNode<Whatever> rChild(right, fio);
		suc = rChild.Remove(elementTNode, fio, occupancy, right, 0);
	}
	// else search the left TNode
	else {
		if(!left)
			return 0;
		TNode<Whatever> lChild(left, fio);
		suc = lChild.Remove(elementTNode, fio, occupancy, left, 0);
	}
	// update height and balance if the caller is not SHB
	if(!fromSHB)
		SetHeightAndBalance(fio, PositionInParent);
	else {
		fio->seekp(this_position, ios::beg);
		Write(fio);
	}
	return suc;
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
	IncrementOperation(); // increment operation
	// stores the success of removal
	long suc;
	// if the tree is empty return false
	if(!root)
		return 0;
	// else set working TNode to the current TNode and call TNode's 
	// Remove method
	TNode<Whatever> elementTNode(element); // a temp node contains element
	TNode<Whatever> rootTNode(root, fio); // a reference of root node
	suc = rootTNode.Remove(elementTNode, fio, occupancy, root, 0);
	element = elementTNode.data;
	// if the tree is empty, set root to the end of file
	if (!root)
		ResetRoot();
	return suc;
}
/***************************************************************************
% Routine Name : void TNode<Whateer>::SetHeightAndBalance (fstream * fio, 
%			offset & PositionInParent)
% File :         Tree.c
% 
% Description :  Updates the height and balance of the current TNode
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% fio			the filestream corresponding to the datafile where
%			the Tree is stored on disk
% PositionInParent	a reference to TNode position in the parent TNode used
%			to get to the current TNode's offset in datafile
***************************************************************************/
template <class Whatever>
void TNode<Whatever> :: SetHeightAndBalance (fstream * fio,
	offset & PositionInParent) {
	/* YOUR CODE GOES HERE */ 
	// process the heights of children
	long lHeight = (left) ? TNode<Whatever> (left, fio).height : -1;
	long rHeight = (right) ? TNode<Whatever> (right, fio).height : -1;
	// update height and balance
	height = (lHeight > rHeight) ? lHeight + 1 : rHeight + 1;
	balance = lHeight - rHeight;
	// if balance is greater than THRESHOLD reinsert the current TNode
	if (abs(balance) > THRESHOLD) {
		long fakeOccupancy;
		TNode<Whatever> elementTNode(data);
		Remove (elementTNode, fio, fakeOccupancy, PositionInParent, 1);
		TNode<Whatever> newParentNode(PositionInParent, fio);
		newParentNode.Insert (elementTNode.data, fio, fakeOccupancy, 
				PositionInParent);
	}
	else {	
		fio->seekp(this_position, ios::beg);
		Write(fio);
	}
}
/***************************************************************************
% Routine Name : long Tree<Whatever> :: GetCost()
% File :         Tree.c
% 
% Description :  Returns the value of the Tree<Whatever>::cost variable. 
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
***************************************************************************/
template <class Whatever>
long Tree <Whatever> :: GetCost () {
	/* YOUR CODE GOES HERE */
	return cost;
}
/***************************************************************************
% Routine Name : long Tree <Whatever> :: GetOperation()
% File :         Tree.c
% 
% Description :  Returns the value of the Tree<Whatever>::operation variable.
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
***************************************************************************/
template <class Whatever>
long Tree <Whatever> :: GetOperation () {
	/* YOUR CODE GOES HERE */
	return operation;
}
/***************************************************************************
% Routine Name : void Tree <Whatever> :: IncrementCost()
% File :         Tree.c
% 
% Description :  Incrementss the value of the Tree<Whatever>::cost variable.
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
***************************************************************************/
template <class Whatever>
void Tree <Whatever> :: IncrementCost () {
	/* YOUR CODE GOES HERE */
	cost ++;
}
/***************************************************************************
% Routine Name : void Tree<Whatever> :: IncrementOperation()
% File :         Tree.c
% 
% Description :  Increment the value of the Tree<Whatever>::operation variable
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
***************************************************************************/
template <class Whatever>
void Tree <Whatever> :: IncrementOperation () {
	/* YOUR CODE GOES HERE */
	operation++;
}
/***************************************************************************
% Routine Name : void Tree <Whatever> :: ResetRoot()
% File :         Tree.c
% 
% Description :  This recursive function delete memory of all TNodes in the tree
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
***************************************************************************/
template <class Whatever>
void Tree <Whatever> :: ResetRoot () {
        /* YOUR CODE GOES HERE */       
	fio->seekg(0, ios::end);
	// set root to the end of the file
	root = fio->tellg();
	fio->seekp(0, ios::beg);
	// update root to the disk
	fio->write((char *)&root, sizeof(root));

}
/***************************************************************************
% Routine Name : unsigned long TNode<Whatever>::Insert (Whatever & element,  
%			fstream * fio, long & occupancy, offset & 
%			PositionInParent)
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
% fio			the filestream corresponding to the datafile where
%			the Tree is stored on disk
% PositionInParent	a reference to TNode position in the parent TNode used
%			to get to the current TNode's offset in datafile
% occupancy		a reference to the occupancy of the tree
***************************************************************************/
template <class Whatever>
unsigned long TNode<Whatever> :: Insert (Whatever & element, fstream * fio,
	long & occupancy, offset & PositionInParent) {
	/* YOUR CODE GOES HERE */
        /* indicate if removal successed */
	long suc;
	// if element already exists in the tree, update its data
	if(element == data) {
		data = element;
		fio->seekp(this_position, ios::beg);
		Write(fio);
		return 1;
	}
	// if the element is greater than data of the current TNode
	// search right TNode and its descendants recursively
	// until an empty space is found
	else if(data < element) {
		if(!right)
		{
			TNode<Whatever> rChild(element, fio, occupancy);
			right = rChild.this_position;
			suc = 1;
		}
		else {
			TNode<Whatever> rChild(right, fio);
			suc = rChild.Insert(element, fio, occupancy, right);
		}
	}
	// else search left TNode and its descendants recursively
	// for a space to insert
	else {
		if(!left)
		{
			TNode<Whatever> lChild(element, fio, occupancy);
			left = lChild.this_position;
			suc = 1;
		}
		else {
			TNode<Whatever> lChild(left, fio);
			suc = lChild.Insert(element, fio, occupancy, left);
		}
	}
	// when the TNode is inserted, update height and balance for all TNodes
	// searched
	SetHeightAndBalance (fio, PositionInParent);
	return suc;
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
	IncrementOperation();
	// return false if the tree is empty
	if(!root)
		return 0;
	// else set a working TNode equals to root
	TNode<Whatever> working(root, fio);
	// use a while loop to search down the tree for the element
	while(!(working.data == element)) {	
		// if data of working TNode is smaller than elment search right
		if(working.data < element) {
			if(working.right)
				working = TNode<Whatever> (working.right, fio);
			else
				return 0;
		}
		// else search left
		else {
			if(working.left)
				working = TNode<Whatever> (working.left, fio);
			else
				return 0;
		}
	}
	// if the element is found, send out its data and return true
	element = working.data;
	return 1;
}
/***************************************************************************
% Routine Name : void TNode<Whatever>::Read(const offset & position, 
%								fstream * fio)
% File :         Tree.c
% 
% Description :  Read a TNode which is present on the datafile into memory
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% position		the position of the node looking for on disk
% fio			a reference to the empty Tree where the root TNode is
%			about to be created
***************************************************************************/
template <class Whatever>
void TNode<Whatever> :: Read (const offset & position, fstream * fio) {
	/* YOUR CODE GOES HERE */
	// read each datafield of TNode from fio
	fio->seekg(position, ios::beg);
	fio->read((char *)(&data), sizeof(data));
	fio->read((char *)(&height), sizeof(height));
	fio->read((char *)(&balance), sizeof(balance));
	fio->read((char *)(&left), sizeof(left));
	fio->read((char *)(&right), sizeof(right));
	fio->read((char *)(&this_position), sizeof(this+position));
	// increment cost
	Tree<Whatever>::IncrementCost();
	if(Tree<Whatever>::debug_on) // print debug message
		cerr << COST_READ << (const char *)data << TAIL;
}
/***************************************************************************
% Routine Name : TNode (const offset & position, fstream * fio)
% File :         Tree.c
% 
% Description :  Read a TNode from the disk
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% position		the position of the node looking for on disk
% fio			a reference to the empty Tree where the root TNode is
%			about to be created
***************************************************************************/
template <class Whatever>
TNode<Whatever> :: TNode (const offset & position, fstream * fio) {
	/* call Read to read from fio */
	Read(position, fio);
}
/***************************************************************************
% Routine Name : TNode (Whatever & element, fstream * fio, long & occupancy)
% File :         Tree.c
% 
% Description :  Write a new TNode to disk
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% element		the data to be stored in the TNode
% fio			a reference to the empty Tree where the root TNode is
%			about to be created
% occupancy		a reference to the occupancy of the tree
***************************************************************************/
template <class Whatever>
TNode<Whatever> :: TNode (Whatever & element, fstream * fio, long & occupancy): 
			data (element), height (0), balance (0), left (0), 
			right (0) {
	/* YOUR CODE GOES HERE */
	data = element;
	fio->seekp(0, ios::end); // get this_position
	this_position = fio->tellp();
	Write(fio); // write the new TNode to disk
	occupancy++;
}
/***************************************************************************
% Routine Name : void TNode<Whatever>::Write(fstream * fio)
% File :         Tree.c
% 
% Description :  Read a TNode which is present on the datafile into memory
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% fio			a reference to the empty Tree where the root TNode is
%			about to be created
***************************************************************************/
template <class Whatever>
void TNode<Whatever> :: Write (fstream * fio) const {
	/* YOUR CODE GOES HERE */
	// write each datafield of TNode to the disk
	fio->write((const char *)&data, sizeof(data));
	fio->write((const char *)&height, sizeof(height));
	fio->write((const char *)&balance, sizeof(balance));
	fio->write((const char *)&left, sizeof(left));
	fio->write((const char *)&right, sizeof(right));
	fio->write((const char *)&this_position, sizeof(this_position));
	Tree<Whatever>::IncrementCost(); // increment cost
	if(Tree<Whatever>::debug_on) // print debug message
		cerr << COST_WRITE << (const char *)data << TAIL;
}
/***************************************************************************
% Routine Name : Tree<Whatever> :: Tree  (public)
% File :         Tree.c
%
% Description :  Guarantee initialization of occupancy and root. It also
%                initializes the tree_count using a static counter.
***************************************************************************/
template <class Whatever>
Tree<Whatever> :: Tree (const char * datafile) :
	fio (new fstream (datafile, ios :: out | ios :: in)), occupancy(0), 
	root(0) {
	/* check for empty file */
	fio->seekg(0, ios::beg);
	offset begin = fio->tellg();
	fio->seekg(0, ios::end);
	offset ending = fio->tellg();
	if(begin == ending) { // empty file
		//reserve space for root and occupancy
		fio->seekp(0, ios::beg);
		fio->write((const char *)&root, sizeof(root));
		fio->write((const char *)&occupancy, sizeof(occupancy));
		root = fio->tellp();
		occupancy = 0;
	}
	else { // file has contents, read root and occupancy
		fio->seekg(0, ios::beg);
		fio->read((char *)(&root), sizeof(root));
		fio->read((char *)(&occupancy), sizeof(occupancy));
	}
	operation = 0;
	static long count = 0;
	tree_count = ++count;
	if(debug_on)
		cerr << TREE << tree_count << ALLOCATE;
}

template <class Whatever>
Tree<Whatever> :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
	/* YOUR CODE GOES HERE */
	fio->seekp(0, ios::beg); // updates root and occupancy to the disk
	fio->write((const char *)&root, sizeof(root));
	fio->write((const char *)&occupancy, sizeof(occupancy));
	if(debug_on) // print debug message
		cerr << TREE << tree_count << DEALLOCATE;
	delete(fio); // deallocate fio
}	/* end: ~Tree */

template <class Whatever>
ostream & operator << (ostream & stream, const TNode<Whatever> & nnn) {
	stream << "at height:  :" << nnn.height << " with balance:  "
		<< nnn.balance << "  ";
	return stream << nnn.data << "\n";
}

template <class Whatever>
ostream & Tree<Whatever> :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree :: Write (public)
% File :         Tree.c
% 
% Description : This funtion will output the contents of the Tree table
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
        long old_cost = cost;

	stream << "Tree " << tree_count << ":\n"
		<< "occupancy is " << occupancy << " elements.\n";

	fio->seekg (0, ios :: end);
	offset end = fio->tellg ();

	// check for new file
	if (root != end) {
		TNode<Whatever> readRootNode (root, fio);
		readRootNode.Write_AllTNodes (stream, fio);
	}

        // ignore cost when displaying nodes to users
        cost = old_cost;

	return stream;
}

template <class Whatever>
ostream & TNode<Whatever> ::
Write_AllTNodes (ostream & stream, fstream * fio) const {
	if (left) {
		TNode<Whatever> readLeftNode (left, fio);
		readLeftNode.Write_AllTNodes (stream, fio);
	}
	stream << *this;
	if (right) {
		TNode<Whatever> readRightNode (right, fio);
		readRightNode.Write_AllTNodes (stream, fio);
	}

	return stream;
}

