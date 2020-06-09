/****************************************************************************

                                                        Zifan Zhang
                                                        CSE 12, Fall 2019
                                                        October 8, 2019
                                                        cs12fa19gt
                                Assignment Two

File Name:      hw2.java
Description:    This program reads strings and integers from the user,
                processes them, and prints them back to the user.  Program
                terminates when user enters ^D.  At termination, program
                outputs sizes of various types of C/C++ pre defined types.

****************************************************************************/
/**
 * The hw2 class is a direct port of hw2.c to java.
 * As you already know in java when you pass literal strings like
 * <P>
 *   writeline("a literal string\n", stream);
 * <P>
 * in C is considered a char[], but in java it's automatically
 * converted and treated as a String object.  Therefore 
 * the function writeline accepts literal strings and 
 * String types.  The getaline function returns a String type.
 */

import java.io.*;        // System.in and System.out
import java.util.*;      // Stack

class MyLibCharacter {
        private Character character;

        public MyLibCharacter (int ch) {
                character = new Character ( (char) ch );
        }

        public char charValue () {
                return character.charValue ();
        }

        public String toString () {
                return "" + character;
        }
}

public class hw2 {
	private static final int ASCII_ZERO = 48;

	private static final int CR = 13;		// Carriage Return
	private static final int MAXLENGTH = 80;	// Max string length

	private static final int EOF = -1;		// process End Of File

	private static final long COUNT = 16;		// # of hex digits

	private static final long DECIMAL = 10;		// to indicate base 10
	private static final long HEX = 16;		// to indicate base 16

	private static final char digits[] = 	// for ASCII conversion
	     new String("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").toCharArray();

	private static final String DEBUG_GETALINE = 
		"[*DEBUG:  The length of the string just entered is ";

	private static final String DIGIT_STRING = "digit ";
	private static final String REENTER_NUMBER ="\nPlease reenter number: ";
	private static final String OUT_OF_RANGE = " out of range!!!\n";
	private static final String CAUSED_OVERFLOW = " caused overflow!!!\n";
	private static final String DEBUG_WRITELINE =
		"\n[*DEBUG:  The length of the string displayed is ";

	private static Stack<MyLibCharacter> InStream =
		new Stack<MyLibCharacter>();

	private static boolean debug_on = false;
	private static long hexCounter = 0; // counter for the number hex digits

	private static final char NL = '\n';//new line character
	private static final int BUFSIZ = 1999999;//max size of char[]

/*--------------------------------------------------------------------------
    Copy your function header and code for baseout() from hw1
--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
Function Name:                baseout
Purpose:                      Takes in a positive number and displays 
			      in a given base.
Description:                  The baseout function takes the long integer 
			      number provided by the user, converts it to 
			      the base that is passed in, and displays it 
			      to the filestream stream.
Input:                        number:	numberic value to be displayed.
			      base:	base to used to display number.
			      stream:	where to display.
Output:                       None.
Result:                       Number displayed.
Side Effects:                 None.
---------------------------------------------------------------------------*/	
	private static void baseout (long number, long base, PrintStream 
			stream) {
		char[] charOut = new char[BUFSIZ];
		//array that store output
		int index = 0;//index number starts 0
		/* when number is 0, print 0 */
		if(number == 0)
		{
			fputc('0', stream);
		}
		/* when number is not 0, print the each digit of number 
		 * converted into desired base */
		while(number != 0)
		{
			index++;	
			charOut[index] = digits[(int)(number % base)];
			number /= base;
		}
		/* if base is 16, add 0s to make the output 16 digits long */
		if(base == HEX)
		{
			for (int i = 0; i < COUNT - index; i++)
			{
				fputc('0', stream);
			}
		}
		/* print out each char stored in charOut */
		while(index > 0)
		{
			fputc(charOut[index], stream);
			index--;
		}
	}



/*-------------------------------------------------------------------------- 
Function Name:          clrbuf 
Purpose:                When called, this function will clear stdin. 
Description:            This function checks to see if the incoming 
                        parameter is already '\n' indicating that stdin 
                        is already clear.  If stdin is already clear, this 
                        function does nothing.  Otherwise, this function 
                        calls "fgetc" in a loop until stdin is clear. 
Input:                  character:  the most recent character received from a 
                                previous call to fgetc. 
Result:                 stdin cleared.  Nothing returned. 
--------------------------------------------------------------------------*/ 
	public static void clrbuf (int character) {
		// YOUR CODE GOES HERE
		// until detects user input equals new line or EOF keep popping
		while(character != NL && character != EOF)
		{
			character = fgetc(System.in);
		}
        }


/*--------------------------------------------------------------------------
Function Name:          decin
Purpose:                This function accepts integer input from from the user.
Description:            This function processes user input in a loop that ends
                        when the user enters either a valid number or EOF.
                        If EOF is entered, EOF is returned.  Otherwise each
                        character entered is checked to verify that it is
                        numeric.  Non-numeric input is identified, the user
                        is notified, reprompted, and the loop begins again.
                        Once the input is verified to be valid, a series
                        of multiplication by 10 and addition can take
                        place to convert the ASCII characters entered into
                        a numeric quantity.
Input:                  None.
Result:                 The number entered or EOF.
--------------------------------------------------------------------------*/
        public static long decin() {
		// YOUR CODE GOES HERE
		long result = 0;//result number to be returned
		int charIn = fgetc(System.in);//character got from stdin
		//Checks for EOF
		if(charIn == EOF)
			return EOF;
		//a loop getting decimal numbers from stdin
		while(charIn - ASCII_ZERO < DECIMAL && charIn - ASCII_ZERO >= 0)
		{
			if(charIn == EOF)
				return EOF;
			if(charIn == NL)
				break;
			//make sure the number don't over flow
			if(((result * 10 + charIn - ASCII_ZERO) - charIn + 
						ASCII_ZERO) / 10 != result)
			{
				break;
			}
			result = result * 10 + charIn - ASCII_ZERO;
			charIn = fgetc(System.in);
		}
		clrbuf(charIn);
		return result;
        }


/*---------------------------------------------------------------------------
Function Name:          deout
Purpose:                Takes in a posityve number and displays it in decimal.
Description:            The decout function takes the long integer number 
			provided by the user and displays it to the filestream 
			stream as an integer.
Input:                  number: positive numeric value to be displayed.
			stream: where to display.
Output:                 None.
Result:                 Number displayed.
Side Effects:           None.
---------------------------------------------------------------------------*/
	public static void decout (long number, PrintStream stream) {
		baseout(number, DECIMAL, stream);
	}

