/****************************************************************************

                                                        Tracker Wonderdog
                                                        CSE 12, Winter 2015
                                                        January 8, 2015
                                                        cs12xzz
                                Assignment Two

File Name:      hw2.c
Description:    This program reads strings and integers from the user,
                processes them, and prints them back to the user.  Program
                terminates when user enters ^D.  At termination, program
                outputs sizes of various types of C/C++ pre defined types.

****************************************************************************/

/* declare fputc/fgetc */
#include <stdio.h>
#include <getopt.h>

/* define some program-wide constants */
#define ASCII_ZERO '0'
#define COUNT ((long) (sizeof (long) << 1))
#define ERROR -1
#define FALSE 0
#define MAXLENGTH 80
#define OFFSET ('a' - 'A')
#define SPACE ' '
#define TRUE 1

#define DECIMAL 10
#define HEX 16

#define NL '\n' /* new line character */

/* define the keyword "NULL" as 0 */
#ifdef NULL
#undef NULL
#endif
#define NULL 0

/* declarations for functions defined in this file */
void baseout (long number, long base, FILE *stream);
void clrbuf (int);
long decin (void);
void decout (unsigned long, FILE *);
void digiterror (int, long *, const char *);
long getaline (char *, int);
void hexout (unsigned long, FILE *);
void newline (FILE *);
long writeline (const char *, FILE *);

/* array for input checking and for output */
const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* messages */
const char CAUSED_OVERFLOW[] =  " caused overflow!!!\n";
const char DIGIT_STRING[] = "digit ";
const char REENTER_NUMBER[] = "\nPlease reenter number: ";
const char OUT_OF_RANGE[] = " out of range!!!\n";

/* debug messages */
const char DEBUG_GETALINE[] =
        "[*DEBUG:  The length of the string just entered is ";
const char DEBUG_WRITELINE[] =
        "\n[*DEBUG:  The length of the string displayed is ";
