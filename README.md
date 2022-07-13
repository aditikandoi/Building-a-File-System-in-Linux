## CSE-506 (Spring 2022) Homework Assignment #2

NAME: Aditi Raj Kandoi

SBU ID: 114395814

EMAIL ID: akandoi@cs.stonybrook.edu

---

## Aim
To become familiar with the VFS layer of Linux, and especially with extensible file systems APIs.  To build a useful file system using stacking technologies. 

---

## FILES

- /usr/src/hw2-akandoi/fs/stbfs/main.c
- /usr/src/hw2-akandoi/fs/stbfs/file.c
- /usr/src/hw2-akandoi/fs/stbfs/inode.c
- /usr/src/hw2-akandoi/fs/stbfs/lookup.c
- /usr/src/hw2-akandoi/fs/stbfs/stbfs.h
- /usr/src/hw2-akandoi/fs/stbfs/utils.c
- /usr/src/hw2-akandoi/fs/stbfs/utils.h
- /usr/src/hw2-akandoi/CSE-506/stbfsctl.c
- /usr/src/hw2-akandoi/CSE-506/stbfsctl.h
---

## MOUNT
Below are the instructions to mount stbfs

- umount /mnt/stbfs
- rmmod stbfs
(- cd /usr/src/hw2-akandoi/fs/stbfs/ && make)
- (cd /usr/src/hw2-akandoi/CSE-506/ && make)
- sleep 1
- insmod /usr/src/hw2-akandoi/fs/stbfs/stbfs.ko
- mount -t stbfs parent_mnt mnt_stbfs

If the user wants to mount with an encryption key the last command is replaced by:
- mount -t stbfs -o enc=PASSWORD /usr/src/parent_mnt /mnt/stbfs 
where the length of the password should be equal to 32.

## SYSTEM DESIGN

#### A. UTILITY FUNCTION (utils.c)
The utils.c consists of the helper functions that are required to carry out several operations in the assignment.
1. test_skcipher_encdec() and test_skcipher() are the functions to carry out encryption and decryption. The reference for these functions is https://www.kernel.org/doc/html/v4.14/crypto/api-samples.html.

2. get_enc_mode() and get_dec_mode() are simpply used to change the flag for encryption and decryption.

3. compare_path() is used to check if the parent directly for a given file is '.stb'.

4. compare_uid_with_root() and compare_uids() are used to compared the current user id with root and with the file's owner respectively.

5. stbfs_delete_undelete() checks if the file has to be encrypted/decrypted/copy. It then reads the contents in the file as PAGE_SIZE and carries out operation until the remaining length becomes 0. 

6. stbfs_undelete() is called by the ioctl function. The function creates the output file name in the required format and passes the input and output files to the stbfs_delete_undelete() to carry out the required operation.

#### B. main.c
During mounting a file system, its parent path is provided which has been externed from main.c so that it can be used in the other important functionalities. Similarly, there is an optional field for providing an encryption key *ENC=* while mount. Based on whether the key is provided, an int variable *encryption_mode* has been assigned.
#### C. UNLINK (inode.c)

All the inode operations take place in inode.c. In the unlink function, two operations are carried out. First, if encryption mode is 1 (key is provided during mount), the text in the file that has to be deleted is encrypted and stored in the required trash bin ".stb". However, if the encryption mode is 0, then the file is simply deleted from its original path and stored in the trash bin. 
The content is copied or encrypted in *int stbfs_delete_undelete(struct file *input_file, struct file *output_file) function.* 

#### D. SETATTR (inode.c)
The files in the trash bin *.stb* can not be modified by any user apart from root. Hence, the function first compares the dir_dentry name with .stb and then checks whether the user who is trying to make changes in the directory is root or not. If not, then it does not allow and throws an error *EPERM*.

