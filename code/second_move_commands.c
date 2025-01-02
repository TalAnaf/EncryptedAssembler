
#include "second_move_commands.h"


/************************************************************************
* Function Name: int_to_binary
* Input: int num
* Output: char
* Function Operation: gets a number and convert it to binary
************************************************************************/
char* int_to_binary(int num) {
    /* Number of bits in an integer */
    int i;
    char* binary_rep = (char*)malloc(BITS_SIZE + 1);
    if (binary_rep == NULL) {
        return NULL;
    }
    binary_rep[BITS_SIZE] = '\0'; /* Null terminator*/

    for ( i = BITS_SIZE - 1; i >= 0; i--) {
        binary_rep[i] = (num & 1) ? '1' : '0';
        num >>= 1;
    }
    binary_rep[BITS_SIZE] = '\0';

    return binary_rep;
}

char* binary_from_char_imm(char* imm){
    int number = strtol(imm, NULL, 10);
    return int_to_binary(number);
}

/************************************************************************
* Function Name: base4_encode
* Input: char *base4_address
* Output: char
* Function Operation: The function translates from binary to secret base 4
************************************************************************/
char* base4_encode(char *base4_address) {
    int i;
    int length = strlen(base4_address);
    char *temp = malloc(sizeof(char) * 3);
    char *encoded_address = malloc(sizeof(char) * MAX_LENGTH);

    if (encoded_address == NULL || temp == NULL) {
        return NULL;
    }
    encoded_address[0] = '\0';

    for (i = 0; i < length; i += 2) {
        temp[0] = base4_address[i];
        temp[1] = base4_address[i + 1];
        temp[2] = '\0';
        if (strcmp(temp, "00") == 0) {
            strcat(encoded_address, "*");
        }
        else if (strcmp(temp, "01") == 0) {
            strcat(encoded_address, "#");
        }
        else if (strcmp(temp, "10") == 0) {
            strcat(encoded_address, "%");
        }
        else if (strcmp(temp, "11") == 0) {
            strcat(encoded_address, "!");
        }
        else{

        }

    }
    free(temp);
    /* need to find all the places that refer to this function and free the memory of encoded_address */
    return encoded_address;
}

/************************************************************************
* Function Name: get_address_of_register
* Input: char* temp_r, int symbol
* Output: char*
* Function Operation: The function
************************************************************************/
char* get_address_of_register(char* temp_r, int symbol) {
    /* Allocate memory */
    int rt_symbol = 2;
    int rd_symbol = 1;
    long number;
    int i;
    char* binary = malloc(BITS_SIZE + 1);
    number = strtol(temp_r, NULL, 10);
    /* Check for valid input range*/
    if (number < 0 || number > 7 || (symbol != 1 && symbol != 2)) {

        return NULL;
    }

    if (binary == NULL) {
        printf("Memory allocation failed!");
        return NULL;
    }

    /* Initialize the binary string with '0'*/
    memset(binary, '0', BITS_SIZE);
    binary[BITS_SIZE] = '\0';  /*Null-terminate the string*/

    for (i = 0; i < 3; i++) {
        if (symbol == rd_symbol) {
            binary[13 - (5 + i)] = (number & (1 << i)) ? '1' : '0';
        }
        else if (symbol == rt_symbol) {
            binary[13 - (2 + i)] = (number & (1 << i)) ? '1' : '0';
        }
    }

    return binary;
}

/************************************************************************
* Function Name: two_operand_register_address
* Input: char* temp_rd,char* temp_rt
* Output: char*
* Function Operation: The function returns the address in case of two registers
 * in the same command
************************************************************************/
char* two_operand_register_address(char* temp_rd,char* temp_rt) {
    char* binary;
    long temp_rt_num;
    long temp_rd_num;
    int i;
    temp_rt++;
    temp_rd++;
    temp_rd_num = strtol(temp_rd,NULL,10);
    temp_rt_num = strtol(temp_rt,NULL,10);

    /* Check for valid input range */
    if ((temp_rd_num < 0 || temp_rd_num > 7) || (temp_rt_num < 0 || temp_rt_num > 7)) {

        return NULL;
    }
    /* Allocate memory for the 14-bit binary string plus the null terminator */
    binary = malloc(BITS_SIZE + 1);
    if (binary == NULL) {
        printf("Memory allocation failed!");
        return NULL;
    }

    /* Initialize the binary string with '0's */
    memset(binary, '0', BITS_SIZE);
    binary[BITS_SIZE] = '\0';  /* Null-terminate the string */

    /* Set the bits for num1 in the 5-7 positions */
    for (i = 0; i < 3; i++) {
        binary[13 - (5 + i)] = (temp_rt_num & (1 << i)) ? '1' : '0';
    }

    /* Set the bits for num2 in the 2-4 positions */
    for (i = 0; i < 3; i++) {
        binary[13 - (2 + i)] = (temp_rd_num & (1 << i)) ? '1' : '0';
    }

    return binary;
}

