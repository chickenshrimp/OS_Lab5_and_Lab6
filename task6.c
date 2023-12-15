#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

struct FileInfo {
    char name[256];
    long size;
};

void calculateDirectorySize(const char *dirPath, FILE *outputFile) {
    DIR *dir;
    struct dirent *entry;
    struct FileInfo largestFile;
    
    dir = opendir(dirPath);
    if (dir == NULL) {
        perror("Failed to open catalog");
        exit(EXIT_FAILURE);
    }
    
    largestFile.size = -1;
    
    while ((entry = readdir(dir)) != NULL) {
        char path[512];
        snprintf(path, sizeof(path), "%s/%s", dirPath, entry->d_name);
        
        //если подкаталог -- вызываем рекурсию
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0
          && strcmp(entry->d_name, "..") != 0) {
            calculateDirectorySize(path, outputFile);
        }
        
        else if (entry->d_type == DT_REG) {
            FILE *file = fopen(path, "rb");
            if (file == NULL) {
                perror("Failed opening file");
                exit(EXIT_FAILURE);
            }
            
            fseek(file, 0, SEEK_END); //размер файла
            long size = ftell(file);
            fclose(file);
            
            if(size > largestFile.size) {
                largestFile.size = size;
                strncpy(largestFile.name, path, sizeof(largestFile.name));
            }
        }
    }
    
    if (closedir(dir) == -1) {
        perror("Failed closing catalog");
        exit(EXIT_FAILURE);
    }
    
    fprintf(outputFile, "Catalog: %s\n", dirPath);
    fprintf(outputFile, "Number of files: %ld\n", largestFile.size > 0
      ? largestFile.size : 0);
    fprintf(outputFile, "Summary memory of files: %ld bytes\n", largestFile.size > 0
      ? largestFile.size : 0 ? largestFile.size : 0);
    fprintf(outputFile, "Name of largest file: %s\n", largestFile.size > 0
      ? largestFile.name : "No files");
    fprintf(outputFile, "\n");
    printf("Catalog: %s\n", dirPath);
    printf("Number of files: %ld\n", largestFile.size > 0 ? largestFile.size : 0);
    printf("Summary memory of files: %ld bytes\n", largestFile.size > 0
      ? largestFile.size : 0);
    printf("Name of largest file: %s\n", largestFile.size > 0
      ? largestFile.name : "No files");
    printf("\n");
 }
 
 int main(int argc, char *argv[]) {
     if (argc < 3) {
         printf("Expected more arguments\n");
         return 1;
     }
     
     FILE *outputFile = fopen(argv[2], "w");
     if (outputFile == NULL) {
         perror("Failed opening file");
         return 1;
     }
     
     calculateDirectorySize(argv[1], outputFile);
     
     fclose(outputFile);
     
     return 0;
 }
