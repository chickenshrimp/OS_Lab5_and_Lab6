#include <stdio.h>
#include <stdlib.h>

void printFromFile(FILE *file, int numLines) {
    int count = 0;
    int ch;
    
    while ((ch = getc(file)) != EOF) {
        putc(ch, stdout);
        if (ch == '\n') {
            count++;
            if (count == numLines) {
                break;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("You didn't enter filename\n");
        return 1;
    }
    
    int numLines = atoi(argv[2]);
    
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Failed to open file\n");
        return 1;
    }
    
    if (numLines == 0) {
        printFromFile(file, -1);
    }
    else {
        int ch;
        do {
            printFromFile(file, numLines);
            ch = getc(stdin);
        } while (ch != EOF);
    }
    
    if (fclose(file) == EOF) {
        printf("Failed to close file\n");
        return 1;
    }
    
    return 0;
}
