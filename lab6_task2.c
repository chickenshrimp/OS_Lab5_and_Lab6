#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_PATH 1024

int process_count = 0; 
int max_processes;     

void compare_files(const char *file1, const char *file2) {
    pid_t pid = getpid();
    int fd1, fd2;
    off_t size1, size2;

    fd1 = open(file1, O_RDONLY);
    fd2 = open(file2, O_RDONLY);

    size1 = lseek(fd1, 0, SEEK_END);
    size2 = lseek(fd2, 0, SEEK_END);

    if (size1 != size2) {
        printf("PID: %d, Files '%s' and '%s' have different sizes\n", pid, file1, file2);
    } else {
        char *buf1 = (char *)malloc(size1);
        char *buf2 = (char *)malloc(size2);

        lseek(fd1, 0, SEEK_SET);
        lseek(fd2, 0, SEEK_SET);
        read(fd1, buf1, size1);
        read(fd2, buf2, size2);

        if (memcmp(buf1, buf2, size1) == 0) {
            printf("PID: %d, Files '%s' and '%s' are identical\n", pid, file1, file2);
        } else {
            printf("PID: %d, Files '%s' and '%s' are different\n", pid, file1, file2);
        }

        // Освобождение памяти
        free(buf1);
        free(buf2);
    }

    // Закрытие файлов
    close(fd1);
    close(fd2);
}

void traverse_directories(const char *dir1, const char *dir2) {
    struct dirent *entry;
    struct stat statbuf;
    DIR *dir;
    char path1[MAX_PATH];
    char path2[MAX_PATH];

    dir = opendir(dir1);
    if (dir == NULL) {
        perror("Unable to open directory");
        return;
    }

    // Обход файлов в первом каталоге
    while ((entry = readdir(dir)) != NULL) {
        snprintf(path1, MAX_PATH, "%s/%s", dir1, entry->d_name);
        snprintf(path2, MAX_PATH, "%s/%s", dir2, entry->d_name);

        // Получение информации о файле
        if (lstat(path1, &statbuf) == -1) {
            perror("Unable to get file info");
            continue;
        }

        // Рекурсивный вызов, если найден подкаталог
        if (S_ISDIR(statbuf.st_mode)) {
            // Пропускаем текущий и родительский каталоги
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            traverse_directories(path1, path2);
        } else {
            // Запуск процесса для пары файлов
            if (process_count < max_processes) {
                pid_t pid = fork();
                if (pid == 0) {
                    compare_files(path1, path2);
                    exit(0);
                } else if (pid > 0) {
                    process_count++;
                } else {
                    perror("Fork failed");
                }
            } else {
                // Ожидание завершения одного из процессов, чтобы освободить слот
                wait(NULL);
            }
        }
    }

    closedir(dir);
}

int main() {
    char dir1[MAX_PATH];
    char dir2[MAX_PATH];

    printf("Enter the first directory: ");
    scanf("%s", dir1);
    printf("Enter the second directory: ");
    scanf("%s", dir2);
    printf("Enter the max number of processes: ");
    scanf("%d", &max_processes);

    traverse_directories(dir1, dir2);

    while (process_count > 0) {
        wait(NULL);
        process_count--;
    }

    return 0;
}
        
