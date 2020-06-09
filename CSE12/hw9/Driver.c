/**************************************************************************** 
 
                                                                Zifan Zhang 
                                                         	CSE 12, Fall 19
                                                                12/6/2019 
                                                             	cs12fa19gt 
                		Assignment EIGHT
 
File Name:  	Driver.h
Description:    This pogram implement a a symble table to store a list of
		sorted UCSDStudents. 
 ****************************************************************************/
#include <iostream>
#include <cstdio>
#include <string>
#include <getopt.h>
#include "Driver.h"
#include "SymTab.h"
#include <fstream>

using namespace std;

#ifdef NULL
#undef NULL
#define NULL 0
#endif

ostream & operator << (ostream & stream, const UCSDStudent & stu) {
        return stream << "name:  " << stu.name
                << " with studentnum:  " << stu.studentnum;
}

int main (int argc, char * const * argv) {
        char buffer[BUFSIZ];
        char command;
        long number;
        char option;
	char inputfile[] = ""; // stores input file user entered
	istream * fin = &cin; // holds the input stream 
	bool isCin = 1; 
	ostream * fout = &cout; // holds the output stream

        
        SymTab<UCSDStudent>::Set_Debug_Off ();

        while ((option = getopt (argc, argv, "x")) != EOF) {

        switch (option) {
                case 'x': SymTab<UCSDStudent>::Set_Debug_On ();
                        break;
                }       
        }
        
        SymTab<UCSDStudent> ST = SymTab<UCSDStudent>("Driver.datafile");
        ST.Write (cout << "Initial Symbol Table:\n" );

        while (1) {
                command = NULL;         // reset command each time in loop
                *fout << "Please enter a command ((i)nsert, "
                        << "(l)ookup, (r)emove, (w)rite):  ";
                *fin >> command;

                switch (command) {

		case 'f': { 
			// prompts the user to enter a file for input
			*fout << "Please enter file name for commands:  ";
			*fin >> inputfile;
			// delete the old input stream if wasn't cin
			if (!isCin) { 
				delete fin;
				delete fout;
			}
			// assign new input and output stream
			fin = new ifstream (inputfile);
			fout = new ofstream ("/dev/null");
			isCin = 0;
			break;
		}
                case 'i': {
                        *fout << "Please enter UCSD student name to insert:  ";
                        *fin >> buffer;  // formatted input

                        *fout << "Please enter UCSD student number:  ";
                        *fin >> number;

                        UCSDStudent stu (buffer, number);

                        // create student and place in symbol table
                        ST.Insert (stu);
                        break;
                }
                case 'l': { 
                        unsigned long found;    // whether found or not

                        *fout << "Please enter UCSD student name to lookup:  ";
                        *fin >> buffer;  // formatted input

                        UCSDStudent stu (buffer, 0);
                        found = ST.Lookup (stu);
                        
                        if (found)
                                cout << "Student found!!!\n" << stu << "\n";
                        else
                                cout << "student " << buffer << " not there!\n";
                        break;
                        }
                case 'r': { 
                        unsigned long removed;

                        *fout << "Please enter UCSD student name to remove:  ";
                        *fin >> buffer;  // formatted input

                        UCSDStudent stu (buffer, 0);
                        removed = ST.Remove(stu);

                        if (removed)
                                cout << "Student removed!!!\n" << stu << "\n";
                        else
                                cout << "student " << buffer << " not there!\n";
                        break;
                }
                case 'w':
                        ST.Write (cout << "The Symbol Table contains:\n");
                }
		if(fin->peek() == EOF)
		{
			if(!isCin) {
				delete fin;
				delete fout;
				fout = &cout;
				fin = &cin;
				isCin = 1;
			}
			else {
				fin = 0;
				break;
			}
		}
        }
	ST.Write (cout << "\nFinal Symbol Table:\n");
	if (ST.GetOperation() != 0) {
        	cout << "\nCost of operations: ";
        	cout << ST.GetCost();
        	cout << " tree accesses";

        	cout << "\nNumber of operations: ";
        	cout << ST.GetOperation();

        	cout << "\nAverage cost: ";
        	cout << (((float)(ST.GetCost()))/(ST.GetOperation()));
        	cout << " tree accesses/operation\n";
	}

	else
        	cout << "\nNo cost information available.\n";
}
