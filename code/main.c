#include "main.h"
#include "pre_processor.h"
#include "first_move.h"

/************************************************************************
* Function Name: main
* Output: int (After finishing analyzing all the assembly files)
* Function Operation: The function receives names of assembler files from the user
					  and starts a compilation process for the files
************************************************************************/
/* argc - will contain the number of arguments passed to the program from the command line */
/* *argv[] - will contain the value of the arguments passed to the program from the command line in an array in order */

int main (int argc, char *argv[])
{
    int i;

    for (i = 1 ; i < argc ; i++) /*for each file*/
    {
        if (first_move_writing(argv[i]) == FALSE)
            continue;
        first_step(argv[i]);
    }
    return 0;
}