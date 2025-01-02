#include "data_commands.h"
int OBC = 0;
int DC = 0;
int mdefine_capacity = 0;
int extern_capacity = 0;
int entries_capacity = 0;
int entries_count = 0;
int extern_count = 0;
int mdefine_count = 0;
entry* entries_pointers;
EXTERN* extern_pointers;

/************************************************************************
* Function Name: to_define
* Input: Mdefine temp
* Output: int
* Function Operation: The function processes the definition of a define
* and updates the necessary pointers and counts.
************************************************************************/
int to_define(Mdefine temp){
    int error_flag = TRUE;
    char* temp_value  = malloc(sizeof(char) * MAX_LABEL_LENGTH);
    char* temp_label = malloc(sizeof(char) * MAX_LENGTH);

    if (temp_label == NULL||temp_value==NULL) {

        printf("Memory allocation failed!");
        error_flag = FALSE;
        goto end;
    }


    sscanf(temp.content,"%s = %s",temp_label,temp_value);

    strcpy(temp.content,temp_value);
    strcpy(temp.label,temp_label);
    temp.L =1;
    error_flag = temp.L;
    if (if_define(temp)==FALSE){
        error_flag = FALSE;
        goto end;
    }

    end:
    DC++;
    enter_to_mdefine_pointer(temp);
    free(temp_label);
    free(temp_value);
    return error_flag;
}

/************************************************************************
* Function Name: if_define
* Input: Mdefine temp
* Output: int
* Function Operation: The function returns if the mdefine is already defined
************************************************************************/
int if_define(Mdefine temp) {
    int i = 0;
    int error_flag = TRUE;


    while (i < OBC) {
        if (strcmp(temp.label, mdefine_pointers[i].label) == 0) {
            error_flag = FALSE;
            goto end;

        }
        i++;}

    end:
    return error_flag;
}
/************************************************************************
* Function Name: to_string
* Input: Mdefine temp
* Output: int
* Function Operation: The function processes a string type  definition
* and updates the necessary pointers and counts.
************************************************************************/
int to_string(Mdefine temp) {
    int error_flag = TRUE;
    char* temp_string = temp.complete_line;
    const char *start = strchr(temp_string, '"');
    const char *end = strrchr(temp_string, '"');
    int length;

    if (temp_string == NULL) {
        printf("Memory allocation failed!");
        return FALSE;
    }
    if (strlen(temp.content) == 0) {
        free(temp_string);
        return FALSE;
    }
    /* Check if two quotation marks are found */
    if (start != NULL && end != NULL && start != end) {
        /* Calculation of the length of the string between the quotation marks */
        length = end - start - 1;


        /* Copying the characters between the quotes into the result string */
        strncpy(temp.content, start + 1, length);
        temp.content[length] = '\0';

        temp.L= strlen(temp.content);
        DC+=temp.L;
        error_flag = temp.L;
    }
    enter_to_mdefine_pointer(temp);
    return error_flag;
}
/************************************************************************
* Function Name: to_data
* Input: Mdefine temp
* Output: int
* Function Operation: The function processes a data type definition
* and updates the necessary pointers and counts.
************************************************************************/
int to_data(Mdefine temp) {
    /* Declaration Section */
    char* temp_input = malloc(sizeof(char) * MAX_LABEL_LENGTH);
    char* temp_int = malloc(sizeof(char) * MAX_LABEL_LENGTH);
    char* token = malloc(sizeof(char) * MAX_LABEL_LENGTH);
    int i= 0;
    int l_counter = 0;
    int error_flag = TRUE;
    temp.L = calculate_L_of_line(temp.content);
    if (!temp_input || !temp_int || !token) {
        printf("Dynamic allocation failed!\n");
        error_flag = FALSE;
        goto cleanup;
    }
    /* Initialization Section */

    strcpy(temp_input, temp.content);
    token = temp_input;

    /* Validation and Processing Section */
    while (sscanf(token, "%s", temp_int) == 1) {
        l_counter++;
        DC++;
        i++;
        if (i==1){
            temp.address++;
            l_counter--;
        }
        /* Check if it's a valid integer */
        if (is_valid_integer(temp_int) != FALSE) {
            /* Handle integer case */
            long value = strtol(temp_int, NULL, 10);
            if (value < 0) {


            } else {

            }
        } else if (is_valid_string(temp_int) != FALSE) {
            /* Handle string case */

        } else {
            /* Invalid content */
            print_an_error(temp.address, 7);
            error_flag = FALSE;
            goto cleanup;
        }
        /* Move to the next token */
        token += strlen(temp_int);
        while (*token && isspace(*token)) {
            token++; /* Skip whitespace */
        }
    }
    error_flag = temp.L=l_counter;
    enter_to_mdefine_pointer(temp);
    cleanup:
    free(temp_input);
    free(temp_int);
    return error_flag;
}