	/*---------------------------------------------------------------------
	Function Name:          digiterror
	Purpose:                This function handles erroneous user input.
	Description:            This function  displays and error message to the
				user, and asks for fresh input.
	Input:                  character: The character that began the problem.
				message:  The message to display to the user.
	Result:                 The message is displayed to the user.
				The result in progress needs to be set to 0 in
				decin after the call to digiterror.
	----------------------------------------------------------------------*/
	public static void digiterror (int character, String message) {

		/* handle error */
		clrbuf (character);

		/* output error message */
		writeline (DIGIT_STRING, System.err);
		fputc ( (char)character, System.err);
		writeline (message, System.err);

		writeline (REENTER_NUMBER, System.err);
	}


/*--------------------------------------------------------------------------
Function Name:          getaline
Purpose:                This function will read a string from the user.
Description:            This function gets input from the user via
                        calls to fgetc up to some maximum number of
                        characters.  Input is terminated when either the
                        maximum number of characters are entered, or
                        a newline character is detected.  If the user
                        enters more characters than the maximum, clrbuf
                        is called to remove extra characters which are
                        ignored.  Since this is routine accepts input,
                        if EOF is detected EOF is passed back to the main
                        program.
Input:                  message:  the destination array where input is stored.
                        maxlength:  the maximum number of non-NULL characters
                                allowed in the string + 1 for the NULL char.
Result:                 User input is stored in message.
                        EOF is returned when the user enters ^D.
                        Otherwise, the length of the message is returned.
--------------------------------------------------------------------------*/
        public static long getaline( char message[], int maxlength ) {
		// YOUR CODE GOES HERE
		int count = 0; //count the number of items stored in message
		int charIn = fgetc(System.in);//get characters from stdin
		//checks for EOF
		if(charIn == EOF)
			return EOF;
		//get char from stdin and store it in message until maxlength
		//reached
		while(count < maxlength)
		{
			if(charIn == EOF)
				return EOF;
			if(charIn == NL)
				break;
			message[count] = (char)charIn;
			count++;
			charIn = fgetc(System.in);
		}
		//clears out the unused part in message
		while(maxlength > count)
		{
			message[maxlength-1] = 0;
			maxlength--;
		}	
		clrbuf(charIn);
		return count;
        }


/*--------------------------------------------------------------------------
    Copy your function header and code for hexout() from hw1
--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
Function Name:          hexout
Purpose:                Takes in a positive number and displays it in hex.
Description:            The hexout funciton takes the long integer number
			provided by the user and displays it to the filesream
			stream as a hexadecimal number.
Input:                  number:	a positive numeric value to be displayed in hex
			stream: where to display
Output:                 None.
Result:                 Number displayed in hex
Side Effects:           None.
---------------------------------------------------------------------------*/
	public static void hexout (long number, PrintStream stream) {

                // Output "0x" for hexidecimal.
                writeline ("0x", stream);
                baseout (number, HEX, stream);
        }	