static long debug_on = FALSE;

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
void baseout (long number, long base, FILE * stream) {
	char charOut[BUFSIZ];/*a very larch array that stores output*/
	int index = 0;	     /*index number that starts from 0*/
	int i;		     /*index number used in for loop*/
	/* when number is 0, print 0 */
	if(number == 0)
	{
		fputc('0', stream);
	}
	/* when number is not 0, print each digits of number converted
	 * into desired base */
	while(number != 0)
	{
		index++;
		charOut[index] = digits[number % base];
		number /= base;
	}
	/* if base is HEX, add 0s to make the output 16 digits long */
	if(base == HEX)
	{
		for(i = 0; i < COUNT - index; i++)
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
void clrbuf (int character) {  
	/* YOUR CODE GOES HERE */ 
	/* Until detects user input equals newline or EOF keep popping */
	while(character != NL && character != EOF)
	{
		character = fgetc(stdin);
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
long decin (void) {
	/* YOUR CODE GOES HERE */
	long result = 0;/* result number to be returned */
	int charIn = fgetc(stdin);/* character got from stdin */
	/* checks for EOF */
	if(charIn == EOF)
		return EOF;
	/* get numbers from stdin */
	while(charIn - ASCII_ZERO < DECIMAL && charIn - ASCII_ZERO >= 0)
	{
		if(charIn == EOF)
			return EOF;
		if(charIn == NL)
			break;
		/* make sure the number don't over flow */
		if(((result * 10 + charIn - ASCII_ZERO) - charIn + ASCII_ZERO)
				/ 10 != result)
			break;
		result = result * 10 + charIn - ASCII_ZERO;
		charIn = fgetc(stdin);
	}
	clrbuf(charIn);
	return result;
}


/*--------------------------------------------------------------------------
    Copy your function header and code for decout() from hw1
--------------------------------------------------------------------------*/
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
void decout (unsigned long number, FILE * stream) {
	baseout(number, DECIMAL, stream);
}



/*--------------------------------------------------------------------------
Function Name:          digiterror
Purpose:                This function handles erroneous user input.
Description:            This function reinitializes sum, displays and
                        error message to the user, and asks for fresh
                        input.
Input:                  character:  The character that began the problem.
                        sum:  A pointer to the sum to reinitialize.
                        message:  The message to display to the user.
Result:                 The message is displayed to the user.  sum is 0.
--------------------------------------------------------------------------*/
void digiterror (int character, long * sum, const char * message) {

	/* handle error */
	clrbuf (character);

	/* reset sum */
	*sum = 0;

	/* output error message */
	writeline (DIGIT_STRING, stderr);
	fputc (character, stderr);
	writeline (message, stderr);

	writeline (REENTER_NUMBER, stdout);
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
long getaline (char * message, int maxlength) {
	/* YOUR CODE GOES HERE */
	int count = 0;/* counts the items stored in message */
	int charIn = fgetc(stdin);/* get char from stdin, stores it in charIn */
	/* checks for EOF */
	if(charIn == EOF)
		return EOF;
	/* get char from stdin until items stored in message reaches maxlength*/
	while(count < maxlength)
	{
		if(charIn == EOF)
			break;
		if(charIn == NL)
			break;
		message[count] = (char)charIn;
		count++;
		charIn = fgetc(stdin);
	}
	message[count] = NULL;/* serves as a flag of when to stop */
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
void hexout (unsigned long number, FILE * stream) {
	/* Output for "0x" for hexadecimal. */ 
	writeline ("0x", stream);
	baseout (number, HEX, stream);
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
void newline (FILE * stream) {
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
long writeline (const char * message, FILE * stream) {
	int index = 0;
	/* display every char in message on stream */
	while(message[index])
	{
		fputc(message[index], stream);
		index ++;
	}
	return index;
}



/*--------------------------------------------------------------------------
Function Name:          main
Description:            This function asks for input and displays output
Purpose:                This program asks the user to enter a string
                        and a number.  Computations are performed on the 
                        strings and numbers, and the results are displayed.
Description:            This function asks for input and displays output
                        in an infinite loop until EOF is detected.  Once EOF
                        is detected, the lengths of the types are displayed.
Input:                  None.
--------------------------------------------------------------------------*/
int main (int argc, char *const* argv) {
	char buffer[MAXLENGTH];      /* to hold string */
	long number;                  /* to hold number entered */
	long strlen;                  /* length of string */
	long base;		      /* to hold base entered */

	long array[10];            /* to show user where memory is allocated */
	long * ap = array;	   /* to show user about addresses in memory */
	long ** app = &ap;	   /* to show user about addresses in memory */
	long * apx = &array[0];	/* to show user about addresses in memory */
	char option;                 /* the command line option */

	/* initialize debug states */
	debug_on = FALSE;

	/* check command line options for debug display */
	while ((option = getopt (argc, argv, "x")) != EOF) {
		switch (option) {
			case 'x': debug_on = TRUE; break;
		}
	}

	/* infinite loop until user enters ^D */
	while (1) {
		writeline ("\nPlease enter a string:  ", stdout);
		strlen = getaline (buffer, MAXLENGTH);
		newline (stdout);

		/* check for end of input */
		if (strlen == EOF)
			break;

		writeline ("The string is:  ", stdout);
		writeline (buffer, stdout);

		writeline ("\nIts length is ", stdout);
		decout (strlen, stdout);
		newline (stdout);

		writeline ("\nPlease enter a decimal number:  ", stdout);
		if ((number = decin ()) == EOF)
			break;

		writeline ("\nPlease enter a decimal base:  ", stdout);
		if ((base = decin ()) == EOF)
			break;

		/* correct bases that are out of range */
		if (base < 2)
			base = 2;
		else if (base > 36)
			base = 36;

		newline (stdout);

		writeline ("Number entered in base ", stdout);
		decout (base, stdout);
		writeline (" is: ", stdout);
		baseout (number, base, stdout);

		writeline ("\nAnd in decimal is:  ", stdout);
		decout (number, stdout);

		writeline ("\nAnd in hexidecimal is:  ", stdout);
		hexout (number, stdout);

		writeline ("\nNumber entered multiplied by 8 is:  ", stdout);
		decout (number << 3, stdout);
		writeline ("\nAnd in hexidecimal is:  ", stdout);
		hexout (number << 3, stdout);

		newline (stdout);
	}

	writeline ("\nThe value of ap is:  ", stdout);
	decout ((long) ap, stdout);
	writeline ("\nAnd in hexidecimal is:  ", stdout);
	hexout ((long) ap, stdout);
	newline (stdout);

	writeline ("The value of app is:  ", stdout);
	decout ((long) app, stdout);
	writeline ("\nAnd in hexidecimal is:  ", stdout);
	hexout ((long) app, stdout);
	newline (stdout);

	writeline ("The value of apx is:  ", stdout);
	decout ((long) apx, stdout);
	writeline ("\nAnd in hexidecimal is:  ", stdout);
	hexout ((long) apx, stdout);
	newline (stdout);

	writeline ("The value of ap + 1 is:  ", stdout);
	decout ((long) (ap+1), stdout);
	writeline ("\nAnd in hexidecimal is:  ", stdout);
	hexout ((long) (ap+1), stdout);
	newline (stdout);

	writeline ("The address of array[0] is:  ", stdout);
	decout ((long) &array[0], stdout);
	newline (stdout);

	writeline ("The address of array[1] is:  ", stdout);
	decout ((long) &array[1], stdout);
	newline (stdout);

	writeline ("The size of a float is:  ", stdout);
	decout (sizeof (float), stdout);
	newline (stdout);

	writeline ("The size of a double is:  ", stdout);
	decout (sizeof (double), stdout);
	newline (stdout);

	writeline ("The size of a long double is:  ", stdout);
	decout (sizeof (long double), stdout);
	newline (stdout);

	writeline ("The size of a char is:  ", stdout);
	decout (sizeof (char), stdout);
	newline (stdout);

	writeline ("The size of an int is:  ", stdout);
	decout (sizeof (int), stdout);
	newline (stdout);

	writeline ("The size of a short is:  ", stdout);
	decout (sizeof (short), stdout);
	newline (stdout);

	writeline ("The size of a short int is:  ", stdout);
	decout (sizeof (short int), stdout);
	newline (stdout);

	writeline ("The size of a long is:  ", stdout);
	decout (sizeof (long), stdout);
	newline (stdout);

	writeline ("The size of a long int is:  ", stdout);
	decout (sizeof (long int), stdout);
	newline (stdout);

	writeline ("The size of a long long is:  ", stdout);
	decout (sizeof (long long), stdout);
	newline (stdout);

	writeline ("The size of a signed is:  ", stdout);
	decout (sizeof (signed), stdout);
	newline (stdout);

	writeline ("The size of a signed char is:  ", stdout);
	decout (sizeof (signed char), stdout);
	newline (stdout);

	writeline ("The size of a signed short is:  ", stdout);
	decout (sizeof (signed short), stdout);
	newline (stdout);

	writeline ("The size of a signed short int is:  ", stdout);
	decout (sizeof (signed short int), stdout);
	newline (stdout);

	writeline ("The size of a signed int is:  ", stdout);
	decout (sizeof (signed int), stdout);
	newline (stdout);

	writeline ("The size of a signed long is:  ", stdout);
	decout (sizeof (signed long), stdout);
	newline (stdout);

	writeline ("The size of a signed long int is:  ", stdout);
	decout (sizeof (signed long int), stdout);
	newline (stdout);

	writeline ("The size of a signed long long is:  ", stdout);
	decout (sizeof (signed long long), stdout);
	newline (stdout);

	writeline ("The size of an unsigned is:  ", stdout);
	decout (sizeof (unsigned), stdout);
	newline (stdout);

	writeline ("The size of an unsigned char is:  ", stdout);
	decout (sizeof (unsigned char), stdout);
	newline (stdout);

	writeline ("The size of an unsigned short is:  ", stdout);
	decout (sizeof (unsigned short), stdout);
	newline (stdout);

	writeline ("The size of an unsigned short int is:  ", stdout);
	decout (sizeof (unsigned short int), stdout);
	newline (stdout);

	writeline ("The size of an unsigned int is:  ", stdout);
	decout (sizeof (unsigned int), stdout);
	newline (stdout);

	writeline ("The size of an unsigned long is:  ", stdout);
	decout (sizeof (unsigned long), stdout);
	newline (stdout);

	writeline ("The size of an unsigned long int is:  ", stdout);
	decout (sizeof (unsigned long int), stdout);
	newline (stdout);

	writeline ("The size of an unsigned long long is:  ", stdout);
	decout (sizeof (unsigned long long), stdout);
	newline (stdout);

	writeline ("The size of a void pointer is:  ", stdout);
	decout (sizeof (void *), stdout);
	newline (stdout);

	writeline ("The size of a character pointer is:  ", stdout);
	decout (sizeof (char *), stdout);
	newline (stdout);

	writeline ("The size of an int pointer is:  ", stdout);
	decout (sizeof (int *), stdout);
	newline (stdout);

	writeline ("The size of a long pointer is:  ", stdout);
	decout (sizeof (long *), stdout);
	newline (stdout);

	writeline ("The size of a float pointer is:  ", stdout);
	decout (sizeof (float *), stdout);
	newline (stdout);

	writeline ("The size of a double pointer is:  ", stdout);
	decout (sizeof (double *), stdout);
	newline (stdout);

	writeline ("The size of a long double pointer is:  ", stdout);
	decout (sizeof (long double *), stdout);
	newline (stdout);

	newline (stdout);

	return 0;
}
