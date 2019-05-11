#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>
#include <signal.h>

typedef struct ShmData_S_ {
    int data;
    int *pointer;
} ShmData_T;

#define FILENAME "/my_shared_memmory"
#define SEMNAME "/my_semaphore"

void sigHanlder(int);

void *ptr;
sem_t *mutex;
struct stat buf;

int main(int argc, char **argv)
{
    int fd;
    ShmData_T *shm_data;
    pid_t pid = getpid();

    fd = shm_open(FILENAME, O_RDWR, 0);
    if (-1 == fd) {
        perror("Failed in shm_open:");
        exit(1);
    }
    if (fstat(fd, &buf) == -1) {
        perror("Failed in fstat:");
        exit(1);
    }
    ptr = mmap(NULL, buf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (MAP_FAILED == ptr) {
        perror("Failed in mmap:");
        exit(1);
    }
    if (close(fd) == -1) {
        perror("Failed in close:");
        exit(1);
    }

    shm_data = (ShmData_T *)ptr;
    mutex = sem_open(SEMNAME, 0);
    if (SEM_FAILED == mutex) {
        perror("Failed in sem_open:");
        exit(1);
    }
    signal(SIGINT, sigHanlder);
    while (1) {
        if (sem_wait(mutex) == -1) {
            perror("Failed in sem_wait:");
            exit(1);
        }

        printf("pid %ld: %d\n", pid, (shm_data->data)++);
        if (sem_post(mutex) == -1) {
            perror("Failed in sem_post:");
            exit(1);
        }
        sleep(1);
    }

    return 0;
}


void sigHanlder(int sigNum)
{
    if (SIGINT == sigNum) {
        if (sem_close(mutex) == -1) {
            perror("Failed in sem_close:");
        }
        if(munmap(ptr, buf.st_size) == -1) {
            perror("Failed in munmap:");
        }
        exit(1);
    }
}