        /**
        * Returns a character from the input stream.
        *
        * @return  <code>char</code> 
        */
        public static int fgetc(InputStream stream) {

                char ToRet = '\0';

                // Check our local input stream first.
                //   If it's empty read from System.in
                if (InStream.isEmpty ()) {

                        try {
                                // Java likes giving the user the
                                // CR character too. Dumb, so just 
                                // ignore it and read the next character
                                // which should be the '\n'.                  
                                ToRet = (char) stream.read ();
                                if (ToRet == CR)
                                        ToRet = (char) stream.read ();
                                
                                // check for EOF
                                if ((int) ToRet == 0xFFFF)
                                        return EOF;
                        }

                        // Catch any errors in IO.
                        catch (EOFException eof) {

                                // Throw EOF back to caller to handle
                                return EOF;
                        }

                        catch (IOException ioe) {

                                writeline ("Unexpected IO Exception caught!\n",
                                                        System.out);
                                writeline (ioe.toString (), System.out);
                        }

                }

                // Else just pop it from the InStream.
                else
                        ToRet = ((MyLibCharacter) InStream.pop ()).charValue ();
                return ToRet;
        }


        /**
        * Displays a single character.
        *
        * @param    Character to display.
        */
        public static void fputc(char CharToDisp, PrintStream stream) {

                // Print a single character.
                stream.print (CharToDisp);   

                // Flush the system.out buffer, now. 
                stream.flush ();
        }


/*--------------------------------------------------------------------------
    Copy your function header and code for newline() from hw1
--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
Function Name:          newline
Purpose:                Prints out a newline character.
Description:            Displays a newline character ('\n') to the 
			filestream stream.
Input:                  stream: where to display
Output:                 None.
Result:                 A newline character displayed.
Side Effects:           None.
---------------------------------------------------------------------------*/
	public static void newline ( PrintStream stream ) {
		fputc('\n', stream);
        }

/*--------------------------------------------------------------------------
    Copy your function header and code for writeline() from hw1
--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
Function Name:          writeline
Purpose:                Prints out a string
Description:            Displays the string message to the user via the 
			filestream stream. 
Input:                  message:a string to print out.
			stream:	where to display.
Output:                 The length of the string.
Result:                 The string displayed. The length of the string returned
Side Effects:           None.
---------------------------------------------------------------------------*/
	public static long writeline (String message, PrintStream stream) {
                stream.print(message);
		return message.length();
	}

	/**
	* Places back a character into the input stream buffer.
	*
	* @param    A character to putback into the input buffer stream.
	*/
	public static void ungetc (int ToPutBack) {

		// Push the char back on our local input stream buffer.
		InStream.push (new MyLibCharacter (ToPutBack));
	}


	public static void main( String[] args ) {

		char buffer[] = new char[MAXLENGTH];       /* to hold string */

		long number;                  /* to hold number entered */
		long strlen;                  /* length of string */
		long base;		      /* to hold base entered */

		/* initialize debug states */
		debug_on = false;

		/* check command line options for debug display */
		for (int index = 0; index < args.length; ++index) {
			if (args[index].equals("-x"))
				debug_on = true;
		} 

		/* infinite loop until user enters ^D */
		while (true) {
			writeline ("\nPlease enter a string:  ", System.out);

			strlen = getaline (buffer, MAXLENGTH);
			newline (System.out);

			/* check for end of input */
			if ( EOF == strlen )
				break;

			writeline ("The string is:  ", System.out);
			writeline ( new String(buffer), System.out);

			writeline ("\nIts length is ", System.out);
			decout (strlen, System.out);
			newline (System.out);

			writeline ("\nPlease enter a decimal number:  ", 
			             System.out);
			if ((number = decin ()) == EOF)
				break;

			writeline ("\nPlease enter a decimal base:  ", 
			             System.out);
			if ((base = decin ()) == EOF)
				break;

			/* correct bases that are out of range */
			if (base < 2)
				base = 2;
			else if (base > 36)
				base = 36;

			newline (System.out);

			writeline ("Number entered in base ", System.out);
			decout (base, System.out);
			writeline (" is: ", System.out);
			baseout (number, base, System.out);

			writeline ("\nAnd in decimal is:  ", System.out);
			decout (number, System.out);

			writeline ("\nAnd in hexidecimal is:  ", System.out);
			hexout (number, System.out);

			writeline ("\nNumber entered multiplied by 8 is:  ", 
			           System.out);
			decout (number << 3, System.out);
			writeline ("\nAnd in hexidecimal is:  ", System.out);
			hexout (number << 3, System.out);

			newline (System.out);
		}
	}
}
