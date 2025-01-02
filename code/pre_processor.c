#include "pre_processor.h"
#include "first_move.h"

macro* macros; /* Define a global pointer to an array of macros */
int macro_count = 0; /* Initialize the global macro count to 0 */

/************************************************************************
* Function Name: main
* Output: int TRUE OR FALSE
* Function Operation: Pre Processor assembler
*
************************************************************************/
int first_move_writing(char* assembler_name) {
    FILE *file_to_read; /* File pointer to read the assembler file */
    FILE *file_to_write; /* File pointer to write the processed assembler file */
    int temp_limit_for_macro_lines = 10; /* Temporary limit for macro lines */
    int macro_capacity = 10; /* Initial capacity for the macros array */
    int mcrFlag = FALSE; /* Flag to indicate if currently inside a macro definition */
    int j, i, t; /* Loop counters */
    char mcr[4] = "mcr"; /* Macro keyword */
    char *first_word; /* Pointer to store the first word of each line */
    char *check_first_word; /* Pointer to check the first word after 'mcr' */
    char *line_to_read; /* Pointer to store each line read from the file */
    /*char* line_to_write_name;  Pointer to store the name of the output file */
    char* assembler_name_to_write; /* Pointer to store the name of the output file */
    char* assembler_name_to_read; /* Pointer to store the name of the input file */
    first_word = (char*)malloc(sizeof(char) * MAX_LENGTH); /* Allocate memory for first_word */
    check_first_word = (char*)malloc(sizeof(char) * MAX_LENGTH); /* Allocate memory for check_first_word */
    line_to_read = (char*)malloc(sizeof(char) * MAX_LENGTH); /* Allocate memory for line_to_read */
    assembler_name_to_write = (char*)malloc(sizeof(char) * MAX_LENGTH);  /* Allocate memory for assembler_name_to_write */
    assembler_name_to_read = (char*)malloc(sizeof(char) * MAX_LENGTH);  /* Allocate memory for assembler_name_to_read */

    if (first_word == NULL || check_first_word == NULL || line_to_read == NULL || assembler_name_to_write == NULL || assembler_name_to_read == NULL) {

        return FALSE; /* Check if memory allocation was successful */
    }

    strcpy(assembler_name_to_read, assembler_name);
    strcat(assembler_name_to_read, ".as");
    file_to_read = fopen(assembler_name_to_read, "r"); /* Open the assembler file for reading */

    if (file_to_read == NULL) {
        perror("Error opening input file\n");
        free(first_word);
        free(check_first_word);
        free(line_to_read);
        free(assembler_name); /* If file cannot be opened, free allocated memory and return */
        return FALSE;
    }

    strcpy(assembler_name_to_write, assembler_name);
    strcat(assembler_name_to_write , ".am");
    file_to_write = fopen(assembler_name_to_write, "w"); /* Open the output file for writing */

    if (file_to_write == NULL) {
        perror("Error opening output file");
        fclose(file_to_read);
        free(first_word);
        free(check_first_word);
        free(line_to_read);
        free(assembler_name); /* If output file cannot be opened, free allocated memory and close input file */
        return FALSE;
    }

    macros = (macro*)malloc(macro_capacity * sizeof(macro)); /* Allocate memory for the macros array */
    if (macros == NULL) {

        fclose(file_to_write);
        fclose(file_to_read);
        free(first_word);
        free(check_first_word);
        free(line_to_read);
        free(assembler_name);
        return FALSE;
    }

    while (fgets(line_to_read, MAX_LENGTH, file_to_read) != NULL) {


        if (strcmp(line_to_read, "\n") == 0 || strchr(line_to_read, ';') != NULL) {
            continue; /* Skip empty lines or comments */
        }
        sscanf(line_to_read, "%s", first_word); /* Read the first word of the line */
        if (mcrFlag == TRUE && !strstr(line_to_read, "endmcr")) {

            if (macros[macro_count].lineSum == temp_limit_for_macro_lines) {
                temp_limit_for_macro_lines *= 2;
                macros[macro_count].commands = (char**)realloc(macros[macro_count].commands, macros[macro_count].lineSum*temp_limit_for_macro_lines * sizeof(char*));
                if (macros[macro_count].commands == NULL) {

                    return FALSE; /* If inside a macro definition, check if more memory is needed and reallocate */
                }
            }
            macros[macro_count].commands[macros[macro_count].lineSum] = (char*)malloc(sizeof(char) * MAX_LENGTH);
            if (macros[macro_count].commands[macros[macro_count].lineSum] == NULL) {

            }
            strcpy(macros[macro_count].commands[macros[macro_count].lineSum], line_to_read);
            macros[macro_count].lineSum++;
            continue; /* Copy the line into the current macro's commands */
        }

        if (strcmp(first_word, mcr) == 0) {

            mcrFlag = TRUE;
            sscanf(line_to_read + strlen(mcr), "%s", check_first_word);

            if (macro_count == macro_capacity) {

                macro_capacity *= 2;
                macros = (macro*)realloc(macros, (macro_count+macro_capacity) * sizeof(macro));
                if (macros == NULL) {

                    return FALSE; /* If a new macro starts, check if more memory is needed and reallocate */
                }
            }

            macros[macro_count].name = (char*)malloc(strlen(check_first_word) + 1);
            if (macros[macro_count].name == NULL) {

                return FALSE;
            }
/* Initialize the new macro's name and commands array */
            strcpy(macros[macro_count].name, check_first_word);
            macros[macro_count].lineSum = 0;
            macros[macro_count].commands = (char**)malloc(temp_limit_for_macro_lines * sizeof(char*));

            if (macros[macro_count].commands == NULL) {

                return FALSE;
            }

        }

        if (strstr(line_to_read, "endmcr")) {

            mcrFlag = FALSE;
            macro_count++; /* If "endmcr" is found, end the current macro definition */
        }
        if (if_macro_exist(first_word) == TRUE && !strstr(line_to_read, "mcr")) {
            int temp_index = get_macro_index(first_word);
            macro temp;

            temp = macros[temp_index];


            for (j = 0; j < temp.lineSum; j++) {
                char* temp_line = macros[temp_index].commands[j];

                fprintf(file_to_write, "%s", temp_line); /* If a macro is found, write its commands to the output file */
            }
            continue;
        } else {
            if (strstr(line_to_read, "mcr") || strstr(line_to_read, "endmcr")) {
                continue; /* Skip lines containing "mcr" or "endmcr" */
            }
            fprintf(file_to_write, "%s", line_to_read); /* Write the line to the output file if not inside a macro */
        }
    }
    fclose(file_to_write); /* Close the output file */
    fclose(file_to_read); /* Close the input file */

    for (i = 0; i < macro_count; i++) {
        free(macros[i].name);
        for (t = 0; t < macros[i].lineSum; t++) {
            free(macros[i].commands[t]);
        }
        free(macros[i].commands); /* Free all allocated memory for macros */
    }

    free(first_word);
    free(check_first_word);
    free(line_to_read); /* Free all allocated memory */
    return TRUE;
}

int if_macro_exist(char* label) {
    int i = 0;
    while (i < macro_count) {
        if (strcmp(label, macros[i].name) == 0) {
            return TRUE; /* Check if a macro with the given label exists */
        }
        i++;
    }
    return FALSE;
}

int get_macro_index(char* label) {
    int i = 0;
    while (i < macro_count) {
        if (strcmp(label, macros[i].name) == 0) {
            return i; /* Get the index of the macro with the given label */
        }
        i++;
    }
    return FALSE;
}