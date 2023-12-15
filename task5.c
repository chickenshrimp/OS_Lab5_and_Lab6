#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

void listDirectory(const char *dirPath) {
    DIR *dir;
    struct dirent *entry;
    
    dir = opendir(dirPath);
    if (dir == NULL) {
        perror("Failed opening catalog");
        exit(EXIT_FAILURE);
    }
    
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name); //вывод содержимого каталога
    }
    
    if (closedir(dir) == -1) {
        perror("Failed closing catalog");
        exit(EXIT_FAILURE);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        printf("Input catalog contains of:\n");
        listDirectory(argv[1]);
    }

    printf("Current catalog contains of:\n");
    listDirectory(".");
    
    return 0;
}

