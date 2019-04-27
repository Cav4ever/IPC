#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>

#define FILENAME "/my_shared_memmory"
#define SEMNAME "/my_semaphore"

int main(int argc, char **argv)
{
    if (-1 == shm_unlink(FILENAME)) {
        perror("Failed in shm_unlink:");
        exit(1);
    }
    printf ("shm_unlink %s success\n", FILENAME);
    if (-1 == sem_unlink(SEMNAME)) {
        perror("Failed in sem_unlink:");
        exit(1);
    }
    printf ("sem_unlink %s success\n", SEMNAME);

    return 0;
}
