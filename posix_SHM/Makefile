.PHONY: all clean
all: clean creat_write read write unlink
creat_write: shm_creat_write.c
	gcc -g -lrt $^ -o $@
read: shm_read.c
	gcc -g -lrt $^ -o $@
write: shm_write.c
	gcc -g -lrt $^ -o $@
unlink: shm_unlink.c
	gcc -g -lrt $^ -o $@
clean:
	-rm -f creat_write read write unlink