/************************************************************************
* Function Name: analysis_addressing_mode
* input: line temp, int symbol
* Output: char*
* Function Operation: This function analysis the line binary
 * words according to the addresing mode of the first word.
 * She analysis base on the symbol of the rd or rt.
 *
************************************************************************/
char* analysis_addressing_mode(line temp, int symbol) {
    int symbol_rd = 1;
    int symbol_rt = 2;
    char *result = malloc(BIT_SIZE_FOR_OPERAND * sizeof(char)+1);
    char* label = malloc(sizeof(char)*MAX_LABEL_LENGTH);
    char* imm = malloc(sizeof(char)*MAX_LABEL_LENGTH);
    char* temp_address_for_define = malloc(sizeof(char)*BITS_SIZE+1);
    char* temp_address_for_imm = malloc(sizeof(char)*BITS_SIZE+1);
    char* temp_address_for_register = malloc(sizeof(char)*BITS_SIZE+1);
    char *endstr = malloc(sizeof(char) * MAX_LABEL_LENGTH);
    char* temp_r = malloc(sizeof(char) * MAX_LABEL_LENGTH + 1 );
    char* temp_addressing_r = malloc(sizeof(char )*MAX_REG_ADDRESSING_MODE);
    long imm_test=FALSE;
    if (temp_address_for_register==NULL||label==NULL||imm==NULL||temp_r==NULL||result == NULL || endstr == NULL||temp_addressing_r==NULL||temp_address_for_imm==NULL||temp_address_for_define==NULL) {
        goto end;
    }

    if (symbol==symbol_rd) {
        strncpy(temp_addressing_r, temp.Addressing_modes_rd, MAX_REG_ADDRESSING_MODE);
        strncpy(temp_r, temp.rd, MAX_LABEL_LENGTH + 1);
    }
    if (symbol==symbol_rt){
        strncpy(temp_addressing_r, temp.Addressing_modes_rt, MAX_REG_ADDRESSING_MODE);
        strncpy(temp_r, temp.rt, MAX_LABEL_LENGTH + 1);
    }


    if (strcmp(temp_addressing_r, "00")==0 && strcmp(temp_r,"NO OPERAND FLAG")==0){
        strncpy(result,"NO ADDRESS",BITS_SIZE+1);
        goto end;
    }
    if (strcmp(temp_addressing_r, "00")==0)
    {
        strncpy(temp_r, temp.rd,MAX_LABEL_LENGTH);

        temp_r++;
        /*immieniate addressing*/
        imm_test = strtol(temp_r, &endstr, 10);

        if (*endstr == '\0') {

            strncpy(temp_address_for_imm,int_to_binary(imm_test),(BITS_SIZE+1));
            temp_address_for_imm[BITS_SIZE] = '\0';

            strncpy(result,temp_address_for_imm,(BITS_SIZE+1));
            strcat(result , get_binary_A_R_E(temp_r)); /**** my addition ****/
            goto end;
        }
        else {
            strncpy(result, binary_from_char_imm(get_mdefine_value(temp_r)),BITS_SIZE+1);
            strcat(result , get_binary_A_R_E(temp_r)); /**** my addition ****/
            goto end;
        }
    }
    if (strcmp(temp_addressing_r, "01")==0) {

        strncpy(result, binary_from_char_imm(get_mdefine_value(temp_r)),BITS_SIZE+1);

        strcat(result , get_binary_A_R_E(temp_r));
        goto end;}
    if (strcmp(temp_addressing_r, "10")==0) {
        ;/* Permit index addressing*/
        /* Using sscanf to get the data index and the label*/
        sscanf(temp_r, "%[^[][%[^]]]", label, imm);

        strncpy(temp_address_for_define, get_data_address(label), (BITS_SIZE + 1));

        if (is_valid_integer(imm) == FALSE) {
            strncpy(temp_address_for_imm, binary_from_char_imm(get_define(imm)), BITS_SIZE + 1);
        }
        else {
            strncpy(temp_address_for_imm, binary_from_char_imm(imm), BITS_SIZE + 1);
        }

        strncpy(result, temp_address_for_define, BITS_SIZE + 1);
        strcat(result, "\n");
        strcat(result,temp_address_for_imm);
    }
    if (strcmp(temp_addressing_r, "11")==0) {
        /*Direct register addressing*/
        if (strcmp(temp.Addressing_modes_rd,temp.Addressing_modes_rt)==0){
            strncpy(temp_address_for_register,two_operand_register_address(temp.rd,  temp.rt),BITS_SIZE+1);

            result = malloc(BITS_SIZE * sizeof(char)+1);
            if (result==NULL) goto end;
            strncpy(result,temp_address_for_register,BITS_SIZE);
            result[BITS_SIZE] = '\0';
        }
        else{
            strncpy(temp_address_for_register,get_address_of_register(temp_r,symbol),BITS_SIZE+1);

            result = malloc(BITS_SIZE * sizeof(char)+1);
            if (result==NULL) goto end;
            strncpy(result,temp_address_for_register,BITS_SIZE);
            result[BITS_SIZE] = '\0';
        }
    }

    end:
    free(temp_addressing_r);
    free(temp_address_for_register);
    free(temp_address_for_imm);
    free(temp_address_for_define);
    return result;
}
