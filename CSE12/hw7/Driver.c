/**************************************************************************** 
 
                                                                Zifan Zhang 
                                                         	CSE 12, Fall 19 
                                                                11/12/2019 
                                                             	cs12fa19gt 
                		Assignment SEVEN
 
File Name:  	Driver.c
Description:    This pogram implement UCSDStudent to store student numbers
		with the associated student name
****************************************************************************/
#include <cstdlib>
#include <iostream>
#include <string.h>
#include "Base.h"
#include "SymTab.h"
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
//     operator > - override > operator return if one Base is greater than 
//     			another
//     
//==========================================================================
class UCSDStudent : public Base {
	char * name;
	long studentnum;
public:

        // your code goes here
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
% val			student number of the UCSDStudent
***************************************************************************/
	UCSDStudent (char * nm, long val = 0) :
		name (strdup (nm)), studentnum (val) {}
/***************************************************************************
% Routine Name : UCSDStudent (const UCSDStudent & student)
% File :         Driver.c
% 
% Description :  This function allocate and initial memory for UCSDStudent
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
% student		a student
***************************************************************************/
	UCSDStudent (const UCSDStudent & student) {
		name = strdup (student.name);
		studentnum = student.studentnum;
	}
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
		free(name);
	}
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
	long operator == (const Base & bbb) const {
		return ! strcmp (name, bbb);
	}
/***************************************************************************
% Routine Name : operator > (const Base & base)const
% File :         Driver.c
% 
% Description :  This functin cast override < operator so it compares the
%			name of two UCSDStudents and return true if one is more
%
% Parameters descriptions :
% 
% name              	description
% ------------------ ------------------------------------------------------
***************************************************************************/
	long operator > (const Base & bbb) const {
		return (strcmp (name, bbb) > 0) ? 1 : 0;
	}

	ostream & Write (ostream & stream) const {
		return stream << "name:  " << name
			<< "  studentnum:  " << studentnum;
	}
};

int main (int argc, char * const * argv) {
	char buffer[80];
	char command;
	long number;

	Tree::Set_Debug(0);

	if (argc != 1 && strcmp("-x", argv[1]) == 0) {
		Tree::Set_Debug(1);
	}
	
	SymTab * ST;
	ST = new SymTab ("UCSDStudentTree");
	ST->Write (cout << "Initial Symbol Table:\n");

	while (cin) {
		command = NULL;		// reset command each time in loop
		cout <<  "Please enter a command:  ((a)llocate, is(e)mpty," <<
			" (i)nsert, (l)ookup, (r)emove, (w)rite):  ";
		cin >> command;

		switch (command) {

		case 'a':
			cout << "Please enter name of new Tree to allocate:  ";
			cin >> buffer; 

			delete ST;
			ST = new SymTab (buffer);

			break;

		case 'e':
                        if (ST->IsEmpty ())
                                cout << "Tree is empty." << endl;
                        else
                                cout << "Tree is not empty." << endl;
                        break;
			
		case 'i':
			cout << "Please enter UCSD student name to insert:  ";
			cin >> buffer;	// formatted input

			cout << "Please enter UCSD student number:  ";
			cin >> number;

			// create student and place in symbol table
			ST->Insert (new UCSDStudent (buffer, number));
			break;

		case 'l': {
			const Base * found;	// whether found or not

			cout << "Please enter UCSD student name to lookup:  ";
			cin >> buffer;	// formatted input

			UCSDStudent stu (buffer, 0);
			found = ST->Lookup (&stu);
			
			if (found)
				found->Write (cout << "Student found!\n") << 
					"\n";
			else
				cout << "student " << buffer << " not there!\n";
			}
			break;
		
		case 'r': {
			Base * removed;	// data to be removed

			cout << "Please enter UCSD student name to remove:  ";
			cin >> buffer;	// formatted input

			UCSDStudent stu (buffer, 0);
			removed = ST->Remove (&stu);

			if (removed)
				removed->Write (cout << "Student removed!\n") 
					<< "\n";
			else
				cout << "student " << buffer << " not there!\n";
			}
			break;


		case 'w':
			ST->Write (cout << "The Symbol Table contains:\n");
		}
	}

	ST->Write (cout << "\nFinal Symbol Table:\n");
	delete ST;
}