#### D. LIST CONTENTS OF THE TRASH BIN (file.c)
In order to protect the trash bin folder, no user other than root should be able to fully list the contents of the directory. Therefore, a temporary directory context has been initialised which consisted of the filtered actor. The filter actor method checks for all the files in the directory, if the user who has called the *ls* command, is indeed the owner of the file and lists only those. However, the contents of the directory are fully visible to the root user. 

#### E. USER PROGRAM IOCTL (stbfsctl.c)
Some users might want to recover/restore a file deleted by mistake. Hence, an ioctl has been created in the user level to receive the parameters for restoration. Here, it is an IOWR ioctl which sends data to the kernel (input file) which is defined as *#define IOCTL_UNDELETE_FILE _IOWR(MAJOR_NUM, 0, char *)* where MAJOR_NUM is set to 100. 

#### F. KERNEL PROGRAM IOCTL (file.c)
The parameters passed in the user space is received by the kernel in stbfs_unlocked_ioctl(). Here, it calls the function stbfs_undelete() which is defined in utils.c, which in turn helps to ove/copy the file to the user's CWD, decrypt it back (if it was encrypted), and rename it back to its original name.

#### G. LOOK UP (lookup.c)
As one of the objectives of the assignment is to protect the .stb trash bin, not every user should have access to all the contents in the directory. Therefore, I have used the utils function to compare the file requested to be looked up with the current user's ID. If it doesn't match, then the function throws an *ENOENT* err. In case the user is root, he can list all the contents in the bin.

#### H. Header files (stbfsctl.h and stbfs.h)
Several user level and kernel level headers are defined in stbfsctl.h and stbfs.h respectively.

---

#### I. EXTRA CREDIT- Secure Deletion
When files are just “unlinked”, their original data still remains on some disk blocks.  Hackers who gain access to the raw media can read all disk blocks, and if they identify blocks that originally belonged to the file, then they can recover some (or all) of the file.  Secure deletion is a method to overwrite data blocks before permanent deletion.

Support a new ioctl called “secdel” that takes a named file F.  If such a file exists in the .stb folder, and the user owns the file, then “securely delete” it as follows: first you overwrite the file with all zeros, from the very first byte till the last byte, and only then y​​ou can unlink the file.  Don’t just truncate the file, b/c we want the data blocks to have been overwritten on disk before the name is linked from the file system.


## VALID COMMAND FOR IOCTL
To restore a file from the trash bin .stb, a user must enter the valid command:
  * ./stbfsctl -u filename
filename - the entire path(.stb) of the file to be restored.

## TEST CASES

  * test01.sh - Test to check if deleted file are moved off to .stb folder and if files from .stb are permanently deleted.
  * test02.sh - Test to check if files are properly moved to .stb when mounted with (encryption) and without a key.
  * test03.sh - Test to check if key size passed for encryption and decryption during mount is correct.
  * test04.sh - Test to check if the files are decrypted and recovered properly when calling the ioctl command.
  * test05.sh - Test to check if the contents inside the .stb folder are visible only to the respective users.

   To run the test cases, use 'sh run_shell_scripts" on command line. This will run all the tests.

---

## REFERENCES
* https://tldp.org/LDP/lkmpg/2.4/html/x856.html
* https://stackoverflow.com/questions/68888917/get-current-working-directory-with-python-c-extension
* https://embetronicx.com/tutorials/linux/device-drivers/ioctl-tutorial-in-linux/
* https://blog.karthisoftek.com/a?ID=01450-8ef057e4-b21c-44b9-9947-73c63af46471
* https://www.guru99.com/file-permissions.html
* https://piazza.com/class/ky92q9drgdz7f7?cid=122
* https://www.linuxquestions.org/questions/slackware-14/bug-unable-to-handle-page-fault-4175696212/
* https://www.baeldung.com/linux/mount-unmount-filesystems
* https://www.linuxjournal.com/article/6485
* https://www.kernel.org/doc/html/v4.14/crypto/api-samples.html
