#include "output_of_files.h"

/************************************************************************
* Function Name: output
* Output: int
* Function Operation: runs the ouput of the second move
************************************************************************/
void output(char* assembler_name){
    printing_last_move(assembler_name);
    printing_extern(assembler_name);
    printing_entries(assembler_name);
    free_memory();
}
/************************************************************************
* Function Name: printing_entries
* Output: int
* Function Operation: prints the entries to different file
************************************************************************/
int printing_entries(char* assembler_name){
    FILE* entries_file ;
    int i = 0;
    char* assembler_name_to_write = (char*)malloc(sizeof(char) * MAX_LENGTH); /* Pointer to store the name of the output file */
    char* temp_label = malloc(sizeof(char) * MAX_LABEL_LENGTH);
    char* temp_Address =  malloc( sizeof(int) );
    if (entries_count==0){
        return TRUE;
    }
    if (temp_label==NULL || temp_Address==NULL || assembler_name_to_write == NULL){
        printf("Memory allocation failed!");
        free(assembler_name_to_write);
        free(temp_label);
        free(temp_Address);
        return FALSE;
    }

    strcpy(assembler_name_to_write, assembler_name);
    strcat(assembler_name_to_write, ".ent");
    entries_file = fopen(assembler_name_to_write, "w"); /* Open the output file for writing */

    if (entries_file == NULL) {
        printf("Error opening the file: \n");
        fclose(entries_file);
        return FALSE;
    }

    while (i<entries_count){

        fprintf(entries_file, "%s" "%s" "%d\n" , entries_pointers[i].label, "\t", entries_pointers[i].address);

        i++;
    }
    fclose(entries_file);
    free(assembler_name_to_write);
    free(temp_label);
    free(temp_Address);
    return TRUE;
}

/************************************************************************
* Function Name: printing_extern
* Output: int
* Function Operation: prints the extern to a defferint file
************************************************************************/
int printing_extern(char* assembler_name){
    FILE* extern_file;
    int i = 0;
    char* assembler_name_to_write = (char*)malloc(sizeof(char) * MAX_LENGTH); /* Pointer to store the name of the output file */
    char* temp_label = malloc(sizeof(char) * MAX_LABEL_LENGTH);
    char* temp_Address =  malloc( sizeof(int) );
    if (extern_count==0){
        return TRUE;
    }

    if (temp_label==NULL || temp_Address==NULL || assembler_name_to_write == NULL){
        printf("Memory allocation failed!");
        free(assembler_name_to_write);
        free(temp_label);
        free(temp_Address);
        return FALSE;
    }

    strcpy(assembler_name_to_write, assembler_name);
    strcat(assembler_name_to_write, ".ext");
    extern_file = fopen(assembler_name_to_write, "w"); /* Open the output file for writing */

    if (extern_file == NULL) {
        printf("Error opening the file: \n");
        fclose(extern_file);
        return FALSE;
    }

    while (i<extern_count){
        fprintf(extern_file, "%s" "%s" "%d\n" , extern_pointers[i].label, "\t", extern_pointers[i].address);
        i++;
    }
    fclose(extern_file);
    free(assembler_name_to_write);
    free(temp_label);
    free(temp_Address);
    return TRUE;
}

