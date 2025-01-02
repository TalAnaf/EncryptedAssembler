
#ifndef UNTITLED1_STRUCTORS_H
#define UNTITLED1_STRUCTORS_H
#include "main.h"
#define LENGTH_OF_ADDRESING_MODE 3

/* Define a structure for a line */
typedef struct line{ /* Label of the line */
    int command_point;
    int command_group;
    int address; /* Address of the line */
    char* rt; /* source operand */
    char* rd; /* destination operand */
    char* command; /* Command of the line */
    char* complete_line;
    char binary_first_word[BITS_SIZE + 1]; /* Binary representation of the command */
    char* Addressing_modes_rt;
    char* Addressing_modes_rd;
    int L;
}line;

typedef struct Mdefine{
    char* label;
    char* content;
    char* complete_line;
    line regular_line;
    int type;
    int address;
    int L;
}Mdefine;

typedef struct entry{
    char* label;
    int address;
}entry;

typedef struct EXTERN{
    char* label;
    int address;
}EXTERN;


#endif /*UNTITLED1_STRUCTORS_H*/
