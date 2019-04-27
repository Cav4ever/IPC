#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define FILENAME "/my_shared_memmory"

int main(int argc, char **argv)
{
    if (-1 == shm_unlink(FILENAME)) {
        perror("Failed in shm_unlink:");
        exit(1);
    }
    printf ("shm_unlink %s success\n", FILENAME);

    return 0;
}
