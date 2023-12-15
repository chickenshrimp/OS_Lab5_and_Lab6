#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void printCurrentTime() {
    time_t now = time(0);
    struct tm *local_time = localtime(&now);
    char timeString[40];
    strftime(timeString, sizeof(timeString), "%H:%M:%S", local_time);
    
    printf("%s\n", timeString);
}

int main() {
    pid_t child1, child2;
    
    printf("Parent PID: %d\n", getpid());
    
    child1 = fork();
    
    if (child1 < 0) {
        printf("Fork failed\n");
        return 1;
    }
    else if (child1 == 0) {
        printf("Child 1 PID's: %d, Parent's PID's: %d, Current time: ", getpid(), getppid());
        printCurrentTime();
        exit(0);
    }
    else {
        child2 = fork();
        
        if (child2 < 0) {
            printf("Fork failed\n");
            return 1;
        }
        else if (child2 == 0) {
            printf("Child 2 PID's: %d, Parent's PID's: %d, Current time: ", getpid(), getppid());
            printCurrentTime();
            exit(0);
        }
        else {
            int status;
            waitpid(child1, &status, 0);
            waitpid(child2, &status, 0);
            
            printf("Running 'ps -x' using system()\n");
            system("ps -x");
        }
    }
    
    return 0;
}
