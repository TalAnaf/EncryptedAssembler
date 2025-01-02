
#ifndef UNTITLED1_SECOND_MOVE_COMMANDS_H
#define UNTITLED1_SECOND_MOVE_COMMANDS_H
#include "main.h"
#include "first_move.h"
#include "valid_check.h"
#include "get_from_memory.h"
#include "structors.h"
/* Functions */
char* int_to_binary(int num);
char* binary_from_char_imm(char* imm);
char* get_address_of_register(char* temp_r, int symbol);
char* two_operand_register_address(char* temp_rd,char* temp_rt);
char* base4_encode(char *base4_address);
char* analysis_addressing_mode(line temp, int symbol);

#endif /*UNTITLED1_SECOND_MOVE_COMMANDS_H*/