/************************************************************************
* Function Name: printing_last_move
* Output: int
* Function Operation: function that write the other address acordding to the
 * line pointer
************************************************************************/
void printing_last_move(char* assembler_name) {
    int i;
    char for_string;
    int rt_symbol =1;
    int rd_symbol =2;
    int line_index = 1;
    int define_index = 0;
    char* string;
    /*WHICH ACTION GROUP*/
    int command_group;
    char* temp_label = malloc(sizeof(char)*MAX_LABEL_LENGTH);
    char* temp_value = malloc(sizeof(char)*MAX_LABEL_LENGTH);
    char *address_of_rt = malloc(BIT_SIZE_FOR_OPERAND * sizeof(char));
    char *address_of_rd = malloc(BIT_SIZE_FOR_OPERAND* sizeof(char));
    char *new_line_last_words = (char *) malloc(BITS_SIZE * sizeof(char)+1);
    char* assembler_name_to_write = (char*)malloc(sizeof(char) * MAX_LENGTH); /* Pointer to store the name of the output file */
    FILE *temp_assembler;
    line temp;
    Mdefine temp_defined;

    strcpy(assembler_name_to_write, assembler_name);
    strcat(assembler_name_to_write, ".ob");
    temp_assembler = fopen(assembler_name_to_write, "w"); /* Open the output file for writing */

    if (temp_assembler == NULL) {
        printf("Error opening the file: \n");
        goto end;
    }
    if (temp_value==NULL||temp_label==NULL||new_line_last_words == NULL || address_of_rt == NULL || address_of_rd == NULL || assembler_name_to_write == NULL) {
        printf("Memory allocation failed!");
        goto end;
    }

     fprintf(temp_assembler, "%d" "%s" "%d\n" , IC, "\t", DC);

    /*Starting to print the lines*/
    while (line_index<line_count) {
        /*Getting the addressing mode*/
        temp = line_pointers[line_index];
        /*WHICH ACTION GROUP*/
        command_group = temp.command_group;
        /*Analyse the line according to the addressing mode*/
        fprintf(temp_assembler, "%d" "%s" "%s\n" ,temp.address, "\t" ,base4_encode(temp.binary_first_word));
        if (command_group == 1) {
            /*Getting the rd address*/
            strncpy(address_of_rd,analysis_addressing_mode(temp, rd_symbol),BIT_SIZE_FOR_OPERAND);
            strncpy(address_of_rt,analysis_addressing_mode(temp, rt_symbol),BIT_SIZE_FOR_OPERAND);

            if (strcmp(address_of_rd,"NO ADDRESS")==0){
                goto end_of_loop;
            }

            fprintf(temp_assembler, "%d" "%s" "%s\n" ,temp.address, "\t" ,base4_encode(address_of_rd));
            fprintf(temp_assembler, "%d" "%s" "%s\n" ,temp.address, "\t" ,base4_encode(address_of_rt));
        }
        /*If there is two offenders*/
        if (command_group == 2) {
            if (strcmp(address_of_rd,"NO ADDRESS")==0){
                goto end_of_loop;
            }
            /*Getting the rt address*/
            strncpy(address_of_rt,analysis_addressing_mode(temp, rt_symbol),BIT_SIZE_FOR_OPERAND);
            fprintf(temp_assembler, "%d" "%s" "%s\n" ,temp.address, "\t" , base4_encode(address_of_rt));
        }
        if (command_group==3){
            fprintf(temp_assembler, "%d" "%s" "%s\n" ,temp.address, "\t" ,  base4_encode(temp.binary_first_word));
            goto end_of_loop;
        }
        end_of_loop:
        line_index++;

    }
    i=0;
    /*Making a symbol with at least 30 chars for make sure there is ? */
    /*If there is no offenders , there is no other words to convert and the program will continue to the next line*/
    /*Printing the symbols*/
    while(define_index<OBC){
        temp_defined = mdefine_pointers[define_index];
        if (temp_defined.type==DATA_D){
            i=0;
            while (i<temp_defined.L){

                sscanf(temp_defined.content,"%s",temp_label);

                if (is_valid_integer(temp_label)==FALSE){
                    strncpy(temp_label, get_mdefine_value(temp_label),MAX_LABEL_LENGTH);
                }
                fprintf(temp_assembler, "%d" "%s" "%s\n" ,temp.address, "\t" ,  base4_encode(binary_from_char_imm(temp_label)));
                temp_defined.content = strstr(temp_defined.content,temp_label) + strlen(temp_label);
                i++;
            }
            goto end_of_loop_OBC;
        }
        if (temp_defined.type==STRING_D){
            string = malloc(sizeof(char)* MAX_LENGTH);
            if (string==NULL){
                goto end_of_loop_OBC;;
            }
            strncpy(string,temp_defined.content, MAX_LENGTH);

            string[MAX_LENGTH - 1] = '\0';
            while(i< strlen(string)){
                for_string = string[i];
                fprintf(temp_assembler, "%d" "%s" "%s\n" ,temp.address, "\t" ,  base4_encode(int_to_binary((int)for_string)));
                i++;
            }

        }

        end_of_loop_OBC:
        define_index++;
    }
    end:
    free(assembler_name_to_write);
    fclose(temp_assembler);
    free(new_line_last_words);
    free(string);
}


/************************************************************************
* Function Name: print_an_error
* Input: int address, int type_of_error
* Function Operation: This file contaions every errors that happend during reading the file.
************************************************************************/
void print_an_error(int address, int type_of_error) {
    FILE *error_output = fopen("errors_output.txt", "a");
    char *error_type = malloc(sizeof(char) * MAX_LENGTH);
    char *error_address = malloc(sizeof(int) * 10);

    if (error_type == NULL || error_address == NULL) {
        printf("Memory allocation failed!");
        fclose(error_output);
        free(error_type);
        free(error_address);
        return;
    }

    if (error_output == NULL) {
        perror("ERROR OF OPENING FILE");
        free(error_type);
        free(error_address);
        return;
    }

    switch (type_of_error) {
        case 1:
            strncpy(error_type, "error in label name in address: ", MAX_LENGTH);
        case 2:
            strncpy(error_type, "error in line reading in address:  ", MAX_LENGTH);
        case 3:
            strncpy(error_type, "Unknown register type in address:  ", MAX_LENGTH);
        case 4:
            strncpy(error_type, "Unknown action type in address: ", MAX_LENGTH);
        case 5:
            strncpy(error_type, "Label already defined in address: ", MAX_LENGTH);
        case 6:
            strncpy(error_type, "Symbol already defined in address: ", MAX_LENGTH);
        case 7:
            strncpy(error_type, "Undefined symbol in address: ", MAX_LENGTH);
        case 8:
            strncpy(error_type, "Error memory allocation Failed!", MAX_LENGTH);
        case 9:
            strncpy(error_type, "Error of addressing mode in address: ", MAX_LENGTH);
        case 10:
            strncpy(error_type, "Error of extras commas in address: ", MAX_LENGTH);
        case 11:
            strncpy(error_type, "Error of reading file", MAX_LENGTH);
            address = 00;
        case 12:
            strncpy(error_type, "False number of operands!", MAX_LENGTH);
        default:
            strncpy(error_type, "Unknown error in address: ", MAX_LENGTH);
    }

    sprintf(error_address, "%d", address);
    strcat(error_type, error_address);
    strcat(error_type, "\n");
    fprintf(error_output, "%s", error_type);

    fclose(error_output);
    free(error_type);
    free(error_address);
}


