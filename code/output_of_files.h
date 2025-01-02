
#ifndef UNTITLED1_OUTPUT_OF_FILES_H
#define UNTITLED1_OUTPUT_OF_FILES_H
#include "main.h"
#include "first_move.h"
#include "valid_check.h"
#include "get_from_memory.h"
#include "second_move_commands.h"
#include "structors.h"
#include "data_commands.h"
#define BIT_SIZE_FOR_OPERAND 30
/* Function to make the output of the project */
void output(char* assembler_name);
int printing_entries(char* assembler_name);
int printing_extern(char* assembler_name);
void printing_last_move(char* assembler_name);
void print_an_error(int address, int type_of_error);

#endif /*UNTITLED1_OUTPUT_OF_FILES_H*/
