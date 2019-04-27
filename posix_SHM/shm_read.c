#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef struct ShmData_S_ {
    int data;
    int *pointer;
} ShmData_T;

#define FILENAME "/my_shared_memmory"

int main(int argc, char **argv)
{
    int fd;
    ShmData_T *shm_data;
    struct stat buf;
    void *ptr;
    pid_t pid = getpid();


    fd = shm_open(FILENAME, O_RDONLY, 0);
    if (-1 == fd) {
        perror("Failed in shm_open:");
        exit(1);
    }
    if (fstat(fd, &buf) == -1) {
        perror("Failed in fstat:");
        exit(1);
    }
    ptr = mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (MAP_FAILED == ptr) {
        perror("Failed in mmap:");
        exit(1);
    }
    if (close(fd) == -1) {
        perror("Failed in close:");
        exit(1);
    }

    shm_data = (ShmData_T *)ptr;
    printf("pid is %ld\n", pid);
    printf("The address of shm_data is %p\n", shm_data);
    printf("shm_data->data is %d\n", shm_data->data);
    printf("shm_data->pointer is %p\n", shm_data->pointer);

    if(munmap(ptr, buf.st_size) == -1) {
        perror("Failed in munmap:");
    }
    return 0;
}
