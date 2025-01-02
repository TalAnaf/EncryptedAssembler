
#ifndef UNTITLED1_GET_FROM_MEMORY_H
#define UNTITLED1_GET_FROM_MEMORY_H
#include "main.h"
#include "first_move.h"
#include "valid_check.h"
#include "data_commands.h"

char* get_define(char* label);
char* get_data_address(char* temp);
char* get_binary_action(int decimal_of_action);
char* get_mdefine_value(char* mdefine_label);
char* get_binary_A_R_E(char* word);
void free_memory();

#endif /*UNTITLED1_GET_FROM_MEMORY_H*/
