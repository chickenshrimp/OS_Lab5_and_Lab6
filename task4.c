#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void copyFile(const char *srcFile, const char *destFile) {
    FILE *src, *dest;
    int c;
    
    src = fopen(srcFile, "r");
    if (src == NULL) {
        perror("Failed opening input file");
        exit(EXIT_FAILURE);
    }
    
    dest = fopen(destFile, "w");
    if (dest == NULL) {
        perror("Failed opening destination file");
        exit(EXIT_FAILURE);
    }
    
    while ((c = fgetc(src)) != EOF) {
        if (fputc(c, dest) == EOF) {
            perror("Failed writing into destination file");
            exit(EXIT_FAILURE);
        }
    }
    
    if (fclose(src) == EOF) {
        perror("Failed closing input file");
        exit(EXIT_FAILURE);
    }
    
    if (fclose(dest) == EOF) {
        perror("Failed closing destination file");
        exit(EXIT_FAILURE);
    }
    
    //копирование прав доступа исх. файла на файл записи
    struct stat src_stat;
    if (stat(srcFile, &src_stat) == -1) {
        perror("Failed to get access rights of file");
        exit(EXIT_FAILURE);
    }
    
    if (chmod(destFile, src_stat.st_mode) == -1) {
        perror("Failed to copy access rights");
        exit(EXIT_FAILURE);
    }
    
    printf("File has been successfully copied\n");

}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("You entered less arguments\n");
        return 1;
    }
    
    char *srcFile = argv[1];
    char *destFile = argv[2];
    
    copyFile(srcFile, destFile);
    
    return 0;
}
