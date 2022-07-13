#include <asm/unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <errno.h>
#include<unistd.h>
#include<stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <getopt.h>
#include <openssl/md5.h>
#include<openssl/evp.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <fcntl.h> 

#define MAJOR_NUM 100 
#define DEVICE_NAME "/usr/src/mnt_stbfs/.stb/" 
#define IOCTL_UNDELETE_FILE _IOWR(MAJOR_NUM, 0, char *)
#define MAX_FILE_PATH_LEN 500