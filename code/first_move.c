#include "first_move.h"
/* Global variable definitions */
int IC = 0; /* IC - Instructions count */
int line_count = 1;
char reg_array[NUMBER_OF_REGISTER][REGISTER_LENGTH] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "PSW"};
line* line_pointers;
Mdefine* mdefine_pointers;

/************************************************************************
* Function Name: first_step
* Function Operation: The function extracts lines from the file and sends them for analysis
************************************************************************/
void first_step(char* assembler_name)
{
    int num_of_L;
    char *current_line = malloc(MAX_LENGTH * sizeof(char));
    int current_address = FIRST_ADDRESS_IN_MEMORY;
    char* assembler_name_to_read; /* Pointer to store the name of the input file */
    FILE* assembler;

    line_pointers =  malloc(sizeof(line)*line_count); /*Pointers for lines*/
    assembler_name_to_read = (char*)malloc(sizeof(char) * MAX_LENGTH);  /* Allocate memory for assembler_name_to_read */

    if (current_line == NULL || line_pointers == NULL || assembler_name_to_read == NULL ){
        printf("Dynamic allocation FAILED! \n");
        return;
    }

    strcpy(assembler_name_to_read, assembler_name);
    strcat(assembler_name_to_read , ".am");
    assembler = fopen(assembler_name_to_read,"r");
    if (assembler==NULL){
        perror("ERROR OF OPENING FILE IN FIRST_MOVE \n");
    }

    /* Process lines from the file */
    while (fgets(current_line, MAX_LENGTH, assembler) != NULL)
    {

        /*Reallocing for new line*/
        line_pointers = realloc(line_pointers, sizeof(line)*(line_count+2));
        if (line_pointers == NULL) {
            printf("Dynamic allocation FAILED! \n");
        }

        /* Every time the loop is entered, a new line is received and the counter increases by one */
        /* Check if the line is not a new line or a comment */
        /*strlen(strchr(current_line, 59)) == 1 NEED TO BE IN BUT DOES LEAKING MEMEORY PROBLEM*/
        if (strcmp(current_line, "\n") == 0  )
        {
            current_address++; /* Increment current address */
            continue;
        }
        /* Checks for comma problem
        if (check_commas(current_line)==FALSE){
            print_an_error(current_address,10);
            continue;
        }*/
        remove_commas(current_line);
        /* Calling line_analysis function and checking if the line is correct */
        num_of_L = line_analysis(current_line,current_address);
        if( num_of_L == FALSE)
        {
            print_an_error(current_address,2);
            continue;
        }

        current_address+=num_of_L; /* Increment current address */
    } /* End of while */

    fclose(assembler);
    free(current_line);
    output(assembler_name);
} /* End of first_step function */

/************************************************************************
* Function Name: line_analysis
* Input: char *current_line, int current_address
* Output: int
* Function Operation: The function checks whether the line starts with a label or a definition
************************************************************************/
int line_analysis(char *current_line, int current_address) {
    /*char *line_command = malloc(sizeof(char) * MAX_LENGTH);*/
    char *label_part = malloc(sizeof(char) * (MAX_LABEL_LENGTH));
    char *temp_line = malloc(sizeof(char) * MAX_LENGTH);


    if (temp_line == NULL ||label_part == NULL ){
        printf("Memory allocation failed!");
        free(temp_line);
        free(label_part);
        return FALSE;
    }

    strncpy(temp_line, current_line, MAX_LENGTH - 1);

    temp_line[MAX_LENGTH - 1] = '\0';
    sscanf(temp_line,"%s",label_part);

    if (strchr(label_part,'.') != NULL || (strchr(label_part,':') != NULL)) {
        return define_new_mdefine(current_line,current_address,label_part);
    }

    return define_regular_line(current_line,current_address);
}

