/**************************************************************************** 
 
                                                                Zifan Zhang 
                                                         	CSE 12, Fall 19 
                                                                9/27/2019 
                                                             	cs12fa19gt 
                		Assignment One 
 
File Name:  	hw1.java 
Description:    This program tests functions to display output strings and  
        	numbers.  
****************************************************************************/
/**
 * The hw1 class is a direct port of hw1.c to java.
 * As you already know in java when you pass literal strings like
 * <P>
 *   writeline("a literal string\n", stream);
 * <P>
 * in C is considered a char[], but in java it's automatically
 * converted and treated as a String object.  Therefore 
 * the function writeline accepts literal strings and 
 * String types.
 */

import java.io.*;      // System.out

public class hw1{
        //number of hex digits to display	
	private static final long COUNT = 16;
	private static final long DECIMAL = 10;	// to indicate base 10
	private static final long HEX = 16;		// to indicate base 16

	private static final char digits[] = 	// used for ASCII conversion
		new String("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").toCharArray
		();
	private static long hexCounter = 0; 
	// counter for the number hex digits


	/**
	* Takes in a positive number and displays in a given base.
        *
        * @param    Numeric value to be displayed.
        * @param    Base to used to display number.
        * @param    Where to display, likely System.out or System.err.
        */
	private static void baseout (long number, long base, PrintStream 
			stream) {
		char[] charOut = new char[10000000];//array that store output
		int index = 0;			      //index number starts 0
		/* when number is 0, print 0 */
		if(number == 0)
		{
			stream.print(0);
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
				stream.print('0');
			}
		}
		/* print out each char stored in charOut */
		while(index > 0)
		{
			stream.print(charOut[index]);
			index--;
		}
	}


        /**
        * Takes in a positive number and displays it in decimal.
        *
        * @param    positive numeric value to be displayed.
        * @param    Where to display, likely System.out or System.err.
        */
	public static void decout (long number, PrintStream stream) {
		baseout(number, DECIMAL, stream);
	}


        /**
        * Displays a single character.
        *
        * @param    Character to display.
        * @param    Where to display, likely System.out or System.err.
        */
        public static void fputc(char CharToDisp, PrintStream stream) {

                // Print a single character.
                stream.print (CharToDisp);   

                // Flush the system.out buffer, now. 
                stream.flush ();
        }


        /**
        * Takes in a positive number and displays it in hex.
        *
        * @param    A positive numeric value to be displayed in hex.
        * @param    Where to display, likely System.out or System.err.
        */
        public static void hexout (long number, PrintStream stream) {

                // Output "0x" for hexidecimal.
                writeline ("0x", stream);
                baseout (number, HEX, stream);
        }


        /**
        * Prints out a newline character.
        * @param    Where to display, likely System.out or System.err.
        *
        */
        public static void newline ( PrintStream stream ) {
		stream.print('\n');
        }


        /**
        * Prints out a string.
        *
        * @param    A string to print out.
        * @param    Where to display, likely System.out or System.err.
        * @return     <code>int</code> The length of the string.
        */
        public static long writeline (String message, PrintStream stream) {
                stream.print(message);
		return message.length();
	}


	public static void main( String[] args ) {

		writeline ("Merhaba Dunya", System.err);
		newline(System.err);
		writeline ("Hello World", System.out);
		newline(System.out);
		decout (123, System.out);
		newline(System.out);
		decout (0, System.out);
		newline(System.out);
		hexout (0xFEEDDAD, System.out);
		newline(System.out);
	}
}
