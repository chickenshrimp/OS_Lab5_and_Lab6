#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>


void printCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
  
    time_t my_time = time(NULL);
    struct tm *now = localtime(&my_time);
  
    printf("%d:%d:%d:%ld\n", now->tm_hour, now->tm_min, now->tm_sec, tv.tv_usec / 1000);
}

int main() {
    
    printf("Parent PID: %d\n", getpid());
    pid_t child1, child2;
    child1 = fork();
  
    if (child1 > 0) {
        child2 = fork();
        if (child2 == 0) {
            printf("Child 2 PID's: %d, Parent's PID's: %d, Current time: ", getpid(), getppid());
            printCurrentTime();
            exit(0);
        
        }
      
        else if (child2 < 0) {
            printf("Fork failed\n");
            return 1;
        }
        
        else if (child2 > 0) {
            system("ps -x | grep 'task1'");
        }
    }
    
    else if(child1 == 0){
        printf("Child 1 PID's: %d, Parent's PID's: %d, Current time: ", getpid(), getppid());
        printCurrentTime();
        exit(0);
    }
    else{
        printf("Fork failed\n");
    }
  
    return 0;
}