/************************************************************************
* Function Name: define_new_mdefine
* Input: char* current_line,int current_address,char* label_part
* Output: int
* Function Operation: The function defines a new mdefine - checks what kind of statement it is
 *                     and inserts it in the array of mdefine with the help of the if_define function
************************************************************************/
int define_new_mdefine(char* current_line,int current_address,char* label_part) {
    int error_flag = TRUE;
    char *check_label = malloc(sizeof(char) * MAX_LABEL_LENGTH);
    char *temp_line = malloc(sizeof(char) * MAX_LENGTH);
    char *current_word = malloc(sizeof(char) * MAX_LABEL_LENGTH);
    char *define_part = malloc(sizeof(char) * (MAX_LENGTH));
    char *command_part = malloc(sizeof(char) * (MAX_LENGTH));
    char *not_label_part = malloc(sizeof(char) * (MAX_LENGTH));
    Mdefine new_mdefine;

    new_mdefine.complete_line = malloc(sizeof(char) * MAX_LENGTH);
    new_mdefine.label = malloc(sizeof(char) * (MAX_LABEL_LENGTH));
    new_mdefine.content = malloc(sizeof(char) * MAX_LENGTH);
    new_mdefine.address = current_address;

    if (new_mdefine.label == NULL || new_mdefine.content == NULL || new_mdefine.complete_line == NULL) {
        printf("dynamic allocation FAILED! \n");
        return FALSE;
    }
    if (strcmp(".entry", label_part) == 0) {

        new_mdefine.type = ENTRY_D;
        current_line = current_line + strlen(label_part);
        strncpy(new_mdefine.content, current_line,MAX_LENGTH);
        mdefine_count++;
        error_flag = to_entry(new_mdefine);
        goto cleanup;
    }
    else if (strcmp(".extern", label_part) == 0) {

        new_mdefine.type = EXTERN_D;
        current_line = strstr(current_line,".extern") + strlen(label_part);
        strncpy(new_mdefine.content, current_line,MAX_LENGTH);
        error_flag = to_extern(new_mdefine);
        goto cleanup;
    }
    else if (strcmp(".define", label_part) == 0) {

        new_mdefine.type = DEFINE_D;
        strcpy(define_part,current_line);
        define_part =  strstr(current_line,".define") + strlen(".define");

        strncpy(new_mdefine.content, define_part, MAX_LENGTH);
        mdefine_count++;
        error_flag = to_define(new_mdefine);
        goto cleanup;
    }
    strncpy(new_mdefine.complete_line, current_line, sizeof(char) * MAX_LENGTH);
    strncpy(new_mdefine.label, remove_colon(label_part),MAX_LABEL_LENGTH);

    strncpy(new_mdefine.content, remove_colon(rest_of_string(current_line, label_part)), sizeof(char) * MAX_LENGTH);

    error_flag = if_define(new_mdefine);
    if (error_flag == FALSE) {

        print_an_error(current_address, 5);
        goto cleanup;
    }
    strcpy(temp_line, new_mdefine.content);
    strcpy(command_part, temp_line);
    sscanf(command_part, "%s", command_part);

    temp_line = strchr(temp_line + strlen(command_part), ' ');

    if (temp_line != NULL){
        strcpy(new_mdefine.content, temp_line);

        strcpy(current_word, command_part);

    }
    else{
        new_mdefine.content = NULL;
    }
    if (strcmp(".data", command_part) == 0) {

        new_mdefine.type = DATA_D;
        mdefine_count++;
        error_flag = to_data(new_mdefine);
        goto cleanup;
    }
    else if (strcmp(".string", command_part) == 0) {

        new_mdefine.type = STRING_D;

        mdefine_count++;
        error_flag = to_string(new_mdefine);
        goto cleanup;

    }
    else {
        new_mdefine.type = REUGLAR_LINE;

        current_line = strstr(current_line, command_part);

        define_regular_line(current_line, new_mdefine.address);
        new_mdefine.regular_line = line_pointers[line_count-1];


        new_mdefine.content= malloc(sizeof(char)*MAX_LENGTH);
        if (new_mdefine.content==NULL){
            return FALSE;
        }
        strncpy(new_mdefine.content,current_line,MAX_LENGTH);
        error_flag = new_mdefine.regular_line.L;
        enter_to_mdefine_pointer(new_mdefine);
        goto cleanup;
        cleanup:
        free(check_label);
        free(label_part);
        free(not_label_part);
        return error_flag;
    }
}

