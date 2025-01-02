#ifndef UNTITLED1_FIRST_MOVE_H
#define UNTITLED1_FIRST_MOVE_H

#include "main.h"
#include "valid_check.h"
#include "output_of_files.h"
#include "get_from_memory.h"
#include "data_commands.h"
#include "structors.h"

/* Flags for types of commands */
#define DATA_D  1
#define STRING_D  2 /*string_d*/
#define EXTERN_D  3  /*extern_d*/
#define DEFINE_D  4  /* define_d*/
#define ENTRY_D  5  /*entry_d*/
#define REUGLAR_LINE  6 /*In case of regular line*/
#define MAX_REG_ADDRESSING_MODE 3



/* Global variable declarations */
extern int IC; /* IC - Instructions count */
extern int macro_count;
extern int line_count;
extern char reg_array[NUMBER_OF_REGISTER][REGISTER_LENGTH];
extern line* line_pointers;
extern Mdefine* mdefine_pointers;

/* Function declarations */
int line_analysis(char *current_line, int current_address);
void first_step();
int getting_line_type(int new_line_index);
int first_group_command(line* new_line);
int second_group_command(line* new_line);
int third_group_command(line* new_line);
void convert_binary_first_word(int line_pointer);
char* what_addressing_mode(char* temp);
int calculate_L_of_line(const char *line);
int what_action_group(int index);
int define_regular_line(char* current_line,int current_address);
int define_new_mdefine(char* current_line,int current_address,char* label_part);
int is_valid_integer(const char* str);
int is_valid_string(const char* str);
#endif /* UNTITLED1_FIRST_MOVE_H*/
