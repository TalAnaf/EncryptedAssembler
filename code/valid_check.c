#include "valid_check.h"

/************************************************************************
* Function Name: valid_check_for_commands
* Input: char *command_temp
* Output: int
* Function Operation: The function returns if the command from the line is correct
************************************************************************/
int valid_check_for_commands(const char *command_temp) {
    /* Define actions */
    int command_array_length = 15;
    const char *action_array[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
                                  "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "hlt"};
    int i;


    /* Array of command strings */
    for (i = 0; i < command_array_length; i++) {
        if (strcmp(command_temp, action_array[i]) == 0) {

            return i;
        }
    }
    return FALSE;
} /* End of valid_check function */


/************************************************************************
* Function Name: valid_check_reg
* Input: char* temp_rd
* Output: int
* Function Operation: The function returns if the reg in the line is correct
************************************************************************/
int valid_check_reg (char* temp_rd){
    int  i =0;
    int error_flag = FALSE;

    while (i< NUMBER_OF_REGISTER){
        if (strcmp(temp_rd,reg_array[i])==0){

            error_flag = i;
            goto end;
        }
        i++;
    }end:
    return error_flag;
}

/************************************************************************
* Function Name: count_operands
* Input: const char* command
* Output: int
* Function Operation: The function counts the number of operands in an assembly command
************************************************************************/
int count_operands(const char* command) {
    int count = 0;
    const char* temp = strtok((char*)command, " ,");

    while (temp != NULL) {
        count++;
        temp = strtok(NULL, " ,");
    }
    return count - 1;
}


/************************************************************************
* Function Name: rest_of_string
* Input: char* str,char* substring
* Output: char
* Function Operation: The function returns the rest of the string given
 * string and sub string
************************************************************************/
char* rest_of_string(char* str, const char* substring) {
    /* Find the position of the substring within str*/
    char* position = malloc(sizeof(char)*MAX_LENGTH);

    if (position==NULL||str==NULL){
        printf("Memory allocation failed!");
        free(position);
        free(str);
        return NULL;
    }
    position = strstr(str, substring);
    if (position) {
        /*Return the pointer to the rest of the string after the substring*/
        return position + strlen(substring);
    }
    free(position);
    return str;
}

/************************************************************************
* Function Name: remove_commas
* Input: char* str
* Output: char
* Function Operation: The function removes the commas from the line to easier the
 * process
************************************************************************/
void remove_commas(char *str) {
    char *read_ptr = str;
    char *write_ptr = str;
    while (*read_ptr != '\0') {
        if (*read_ptr != ',') {
            *write_ptr = *read_ptr;
            write_ptr++;
        } else {
            *write_ptr = ' ';
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}

/************************************************************************
* Function Name: remove_colon
* Input: char* str
* Output: char
* Function Operation: The function removes the colons from the line to easier the
 * process
************************************************************************/
char* remove_colon(char* str) {
    char* colon_pos = strchr(str, ':');
    if (colon_pos) {
        /* Shift all characters after the colon to the left by one position*/
        memmove(colon_pos, colon_pos + 1, strlen(colon_pos));
    }
    return str;
}

int is_valid_integer(const char* str) {
        if (*str == '-' || *str == '+') str++;
        if (*str == '\0') return FALSE;
        while (*str) {
            if (!isdigit(*str)) return FALSE;
            str++;
        }
    /* Valid integer (string of digits)*/
    return TRUE;
}

int is_valid_string(const char* str) {
    while (*str) {
        if (!isalpha(*str)) {
            return FALSE; /* Not a valid string*/
        }
        str++;
    }
    return TRUE; /* Valid string*/
}
