#include "get_from_memory.h"


/************************************************************************
* Function Name: get_define
* Input: char* label
* Output: char
* Function Operation: Returns the content of the define
************************************************************************/
char* get_define(char* label){
    int i = 0;
    while(i<OBC){
        if (mdefine_pointers[i].type!= DEFINE_D){
            i++;
            continue;
        }
        if (strcmp(mdefine_pointers[i].label,label)) {

            return mdefine_pointers[i].content;
        }
        i++;
    }
    return NULL;
}

/************************************************************************
* Function Name: get_from_data
* Input: char* temp
* Output: char
* Function Operation: The function Gets the value from data array
************************************************************************/
char* get_data_address(char* temp) {
    char* data_index = malloc(MAX_LABEL_LENGTH + 1);
    char* value = malloc(MAX_LENGTH);
    char* label = malloc(MAX_LABEL_LENGTH + 1);
    char* result = malloc(BITS_SIZE + 1);

    int i = 0;


    if (result==NULL|| data_index == NULL || value == NULL || label == NULL) {
        printf("Memory allocation failed!\n");
        result=NULL;
        goto end;
    }

    /* While loop to get the data from the mdefine pointer*/
    while (i < OBC) {
        if (strcmp(mdefine_pointers[i].label, temp) == 0) {

            strncpy(result, int_to_binary(mdefine_pointers[i].address),BITS_SIZE+1);
            goto end;
        }
        i++;
    }
    end:
    free(temp);
    free(data_index);
    free(value);
    free(label);
    return result;
}


/************************************************************************
* Function Name: get_binary_action
* Input: int decimal_of_action
* Output: char
* Function Operation: returns the binary of the command
************************************************************************/
char* get_binary_action(int decimal_of_action) {
    /* Arrays for action */
    char action_binary[16][5] = {"0000", "0001", "0010", "0011", "0100",
                                 "0101", "0110", "0111", "1000", "1001",
                                 "1010", "1011", "1100", "1101", "1110", "1111"};
    char* result = malloc(sizeof(char) * 5); /* Allocate space for a single string*/

    if (result == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    strncpy(result, action_binary[decimal_of_action], 5);

    return result;
}

/************************************************************************
* Function Name: get_mdefine_value
* Input: char* mdefine_label
* Output: char
* Function Operation: returns the mdefine value (in a case of the mdeifne is
 * define type)
************************************************************************/
char* get_mdefine_value(char* mdefine_label){
    int i=0;
    char* temp_content = malloc(sizeof(char)*MAX_LABEL_LENGTH);
    if (temp_content==NULL){
        return NULL;
    }
    while(i< OBC){
        if (strcmp(mdefine_pointers[i].label,mdefine_label)==0){
            return mdefine_pointers[i].content;
        }
        i++;
    }
    i=0;
    while(i<extern_count){
        if (strstr(extern_pointers[i].label,mdefine_label)==0){
            strncpy(temp_content,"0",MAX_LABEL_LENGTH);
            return temp_content ;
        }
        i++;
    }
    return NULL;
}

/************************************************************************
* Function Name: get_mdefine_type
* Input: char* mdefine_label
* Output: int
* Function Operation: returns the mdefine type (.define , .extern etc.)
************************************************************************/
int get_mdefine_type(char* mdefine_label){
    int i=0;
    while(i< OBC){
        if (strcmp(mdefine_pointers[i].label,mdefine_label)){
            free(mdefine_label);
            return mdefine_pointers[i].type;
        }
        i++;
    }
    free(mdefine_label);
    return FALSE;
}


/************************************************************************
* Function Name: get_binary_A_R_E
* Input: char word
* Output: char*
* Function Operation: The function checks the memory usage of the word and returns the binary code accordingly
************************************************************************/
char* get_binary_A_R_E(char* word) {
    int i = 0;
    char *result = malloc(sizeof(char) * 3);

    /* Checking if the content of the word depends on the value of an external symbol */
    if(if_defined_extern(word) == 0){
        strncpy(result, "01", 3);
        return result;
    }

    /* Checking if the content of the word depends on other memory location in the same file */
    else {
        while (i < OBC) {
            if (strcmp(word , mdefine_pointers[i].label) == 0) {
                strncpy(result, "10", 3);
                return result;
            }
            i++;
        }
    }

    /* If the content of the word does not depend on any other place in the memory */
    strncpy(result, "00", 3);
    return result;
}

/************************************************************************
* Function Name: free_memory
* Function Operation: The function frees all the memory of the arrays -
 *                    mdefine_pointers, line_pointers, extern_pointers, entries_pointers
************************************************************************/
void free_memory() {

    int i;
    /* free all the memory in mdefine_pointers array */
    for (i = 0; i < mdefine_count; i++) {
        free(mdefine_pointers[i].label);
        free(mdefine_pointers[i].content);
        free(mdefine_pointers[i].complete_line);
    }

    /* free all the memory in line_pointers array
    for (i = 0; i < line_count; i++) {
        free(line_pointers[i].rt);
        free(line_pointers[i].rd);
        free(line_pointers[i].command);
        free(line_pointers[i].complete_line);
        free(line_pointers[i].Addressing_modes_rt);
        free(line_pointers[i].Addressing_modes_rd);
    }*/


    /* free all the memory in extern_pointers array */
    for (i = 0; i < extern_count; i++) {
        free(extern_pointers[i].label);
    }


    /* free all the memory in entries_pointers array
    for (i = 0; i < entries_count; i++) {
        free(entries_pointers[i].label);
    }*/

}

