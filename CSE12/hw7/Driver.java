/**************************************************************************** 
 
                                                                Zifan Zhang 
                                                         	CSE 12, Fall 19 
                                                                11/12/2019 
                                                             	cs12fa19gt 
                		Assignment SEVEN
 
File Name:  	Driver.java
Description:    This pogram implement UCSDStudent to store student numbers
		with the associated student name
****************************************************************************/
import java.io.*;
//=========================================================================
// class UCSDStudent extends Base
//
// Description: Implements the UCSDStudent. Each UCSDStudent has a name
// 		and a student number. 
//
// Data Fields:
//     name (String)  - name of the UCSD student
//     studentnum (long)  - the student number of the UCSD student
//
// Public functions:
//     UCSDSudent - allocae memory and initialize the UCSDStudent
//     getName - return the name of the UCSDStudent
//     equals - return true if two UCSDStudent's name are equal
//     isGreaterThan - return true if a UCSDStudent's name is greater than 
//     			another's
//     toString - return a string that prints the data fields in UCSDStudent
//==========================================================================
class UCSDStudent extends Base {

        private String name;
        private long studentnum;

        /* YOUR CODE GOES HERE */
	/**
         * UCSDStudent
         * Allocates and initializes the memory for a UCSDStudent
         *
         * @param  nm	The name of the UCSDStudent
	 * @param  stdnum	The student number of the UCSDStudent
         */
	public UCSDStudent (String nm, long stdnum) {
		name = new String (nm);
		studentnum = stdnum;
	}
	/**
         * UCSDStudent
         * Allocates and initializes the memory for a UCSDStudent
         *
         * @param  student	A UCSDStudent
         */
	public UCSDStudent (UCSDStudent student) {
		name = new String (student.name);
		studentnum = student.studentnum;
	}
	/**
         * getName
         * Return the name of a UCSDStudent
         *
	 * <return>  The student name of the UCSDStudent
         */
	public String getName () {
		return name;
	}
	/**
         * equals
         * return true if two UCSDStudents have the same name
         *
         * @param  object	The other UCSDStudent
	 * @<reurn>  	true if their name are the same false otherwise
         */
	public boolean equals (Object object) {
		if (this == object)
			return true;
		if (!(object instanceof UCSDStudent))
			return false;
		UCSDStudent otherStu = (UCSDStudent) object;
		return name.equals (otherStu.getName());
	}
	/**
         * isGreaterThan
         * return true if the UCSDStudent's name is greater than the another's
         *
         * @param  object	The other UCSDStudent
	 * @<reurn>  	true if name is greater than the other UCSDStudent's
         */
	public boolean isGreaterThan (Base base) {
		return (name.compareTo (base.getName ()) > 0) ? true : false;
	}
	/**
         * toString
         * return the data fields of UCSDStudent
         *
	 * @<reurn>  	data fields of UCSDStudent
         */
        public String toString () {
                return "name:  " + name + "  studentnum:  " + studentnum;
        }
}

public class Driver {
        private static final short NULL = 0;

        public static void main (String [] args) {
        
                /* initialize debug states */
                Tree.debugOff();

                /* check command line options */
                for (int index = 0; index < args.length; ++index) {
                        if (args[index].equals("-x"))
                                Tree.debugOn();
                }


                /* The real start of the code */
                SymTab<UCSDStudent> symtab = 
                                new SymTab<UCSDStudent>("UCSDStudentTree");
                String buffer = null;
                char command;
                long number = 0;

                System.out.println ("Initial Symbol Table:\n" + symtab);

                while (true) {
                        command = NULL; // reset command each time in loop
                        System.out.print ("Please enter a command:  " + 
                                "((a)llocate, is(e)mpty, (i)nsert, (l)ookup,"+
                                " (r)emove, (w)rite):  ");

                        try {
                        command = MyLib.getchar ();
                        MyLib.clrbuf (command); // get rid of return

                        switch (command) {
                        case 'a':
                                System.out.print
                                ("Please enter name of new Tree to " +
                                 "allocate:  ");
                                
                                buffer = MyLib.getline ();// formatted input
                                symtab = new SymTab<UCSDStudent>(buffer);
                                break;
			case 'e':
				if(symtab.isEmpty()){
					System.out.println("Tree is empty.");
				} else {
					System.out.println("Tree is "+
						"not empty.");
				}
				break;

                        case 'i':
                                System.out.print
                                ("Please enter UCSD student name to insert:  ");

                                buffer = MyLib.getline ();// formatted input

                                System.out.print
                                        ("Please enter UCSD student number:  ");

                                number = MyLib.decin ();
                                MyLib.clrbuf (command); // get rid of return

                                // create student and place in symbol table
                                symtab.insert(new UCSDStudent (buffer, number));
                                break;

                        case 'l': { 
                                UCSDStudent found;      // whether found or not

                                System.out.print
                                ("Please enter UCSD student name to lookup:  ");
                                buffer = MyLib.getline ();// formatted input

                                UCSDStudent stu = new UCSDStudent (buffer, 0);
                                found = symtab.lookup (stu);
                                
                                if (found != null) {
                                        System.out.println("Student found!");
                                        System.out.println(found);
                                }
                                else
                                        System.out.println ("student " + buffer
                                                + " not there!");
                                }
                                break;
                        
                        case 'r': { 
                                UCSDStudent removed; // data to be removed

                                System.out.print
                                ("Please enter UCSD student name to remove:  ");

                                buffer = MyLib.getline ();

                                UCSDStudent stu = new UCSDStudent (buffer, 0);

                                removed = symtab.remove(stu);

                                if (removed != null) {
                                        System.out.println("Student removed!"); 
                                        System.out.println(removed);
                                }
                                else
                                        System.out.println ("student " + buffer
                                                + " not there!");
                                }
                                break;

                        case 'w':
                                System.out.println("The Symbol Table " +
                                "contains:\n" + symtab);
                        }
                        }
                        catch (EOFException eof) {
                                break;
                        }
                }
                System.out.println("\nFinal Symbol Table:\n" + symtab);
        }
}
