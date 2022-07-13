#include "stbfsctl.h"

void help(){
    printf(" USE THE SYNTAX\n ./stbfsctl -u filename\n");
}

int main(int argc, char *argv[]){
    struct file *input_file = NULL;
    // int input_file, pwd_file;
    char *input_filename;
    char *message = "Message passed by ioctl\n";
    int return_value = -1;
    int command_flag_char;

    input_filename = (char *) malloc(sizeof(char) * MAX_FILE_PATH_LEN);
    if(argc == 2){
        help();
        goto out;
    }

    while((command_flag_char = getopt(argc, argv, "u:h")) != -1){
        switch(command_flag_char){
            case 'u': 
                break;
            case 'h':
                help();
                goto out;
            default:
                printf("INCORRECT SYNTAX: use -h for help.\n");
                goto out;
        }
    }
    // printf("argv is %s", argv[optind]);
    // strcpy(input_filename, DEVICE_NAME);
    strcpy(input_filename, argv[optind - 1]);
    // printf("input_filename is %s\n", input_filename);


    input_file = open(input_filename, O_RDWR | O_CREAT);
    if(input_file < 0) {
        printf("WRAPFS is not mounted\n");
        goto out;
    }

    return_value = ioctl(input_file, IOCTL_UNDELETE_FILE, message);
    if(return_value  < 0){
        printf ("ioctl_set_msg failed:%d\n", return_value);
        goto out;

    }
out: 
    if(input_filename != NULL)
        free(input_filename);
    if(input_file != NULL)
        close(input_file);
    return return_value;

}