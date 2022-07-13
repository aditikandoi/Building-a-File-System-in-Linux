#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "async_ops.h"

int get_unlink_args(int argc, const char *argv[], struct var_args *args)
{
    int iterator;
    if(argc == 2) {
        printf("Missing operand after %s delete\n", argv[0]);
        return -1;
    } else if (argc > 6) {
        printf("Too many operands. Upto 6 input files allowed\n");
        return -1;
    }

    args->count = argc - 2;
    for(iterator = 2; iterator < argc; ++iterator) {
        args->filenames[iterator - 2] = (char *)malloc(strlen(argv[iterator])+1);
        strcpy(args->filenames[iterator - 2], argv[iterator]);
    }

    return 0;
}

int main(int argc, const char *argv[])
{
    int rc = 0;
    int async_ops_driver;
    int iterator;

    async_ops_driver = open("/proc/async_ops_driver", O_RDWR);

    if(async_ops_driver < 0) {
        printf("Cannot open the driver\n");
        rc = -1;
        goto out;
    }

    if(strcmp("delete", argv[1]) == 0) {
        struct var_args args;
        rc = get_unlink_args(argc, argv, &args);
        rc = 0;
        if(rc < 0)
            exit(rc);
        rc = ioctl(async_ops_driver, IOCTL_DELETE, &args);
        for(iterator = 0; iterator < args.count; ++iterator) {
            free(args.filenames[iterator]);
        }
    }else{
        printf("Invalid operation\n");
    }
   
out:

    if(rc < 0) {
        perror("Error:");
    }else if(rc > 0) {
        printf("Id of submitted job:%d\n", rc);
    }

    close(async_ops_driver); 

	exit(rc);
}
