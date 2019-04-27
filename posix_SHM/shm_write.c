#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>

typedef struct ShmData_S_ {
    int data;
    int *pointer;
} ShmData_T;

#define FILENAME "/my_shared_memmory"
#define SEMNAME "/my_semaphore"

int main(int argc, char **argv)
{
    int fd, number;
    ShmData_T *shm_data;
    struct stat buf;
    void *ptr;
    char *stop = NULL;
    sem_t *mutex;
    pid_t pid = getpid();

    if (2 != argc) {
        printf("usage: ./write number\n");
        exit(1);
    }
    number = strtoul(argv[1], &stop, 0);
    if ('\0' != *stop || errno) {
        printf("usage: ./write number\n");
        exit(1);
    }

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
    if (sem_wait(mutex) == -1) {
        perror("Failed in sem_wait:");
        exit(1);
    }

    printf("pid is %ld\n", pid);
    printf("The address of shm_data is %p\n", shm_data);
    printf("shm_data->data is %d\n", shm_data->data);
    printf("shm_data->pointer is %p\n", shm_data->pointer);
    shm_data->data = number;
    shm_data->pointer = &shm_data->data;
    printf("shm_data->data is %d\n", shm_data->data);
    printf("shm_data->pointer is %p\n", shm_data->pointer);

    if (sem_post(mutex) == -1) {
        perror("Failed in sem_post:");
        exit(1);
    }
    if (sem_close(mutex) == -1) {
        perror("Failed in sem_close:");
        exit(1);
    }
    if(munmap(ptr, buf.st_size) == -1) {
        perror("Failed in munmap:");
    }
    return 0;
}
