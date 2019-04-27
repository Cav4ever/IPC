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
    size_t pagesize, filesize;
    struct stat buf;
    void *ptr;
    pid_t pid = getpid();

    pagesize = sysconf(_SC_PAGESIZE);
    printf ("The sizeof of the page is %ld\n", (long)pagesize);
    filesize = pagesize;

    fd = shm_open(FILENAME, O_RDWR | O_CREAT | O_EXCL, 0644);
    if (-1 == fd) {
        perror("Failed in shm_open:");
        exit(1);
    }
    if (fstat(fd, &buf) == -1) {
        perror("Failed in fstat:");
        exit(1);
    }
    printf ("The original sizeof of the shm object is %ld\n", buf.st_size);
    if (ftruncate(fd, filesize) == -1) {
        perror("Failed in ftruncate:");
        exit(1);
    }
    if (fstat(fd, &buf) == -1) {
        perror("Failed in fstat:");
        exit(1);
    }
    printf ("After ftruncate sizeof of the shm object is %ld\n", buf.st_size);

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
    shm_data->data = 23;
    shm_data->pointer = &shm_data->data;
    printf("pid is %ld\n", pid);
    printf("The address of shm_data is %p\n", shm_data);
    printf("shm_data->data is %d\n", shm_data->data);
    printf("shm_data->pointer is %p\n", shm_data->pointer);

    if(munmap(ptr, buf.st_size) == -1) {
        perror("Failed in munmap:");
    }
    return 0;
}