/************************************************************************
* Function Name: define_regular_line
* Input: char char* current_line,int current_address
* Output: int
* Function Operation: The function defines a line type and inserts it into the line array
 *                     and converts the first word of the line to binary
************************************************************************/
int define_regular_line(char* current_line,int current_address){
    line new_line;
    char* command_part = malloc(sizeof(char)*MAX_COMMAND_LENGTH);
    new_line.address = current_address;
    new_line.complete_line = malloc(sizeof(char)*MAX_LENGTH);
    new_line.command = malloc(sizeof(char) * MAX_LABEL_LENGTH);
    new_line.rd = malloc(sizeof(char) * MAX_LABEL_LENGTH);
    new_line.rt = malloc(sizeof(char) * MAX_LABEL_LENGTH);
    new_line.Addressing_modes_rd = malloc(sizeof(char) * LENGTH_OF_ADDRESING_MODE);
    new_line.Addressing_modes_rt = malloc(sizeof(char) * LENGTH_OF_ADDRESING_MODE);
    if (new_line.Addressing_modes_rt==NULL||new_line.Addressing_modes_rd==NULL||new_line.command == NULL|| new_line.complete_line ==NULL||command_part==NULL||new_line.rd==NULL||new_line.rt==NULL) {
        printf("Memory allocation failed!");
        return FALSE;
    }

    strncpy(new_line.complete_line,current_line,MAX_LENGTH);
    sscanf(current_line, "%s", command_part);
    strncpy(new_line.command ,command_part, MAX_COMMAND_LENGTH);

    strcpy(command_part,new_line.command);
    new_line.command_point = valid_check_for_commands(command_part);
    if (new_line.command_point ==FALSE){
        print_an_error(current_address,4);
        free(command_part);
        return FALSE;
    }

    new_line.L = calculate_L_of_line(new_line.complete_line);

    line_pointers[line_count] = new_line;
    line_count++;
    getting_line_type(line_count - 1);

    convert_binary_first_word(line_count);
    free(command_part);
    IC++;
    return new_line.L;
}

/************************************************************************
* Function Name: getting_line_type
* Input: int new_line_index
* Output: int
* Function Operation: The function checks to which command group the row belongs
************************************************************************/
int getting_line_type(int new_line_index) {
    line new_line = line_pointers[new_line_index];
    int error_flag = TRUE;
    int check = FALSE;
    char *temp_command = malloc(sizeof(char) * 5);

    if(temp_command==NULL){
        printf("Dynamic allocation FAILED!");
        return FALSE;
    }
    /*Getting the command for temp using*/
    strcpy(temp_command, new_line.command);
    /* Cutting the command from the line*/
    check = what_action_group(new_line.command_point );


    /* commands with two operands */
    if (check==1) {
        new_line.command_group = 1;
        error_flag  = first_group_command(&new_line);
        goto end;
    }
        /* commands with one operand */
    else if (check==2) {
        new_line.command_group = 2;
        error_flag = second_group_command(&new_line);
        goto end;
    }
        /*commands with no operand*/
    else if (check==3){
        new_line.command_group = 3;
        error_flag = third_group_command(&new_line);
        goto end;
    }
    end:
    line_pointers[new_line_index] = new_line;
    free(temp_command);
    return error_flag;
}/* End of getting_line_type */

/************************************************************************
* Function Name:  what_action_group
* Input: int check
* Output: int
* Function Operation: The function associates a command word with its command group
************************************************************************/
int what_action_group(int check) {

    switch (check) {
        /* For - lea ,sub ,add ,cmp ,mov*/
        case 0:
        case 1:
        case 2:
        case 3:
        case 6:
            return 1;
            /*For - not, clr, inc, dec, jmp, bne, red, prn, jsr*/
        case 4:
        case 5:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            return 2;
            /*In case of hlt and rts commands that NO OFFENDERS NEEDED*/
        default:
            return 3;
    }
}
/************************************************************************
* Function Name: first_group_command
* Input: line new_line
* Output: int
* Function Operation: The function converts the operands of the first command group set to binary
************************************************************************/
int first_group_command(line* new_line) {
    /* Declarations */
    int error_flag = TRUE;
    char *temp_line;
    char *token;
    char* addressing_mode_result;

    temp_line = (char*)malloc(sizeof(char) * MAX_LABEL_LENGTH);
    token =  (char*)malloc(sizeof(char) * MAX_LABEL_LENGTH);
    addressing_mode_result = (char*)malloc(sizeof(char) * 3);

    if (token==NULL||temp_line == NULL||addressing_mode_result==NULL) {
        printf("Memory allocation failed!");
        error_flag=FALSE;
        goto end;
    }
    /* Copy the complete line to a temporary variable */
    strcpy(temp_line, new_line->complete_line);

    /* Tokenize the line to extract the command, rt, and rd */
    if (sscanf(temp_line, "%s %s %s", token, new_line->rt,new_line->rd) != 3) {
        error_flag=FALSE;
        goto end;
    }

    strncpy(token,new_line->rt, MAX_LABEL_LENGTH);
    strncpy(addressing_mode_result, what_addressing_mode(token), 3);
    strncpy(new_line->Addressing_modes_rt,addressing_mode_result, 3 );

    strncpy(token,new_line->rd, MAX_LABEL_LENGTH);
    strncpy(addressing_mode_result, what_addressing_mode(token), 3);
    strncpy(new_line->Addressing_modes_rd,addressing_mode_result, 3);


    end:
    free(token);
    free(temp_line);
    free(addressing_mode_result);

    return error_flag;
}


