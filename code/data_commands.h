
#ifndef UNTITLED1_DATA_COMMANDS_H
#define UNTITLED1_DATA_COMMANDS_H
#include "valid_check.h"
#include "main.h"
#include "structors.h"

/* Functions */
int to_define(Mdefine temp);
int to_string(Mdefine temp);
int to_data(Mdefine temp);
int if_defined_extern(char* temp);
int if_defined_entries(char* temp);
int to_extern (Mdefine temp);
int to_entry (Mdefine temp);
int if_define(Mdefine temp);
void enter_to_extern_pointer(EXTERN temp);
void enter_to_entry_pointer(entry temp);
void enter_to_mdefine_pointer(Mdefine temp);
extern int OBC;
extern int mdefine_capacity;
extern int entries_count ;
extern int extern_count ;
extern int mdefine_count;
extern int extern_capacity;
extern int DC;
extern entry* entries_pointers;
extern EXTERN* extern_pointers;

#endif /*UNTITLED1_DATA_COMMANDS_H*/
