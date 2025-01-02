#ifndef UNTITLED1_VALID_CHECK_H
#define UNTITLED1_VALID_CHECK_H
#include "first_move.h"
#include "structors.h"
#include "data_commands.h"

/* Functions of valid check */
int valid_check_reg (char* temp_rd);
int valid_check_for_commands(const char *command_temp);
int count_operands(const char* command);
char* rest_of_string(char* str, const char* substring);
char* remove_colon(char* str);
void remove_commas(char *str);

#endif /*UNTITLED1_VALID_CHECK_H*/