/************************************************************************
* Function Name: second_group_command
* Input: line new_line
* Output: int
* Function Operation: The function converts the operands of the second command group set to binary
************************************************************************/
int second_group_command(line* new_line) {
    /* Declarations */
    int error_flag = TRUE;
    char *temp_line;
    char *token;
    char* addressing_mode_result;
    char* no_operand = malloc(sizeof(char)*MAX_LABEL_LENGTH);

    temp_line = (char*)malloc(sizeof(char) * MAX_LABEL_LENGTH);
    token =  (char*)malloc(sizeof(char) * MAX_LABEL_LENGTH);
    addressing_mode_result = (char*)malloc(sizeof(char) * 3);
    new_line->rd = (char*)malloc(sizeof(char) * MAX_LABEL_LENGTH);
    new_line->rt = (char*)malloc(sizeof(char) * MAX_LABEL_LENGTH);
    if (no_operand==NULL||temp_line == NULL||token==NULL||addressing_mode_result==NULL||new_line->rd==NULL||new_line->rt==NULL) {
        printf("Memory allocation failed!");
        free(temp_line);
        free(token);
        free(addressing_mode_result);
        return FALSE;
    }
    strcpy(no_operand,"NO OPERAND FLAG");
    strcpy(new_line->rt,no_operand);
    /* Copy the complete line to a temporary variable */
    strcpy(temp_line, new_line->complete_line);

    /* Tokenize the line to extract the command, rt, and rd */
    if (sscanf(temp_line, "%s %s ", token ,new_line->rd) != 2) {
        return FALSE;
    }


    strcpy(token,new_line->rd);
    strcpy(addressing_mode_result, what_addressing_mode(token));

    strncpy(new_line->Addressing_modes_rd,addressing_mode_result,LENGTH_OF_ADDRESING_MODE);

    strncpy(new_line->Addressing_modes_rt,"00",LENGTH_OF_ADDRESING_MODE);

    free(temp_line);
    free(addressing_mode_result);

    return error_flag;
}

