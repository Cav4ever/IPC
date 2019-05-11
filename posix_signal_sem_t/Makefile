.PHONY: all clean
all: clean creat_write read write unlink
creat_write: shm_creat_write.c
	gcc -g -lrt -pthread $^ -o $@
read: shm_read.c
	gcc -g -lrt -pthread $^ -o $@
write: shm_write.c
	gcc -g -lrt -pthread $^ -o $@
unlink: shm_unlink.c
	gcc -g -lrt -pthread $^ -o $@
clean:
	-rm -f creat_write read write unlink
