// Dumb brainfuck interpreter in C
// https://en.wikipedia.org/wiki/Brainfuck

#include <stdio.h>

struct Data {
	char  array[30000];
	char* ptr;
	FILE* code;
};

void interpret(struct Data* prog, int stack_height) {

	while (1) {
		int loop_start;	//current_pos
		char ch = fgetc(prog->code);

		if (feof(prog->code)) {
			// Done. Note that if the program is malformed (i.e. not an
			// equal number of '[' and ']', this won't behave correctly.
			return;
		}

		// +[-[<<[+[--->]-[<<<]]]>>>-]>-.---.>..>.<<<<-.<+.>>>>>.>.<<.<-.
		//printf("	sh: %d		ch: %c		pt: %u\n", stack_height, ch, prog->ptr);

		switch(ch) {
			case '>': ++prog->ptr;
				break;
			case '<': --prog->ptr;
				if (prog->ptr < prog->array) {
					puts("Warning: Negative data pointer!");
				}
				break;
			case '+': ++*prog->ptr;
				break;
			case '-': --*prog->ptr;
				break;
			case '.': putchar(*prog->ptr);
				break;
			case ',': *prog->ptr = getchar();
				break;
			case '[': // Call the interpreter again to handle the inner loop
				loop_start = ftell(prog->code);

				// Handle loop condition false
				if (!*prog->ptr)
					while(fgetc(prog->code) != ']');

				// Handle loop condition true
				while (*prog->ptr) {
					// Set code pointer to beginning of loop
					fseek(prog->code, loop_start, SEEK_SET);
					interpret(prog, stack_height + 1);
				}

				break;
			case ']': return;
				break;
			default: // Everything else is NOP
				break;
		}//end switch
	}//end loop
}

int main(int argc, char* argv[]) {

	struct Data prog = { .array = {0}, .ptr = prog.array, .code = NULL};

	if (argc > 1) {

		prog.code = fopen(argv[1], "r");

		if (prog.code == NULL) {
			perror("Error opening file");
			return 1;
		}

		interpret(&prog, 0);

		fclose(prog.code);
	}

	// Newline
	puts("");

	return 0;
}