/************************************************************************
* Function Name: third_group_command
* Input: int new_line
* Output: int
* Function Operation: The function converts the operands of the third command group set to binary
************************************************************************/
int third_group_command(line* new_line){
    int error_flag=TRUE;
    char* no_operand = malloc(sizeof(char)*MAX_LABEL_LENGTH);
    if(no_operand == NULL){
        printf("Memory allocation failed!");
        error_flag= FALSE;
        goto end;
    }

    strcpy(no_operand,"NO OPERAND FLAG");
    new_line->rt = malloc(sizeof(char)* MAX_LABEL_LENGTH+ 1);
    new_line->rd = malloc(sizeof(char)* MAX_LABEL_LENGTH + 1);
    if (new_line->rt==NULL||new_line->rd==NULL||count_operands((new_line->command)) != 0 ){
        error_flag = FALSE;
        goto end;
    }
    strncpy(new_line->rt,no_operand, MAX_LABEL_LENGTH);
    strncpy(new_line->rd,no_operand, MAX_LABEL_LENGTH);
    strncpy(new_line->Addressing_modes_rd,"00", LENGTH_OF_ADDRESING_MODE);

    strncpy(new_line->Addressing_modes_rt,"00", LENGTH_OF_ADDRESING_MODE);

    end:

    return error_flag;
}
/************************************************************************
* Function Name: what_addressing_mode
* Input: char *temp
* Output: char
* Function Operation: The function gets the addressing mode of the line
************************************************************************/
char* what_addressing_mode(char *temp) {
    char* result = malloc(sizeof(char) * 3);
    int error_flag = TRUE;
    char *test = malloc(sizeof(char) * MAX_LABEL_LENGTH);
    char* data_index = malloc(sizeof(char)*MAX_LABEL_LENGTH);


    if (test == NULL || result == NULL || temp == NULL||data_index==NULL) {
        printf("Memory allocation failed! \n");
        free(test);
        free(result);
        return NULL;
    }
    strncpy(test, temp, MAX_LABEL_LENGTH);
    test[MAX_LABEL_LENGTH - 1] = '\0';

    if (strcmp(temp,"NO OPERAND FLAG")==0){

        strncpy(result, "00", 3);
        goto end;
    }
    if (temp[0] == '#') {

        strncpy(result, "00", 3);
        goto end;
    }


    if (sscanf(temp, "%30[^[][%30[^]]", test, data_index) == 2) {

        strncpy(result, "10", 3);
        goto end;
    }
    /* Checking validity of register */
    error_flag = valid_check_reg(temp);
    if (error_flag != FALSE) {
        strncpy(result, "11", 3);
        goto end;
    } else {
        strncpy(result, "01" ,3);
        goto end;
    }

    end:
    free(test);
    free(data_index);

    return result;
}

/************************************************************************
* Function Name: convert_binary_first_word
* Input: int line_pointer
* Function Operation: The function converts the first word of each line to binary
************************************************************************/
void convert_binary_first_word(int line_pointer) {
    /* Declarations */
    char* binary_first_word = malloc(sizeof(char)*MAX_LABEL_LENGTH);
    line temp;
    char *always_zero;
    char *always_zero_first_word;
    char *content;
    char *temp_rt;
    char *temp_rd;
    char *temp_command_point;

    always_zero_first_word = malloc(4 * sizeof(char));
    always_zero = malloc(5 * sizeof(char));
    temp_rt = malloc(4 * sizeof(char));
    temp_rd = malloc(4 * sizeof(char));
    temp_command_point = malloc(5 * sizeof(char));
    content = malloc(MAX_LENGTH * sizeof(char));
    if (!always_zero || !always_zero_first_word || !content ||
        !temp_rt || !temp_rd || !temp_command_point||!binary_first_word) {
        printf("Allocation failed!\n");
        goto cleanup;
    }

    /* Getting the line from the pointer */
    temp = line_pointers[line_pointer - 1];
    if (temp.command == NULL) {
        goto cleanup;
    }

    strncpy(content, temp.complete_line, MAX_LENGTH);
    content[MAX_LENGTH - 1] = '\0';

    strncpy(temp_command_point, get_binary_action(temp.command_point), 5);

    strncpy(always_zero, "0000", 5);
    strncpy(always_zero_first_word, "00", 3);

    strcpy(binary_first_word, always_zero);

    strcat(binary_first_word, temp_command_point);

    strcat(binary_first_word, temp.Addressing_modes_rt);

    strcat(binary_first_word, temp.Addressing_modes_rd);

    strcat(binary_first_word, always_zero_first_word);

    strcpy(temp.binary_first_word, binary_first_word);

    line_pointers[line_pointer - 1] = temp ;


    cleanup:
    /* Freeing the allocated memory */
    free(content);
    free(always_zero_first_word);
    free(always_zero);
    free(temp_rt);
    free(temp_rd);
    free(temp_command_point);
}


/************************************************************************
* Function Name: calculate_L_of_line
* Input: const char* line
* Output: int
* Function Operation: The function calculates the number of L of the line
************************************************************************/
int calculate_L_of_line(const char *line) {
    int word_count = 0;
    char prev_char = ' ';
    int i ;

    for ( i = 0; line[i] != '\0'; i++) {
        if (line[i] != ' ' && (prev_char == ' ' || prev_char == '\t' || prev_char == '\n')) {
            word_count++;
        }
        prev_char = line[i];
    }

    return word_count;
}