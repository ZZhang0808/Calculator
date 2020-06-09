/**************************************************************************** 
 
                                                                Zifan Zhang 
                                                         	CSE 12, Fall 19 
                                                                11/5/2019 
                                                             	cs12fa19gt 
                		Assignment SIX
 
File Name:  	Driver.c
Description:    This pogram implement UCSDStudent to store student numbers
		with the associated student name
****************************************************************************/
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <getopt.h>
#include "Base.h"
#include "SymTab.h"
#include <cstdlib>
using namespace std;

#ifdef NULL
#undef NULL
#define NULL 0
#endif
//=========================================================================
// class UCSDStudent : public Base
//
// Description: Implements the UCSDStudent. Each UCSDStudent has a name
// 		and a student number. 
//
// Data Fields:
//     name (char *)  - name of the UCSD student
//     studentnum (long)  - the student number of the UCSD student
//
// Public functions:
//     UCSDSudent - allocae memory and initialize the UCSDStudent
//     ~UCSDStudent - delete memory of the UCSDStudent
//     operator const char * - override char * casting of UCSDStudent
//     				return its name
//     operator long - override long casting of UCSDStudent return its hase
//     			code
//     operator == - override == operator return if two Bases's name are equal
//     operator < - override < operator return if one Base is less than another
//     
//==========================================================================
class UCSDStudent : public Base {
	char * name;
	long studentnum;
public:
/***************************************************************************
% Routine Name : UCSDStudent (char * nm, long sn)
% File :         Driver.c
% 
% Description :  This function allocate and initial memory for UCSDStudent
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% nm	          	name of the UCSDStudent
% sn			student number of the UCSDStudent
***************************************************************************/
	UCSDStudent (char * nm, long sn) :
		name (strdup (nm)), studentnum (sn) {}
/***************************************************************************
% Routine Name : ~UCSDStudent (void)
% File :         Driver.c
% 
% Description :  This function delete memory of UCSDStudent
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
***************************************************************************/
	~UCSDStudent (void) {
		/* YOUR CODE GOES HERE*/
		delete(name);
	}

	/* YOUR CODE GOES HERE */
/***************************************************************************
% Routine Name : operator const char* (void) const
% File :         Driver.c
% 
% Description :  This functin cast a UCSDStudent into a constant char* of its
			name
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
***************************************************************************/
	operator const char * (void) const {
		return name;
	}
/***************************************************************************
% Routine Name : operator long (void) const
% File :         Driver.c
% 
% Description :  This functin cast a UCSDStudent into a long integer of its
			hase code
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
***************************************************************************/
	operator long (void) const{
		/* stores the returning value */
		long sum = 0;
		/* loop through each char in name and add the ascii value to
		 * sum */
		for(long index = 0; name[index]; index ++)
			sum += name[index];
		return sum;
	}
/***************************************************************************
% Routine Name : operator == (const Base & base)const
% File :         Driver.c
% 
% Description :  This functin cast override == operator so it compares the
%			name of two UCSDStudents and return true if equal
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
***************************************************************************/
	long operator == (const Base & base)const {
		return !strcmp (name, base);
	}
/***************************************************************************
% Routine Name : operator < (const Base & base)const
% File :         Driver.c
% 
% Description :  This functin cast override < operator so it compares the
%			name of two UCSDStudents and return true if one is less
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
***************************************************************************/
	long operator < (const Base & base) const {
		return (strcmp (name, base) < 0) ? 1 : 0;
	}
	ostream & Write (ostream & stream) const {
		return stream << "name:  " << name
			<< "  Studentnum:  " << studentnum;
	}
};

/* You may have one or more functions defined here, too */



int main (int argc, char * const * argv) {
	SymTab ST (5);
	char buffer[128];
	char command;
	long number;
	char option;

	/* initialize debug variable */
	HashTable::Set_Debug(0);

	 /* check command line options for debug display */
	 while ((option = getopt (argc, argv, "x")) != EOF) {

		 switch (option) {
			 case 'x': HashTable::Set_Debug(1);
				 break;
		 	}
	} 

	ST.Write (cout << "Initial Symbol Table:\n");
	while (cin) {
		command = NULL;		// reset command each time in loop
		cout << "Please enter a command:  ((i)nsert, (l)ookup, (w)rite):  ";
		cin >> command;

		switch (command) {
		case 'i':
			cout << "Please enter UCSD student name to insert:  ";
			cin >> buffer;	// formatted input

			cout << "Please enter UCSD student number:  ";
			cin >> number;

			// create student and place in symbol table
			if(!ST.Insert (new UCSDStudent (buffer, number))){

				ST.Write (cout << "\nFinal Symbol Table:\n");
				exit (0);
			}
			break;

		case 'l': {
			const Base * found;	// whether found or not

			cout << "Please enter UCSD student name to lookup:  ";
			cin >> buffer;	// formatted input

			UCSDStudent stu (buffer, 0);
			found = ST.Lookup (& stu);
			
			if (found)
				found->Write (cout << "Student found!!!\n") << "\n";
			else
				cout << "Student " << buffer << " not there!\n";
			}
			break;

		case 'w':
			ST.Write (cout << "The Symbol Table contains:\n");
		}
	}
	ST.Write (cout << "\nFinal Symbol Table:\n");
}

