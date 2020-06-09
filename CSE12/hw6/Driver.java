/**************************************************************************** 
 
                                                                Zifan Zhang 
                                                         	CSE 12, Fall 19 
                                                                11/5/2019 
                                                             	cs12fa19gt 
                		Assignment SIX
 
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
//     hashCode - return the hash code of the UCSDStudent
//     equals - return true if two UCSDStudent's name are equal
//     isLessThan - return true if a UCSDStudent's name is less than another's
//     toString - return a string that prints the data fields in UCSDStudent
//==========================================================================
class UCSDStudent extends Base {
        private String name; //name of the UCSDStudent
        private long studentnum; //student number of the UCSDStudent

        /* YOUR CODE GOES HERE */
	/**
         * UCSDStudent
         * Allocates and initializes the memory for a UCSDStudent
         *
         * @param  buffer	The name of the UCSDStudent
	 * @param  number	The student number of the UCSDStudent
         */
	public UCSDStudent (String buffer, long number)
	{
		name = new String (buffer);
		studentnum = number;
	}
	/**
         * getName
         * return the name of UCSDStudent
         *
         * <return>   The name of UCSDStudent
         */
	public String getName()
	{
		return name;
	}
	/**
         * hashCode
         * return the hash code of UCSDStudent
         *
         * <return>   The hash code of UCSDStudent
         */
	public int hashCode () 
	{
                int retval = 0; //stores return value
                int index = 0; //counts number of characters in name added
		//loop through each char in name and add its ascii to retval
                while (index != name.length ()) {
                        retval += name.charAt (index);
                        index ++;
                }
		
                return retval;
        }
	/**
         * equals
         * return true if two UCSDStudents have the same name
         *
         * @param  object	The other UCSDStudent
	 * @<reurn>  	true if their name are the same false otherwise
         */

	public boolean equals (Object object)
	{
		Base bbb = (Base) object;
		return name.equals(bbb.getName());
	}
	/**
         * isLessThan
         * return true if the UCSDStudent's name is less than the another's
         *
         * @param  object	The other UCSDStudent
	 * @<reurn>  	true if name is less than the other UCSDStudent's
         */
	public boolean isLessThan (Base bbb)
	{
		return (name.compareTo (bbb.getName ()) < 0) ? true : false;
	}
	/**
         * toString
         * return the data fields of UCSDStudent
         *
	 * @<reurn>  	data fields of UCSDStudent
         */
	public String toString()
	{
		return "name: " + name + " Studentnum: " + studentnum;
	}
}

public class Driver {
        private static final short NULL = 0;

        public static void main (String [] args) {
                /* initialize debug states */
                HashTable.setDebugOff();

                /* check command line options */
                for (int index = 0; index < args.length; ++index) {
                        if (args[index].equals("-x"))
                                HashTable.setDebugOn();
                }

                /* The real start of the code */
                SymTab symtab = new SymTab (5);
                String buffer = null;
                char command;
                long number = 0;

                System.out.print ("Initial Symbol Table:\n" + symtab);

                while (true) {
                        command = 0;    // reset command each time in loop
                        System.out.print ("Please enter a command:  "
                                + "((i)nsert, (l)ookup, (w)rite):  ");

                        try {
                        command = MyLib.getchar ();
                        MyLib.clrbuf (command); // get rid of return

                        switch (command) {
                        case 'i':
                                System.out.print (
                                "Please enter UCSD Student name to insert:  ");
                                buffer = MyLib.getline ();// formatted input

                                System.out.print (
                                        "Please enter UCSD Student number:  ");

                                number = MyLib.decin ();
                                MyLib.clrbuf (command); // get rid of return

                                // create Student and place in symbol table
                                if(!symtab.insert (
                                        new UCSDStudent (buffer, number))){

                                        System.out.print ("\nFinal Symbol "
                                                        + "Table:\n" + symtab);
                                        System.exit(0);
                                }
                                break;

                        case 'l': {
                                Base found;     // whether found or not

                                System.out.print (
                                "Please enter UCSD Student name to lookup:  ");

                                buffer = MyLib.getline ();// formatted input

                                UCSDStudent stu = new UCSDStudent (buffer, 0);
                                found = symtab.lookup (stu);
                                
                                if (found != null) {
                                        System.out.println ("Student found!!!");
                                        System.out.println (found);
                                }
                                else
                                        System.out.println ("Student " + buffer
                                                + " not there!");
                                }
                                break;

                        case 'w':
                                System.out.print (
                                    "The Symbol Table contains:\n" + symtab);
                        }
                        }
                        catch (EOFException eof) {
                                break;
                        }
                }

                System.out.print ("\nFinal Symbol Table:\n" + symtab);
        }
}
