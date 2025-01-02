#ifndef UNTITLED1_PRE_PROCESSOR_H
#define UNTITLED1_PRE_PROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int macro_count;
int if_macro_exist(char* label);
int get_macro_index(char* label);
int first_move_writing(char* assembler_name);

/* Define a struct for macro */
typedef struct macro {
    char* name; /* Name of the macro */
    char** commands; /* Command of the macro */
    int lineSum; /* The numbers of lines in the macro */
} macro;

extern macro* macros;

#endif /*UNTITLED1_PRE_PROCESSOR_H*/
