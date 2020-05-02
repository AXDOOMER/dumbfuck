// Dumb brainfuck interpreter in C
// https://en.wikipedia.org/wiki/Brainfuck
// https://esolangs.org/wiki/Brainfuck

#include <stdio.h>

#define TAPE_SIZE 30000

struct Data {
	char tape[TAPE_SIZE];	// The program's memory
	char* cell;	// Pointer to a cell
	FILE* code;	// File descriptor, points to code in the text file
};

int read_char(char* ch, FILE* code) {
	*ch = fgetc(code);
	return feof(code);
}

void interpret(struct Data* prog, int stack_height) {

	while (1) {
		int loop_start;
		int loop_depth;
		char ch;

		if (read_char(&ch, prog->code))
			return;

		switch(ch) {
			case '>':
				++prog->cell;
				if (prog->cell == prog->tape + TAPE_SIZE) {
					prog->cell -= TAPE_SIZE;
				}
				break;
			case '<':
				--prog->cell;
				if (prog->cell < prog->tape) {
					prog->cell += TAPE_SIZE;
				}
				break;
			case '+':
				++*prog->cell;
				break;
			case '-':
				--*prog->cell;
				break;
			case '.':
				putchar(*prog->cell);
				break;
			case ',':
				*prog->cell = getchar();
				break;
			case '[': // Call the interpreter again to handle the inner loop
				loop_start = ftell(prog->code);
				loop_depth = 0;

				// Handle loop when condition is false
				if (!*prog->cell) {
					while (1) {
						if (read_char(&ch, prog->code))
							return;

						if (ch == ']' && loop_depth == 0) {
							break;
						} else if (ch == '[') {
							++loop_depth;
						} else if (ch == ']') {
							--loop_depth;
						}
					}
				}

				// Handle loop when condition is true
				while (*prog->cell) {
					// Set code pointer to beginning of loop
					fseek(prog->code, loop_start, SEEK_SET);
					interpret(prog, stack_height + 1);
				}

				break;
			case ']':
				return;

		}//end switch
	}//end loop
}

int main(int argc, char* argv[]) {
	struct Data prog = { .tape = {0}, .cell = prog.tape, .code = NULL};

	if (argc > 1) {
		prog.code = fopen(argv[1], "r");

		if (prog.code == NULL) {
			perror("Error opening file");
			return 1;
		}

		interpret(&prog, 0);

		fclose(prog.code);
	}

	return 0;
}