/************************************************************************
* Function Name: to_extern
* Input: Mdefine temp
* Output: int
* Function Operation: The function processes an extern type definition
* and updates the necessary pointers and counts.
************************************************************************/
int to_extern (Mdefine temp){
    char* temp_label = malloc(sizeof(char)*MAX_LABEL_LENGTH);
    EXTERN temp_extern;


        strncpy(temp.label,temp.content,MAX_LABEL_LENGTH);

        strncpy(temp_label,temp.label,MAX_LABEL_LENGTH);
        if (if_defined_entries(temp_label) == FALSE) {

            print_an_error(temp.address, 5);
            return FALSE;
        }
        else {

            temp_extern.label = malloc(sizeof(char)*MAX_LABEL_LENGTH);
            if (temp_extern.label==NULL){

                print_an_error(temp.address, 5);
                return FALSE;
            }

            strncpy(temp_extern.label, temp.label,MAX_LABEL_LENGTH);
            temp_extern.address = temp.address;
            enter_to_extern_pointer( temp_extern);
        }

    return 1;
}

/************************************************************************
* Function Name: to_entry
* Input: Mdefine temp
* Output: int
* Function Operation:The function processes an entry type definition
* and updates the necessary pointers and counts.
************************************************************************/
int to_entry (Mdefine temp){
    entry temp_entry;


        /* Checking if already defined as entries */
        if (if_defined_extern(temp.label) != FALSE) {

            print_an_error(temp.address, 5);
            return FALSE;
        }
        temp_entry.label = malloc(sizeof(char )*MAX_LENGTH);
    if (temp_entry.label==NULL){

        print_an_error(temp.address, 8);
        return FALSE;
    }

    strcpy(temp_entry.label, temp.content);
    temp_entry.address = temp.address;
    enter_to_entry_pointer(temp_entry);
    return 1;
}

/************************************************************************
* Function Name: if_defined_entries
* Input: char* temp
* Output: int
* Function Operation: Checks if the entries already defined
************************************************************************/
int if_defined_entries(char* temp){
    char* temp_check = malloc(sizeof(char)*MAX_LABEL_LENGTH);
    int i;
    if (temp_check==NULL){
        return FALSE;
    }
    strncpy(temp_check,temp,MAX_LABEL_LENGTH);

    for (i = 0; i < entries_count; i++) {
        if (strcmp(temp, entries_pointers[i].label) == 0) {
            return FALSE;
        }
    }
    return TRUE;
}

/************************************************************************
* Function Name: if_defined_extern
* Input: char* temp
* Output: int
* Function Operation:Checks if the externs already defined
************************************************************************/
int if_defined_extern(char* temp){
    int i;
    for (i = 0; i < extern_count; i++) {
        if (strcmp(temp, extern_pointers[i].label) == 0) {
            return i;
        }
    }
    return FALSE;
}
/************************************************************************
* Function Name: enter_to_mdefine_pointer
* Input: Mdefine temp
* Function Operation: The function enters the given mdefine into
* the mdefine pointers array, reallocating memory if necessary.
************************************************************************/
void enter_to_mdefine_pointer(Mdefine temp){

    if (OBC == 0) {
        mdefine_pointers = malloc(sizeof(Mdefine) * 1);
        if (mdefine_pointers == NULL) {

            return;
        }
        mdefine_capacity = 1;
    }

    else if (OBC >= mdefine_capacity) {
        mdefine_capacity *= 2;
        mdefine_pointers = realloc(mdefine_pointers, mdefine_capacity * sizeof(Mdefine));
        if (mdefine_pointers == NULL) {
            printf("Memory reallocation failed!\n");
            return;
        }
    }
    OBC++;


    mdefine_pointers[OBC-1]= temp;
}
/************************************************************************
* Function Name: enter_to_extern_pointer
* Input: Mdefine temp
* Function Operation: The function enters the given extern definition into
* the extern pointers array, reallocating memory if necessary.
************************************************************************/
void enter_to_extern_pointer(EXTERN temp){

    if (extern_count == 0) {
        extern_pointers = malloc(sizeof(Mdefine) * 1);
        if (extern_pointers == NULL) {
            printf("Memory allocation failed!\n");
            return;
        }
        extern_capacity = 1;
    }

    else if (extern_count >= extern_capacity) {
        extern_capacity *= 2;
        extern_pointers = realloc(extern_pointers, extern_capacity * sizeof(Mdefine));
        if (extern_pointers == NULL) {
            printf("Memory reallocation failed!\n");
            return;
        }
    }
    extern_count++;


    extern_pointers[extern_count-1]= temp;

}
/************************************************************************
* Function Name: enter_to_mdefine_pointer
* Input: Mdefine temp
* Function Operation: The function enters the given entry into
* the entry pointers array, reallocating memory if necessary.
************************************************************************/
void enter_to_entry_pointer(entry temp){

    if (entries_count == 0) {
        entries_pointers = malloc(sizeof(entry) * 1);
        if (entries_pointers == NULL) {
            printf("Memory allocation failed!\n");
            return;
        }
        entries_capacity = 1;
    }

    else if (entries_count >= entries_capacity) {
        entries_capacity *= 2;
        entries_pointers = realloc(entries_pointers, entries_capacity * sizeof(Mdefine));
        if (entries_pointers == NULL) {
            printf("Memory reallocation failed!\n");
            return;
        }
    }
    entries_count++;
    entries_pointers[entries_count-1]= temp;

}