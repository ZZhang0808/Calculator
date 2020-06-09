/****************************************************************************

                                                        Zifan Zhang
                                                        CSE 12, Fall 19
                                                        9/27/2019
                                                        cs12fa19gt
                                Assignment One

File Name:      hw1.c
Description:    This program tests functions to display output strings and 
                numbers. 
****************************************************************************/
#include <stdio.h>

#define COUNT ((long) (sizeof (long) << 1))
#define HEX 16
#define DECIMAL 10

void baseout (long number, long base, FILE * stream );
void decout (unsigned long number, FILE * stream);
void hexout (unsigned long number, FILE * stream);
void newline (FILE * stream);
long writeline (const char * message, FILE * stream);

/* array for input checking and for output */
const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";


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
	while(message[index])
	{
		fputc(message[index], stream);
		index ++;
	}
	return index;
}




int main (int argc, char *const* argv) {
  writeline ("Hello World", stdout);
  writeline ("Merhaba Dunya", stderr);
  newline(stderr);
  newline(stdout);
  decout (123, stdout);
  newline(stdout);
  decout (0, stdout);
  newline(stdout);
  hexout (0xFEEDDAD, stdout);
  newline(stdout);
  return 0;
}
