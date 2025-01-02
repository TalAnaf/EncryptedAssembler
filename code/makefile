CFLAGS = -Wall -ansi -pedantic -fsanitize=address -g
LDFLAGS = -fsanitize=address -lm

assembler: main.o pre_processor.o first_move.o get_from_memory.o data_commands.o second_move_commands.o output_of_files.o valid_check.o
	gcc $(LDFLAGS) main.o pre_processor.o first_move.o get_from_memory.o data_commands.o second_move_commands.o output_of_files.o valid_check.o -o assembler

main.o: main.c main.h
	gcc $(CFLAGS) -c main.c -o main.o

pre_processor.o: pre_processor.c pre_processor.h
	gcc $(CFLAGS) -c pre_processor.c -o pre_processor.o

first_move.o: first_move.c first_move.h
	gcc $(CFLAGS) -c first_move.c -o first_move.o

get_from_memory.o: get_from_memory.c get_from_memory.h
	gcc $(CFLAGS) -c get_from_memory.c -o get_from_memory.o

data_commands.o: data_commands.c data_commands.h
	gcc $(CFLAGS) -c data_commands.c -o data_commands.o

second_move_commands.o: second_move_commands.c second_move_commands.h
	gcc $(CFLAGS) -c second_move_commands.c -o second_move_commands.o

output_of_files.o: output_of_files.c output_of_files.h
	gcc $(CFLAGS) -c output_of_files.c -o output_of_files.o

valid_check.o: valid_check.c valid_check.h
	gcc $(CFLAGS) -c valid_check.c -o valid_check.o
