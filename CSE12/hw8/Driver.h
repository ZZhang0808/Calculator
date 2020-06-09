#ifndef DRIVER_H
#define DRIVER_H

#include <string.h>
#include <iostream>
#include <cstdlib>
using namespace std;
//=========================================================================
// class UCSDStudent : 
//
// Description: Implements the UCSDStudent. Each UCSDStudent has a name
// 		and a student number. 
//
// Data Fields:
//     name (char)  - name of the UCSD student
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
class UCSDStudent {
        friend ostream & operator << (ostream &, const UCSDStudent &);
        char name[20];
        long studentnum;
public:
        /* YOUR CODE GOES HERE */
/***************************************************************************
% Routine Name : UCSDStudent (char * nm, long val = 0)
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
	UCSDStudent (char * nm, long val = 0) :studentnum (val) {
		// set name
		strcpy (name, nm);
	}
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
		// set name and student number
		strcpy (name, student.name);
		studentnum = student.studentnum;
	}
/***************************************************************************
% Routine Name : operator const char (void) const
% File :         Driver.c
% 
% Description :  This functin cast a UCSDStudent into a constant char of its
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
	long operator == (const UCSDStudent & stu) const {
		return ! strcmp (name, stu.name);
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
	long operator < (const UCSDStudent & stu) const {
		return (strcmp (name, stu.name) < 0) ? 1 : 0;
	}

};

#endif
