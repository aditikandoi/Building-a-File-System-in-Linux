#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include "async_ops.h"

static struct proc_dir_entry* async_ops_proc_entry;

static struct workqueue_struct *async_ops_wq;
static int global_job_identifier;

struct work_struct_wrapper{
    struct work_struct async_job;
    void *args;
    int jod_id;
};

struct work_struct_wrapper async_job_wrapper;

static void sample_func(struct work_struct *job)
{
    struct work_struct_wrapper *job_wrapper = container_of(job, struct work_struct_wrapper, async_job);
    struct var_args *args = (struct var_args*)job_wrapper->args;
    int iterator;
    // MAIN BODY
    for(iterator = 0; iterator < args->count; ++iterator) {
        printk("Line no:%d, File name:%s\n", __LINE__, args->filenames[iterator]);
        kfree(args->filenames[iterator]);
    }
    kfree(args);
    return;
}

static long async_ops_ioctl(struct file *file, unsigned int cmd,
				  unsigned long arg)
{
    long err = 0;
    int iterator;
    char *filename_str;
    struct filename *temp_filename;
    struct var_args *args;

    switch(cmd) {
        case IOCTL_DELETE:
                
                async_job_wrapper.args = kmalloc(sizeof(struct var_args), GFP_KERNEL);
                async_job_wrapper.jod_id = ++global_job_identifier;
                if(copy_from_user(async_job_wrapper.args, (void *)arg, sizeof(struct var_args))) {
                    return -EFAULT;
                }

                args = (struct var_args*)async_job_wrapper.args;

                for(iterator = 0; iterator < args->count; ++iterator) {
                    filename_str = kmalloc(PATH_MAX, GFP_KERNEL);
                    temp_filename = getname(args->filenames[iterator]);
                    strcpy(filename_str, temp_filename->name);
                    printk("File name:%s\n", temp_filename->name);
                    putname(temp_filename);
                    args->filenames[iterator] = filename_str;
                }

                INIT_WORK(&async_job_wrapper.async_job, sample_func);
                queue_work(async_ops_wq, &async_job_wrapper.async_job);
                return global_job_identifier;

            break;

        default:
           err = -EINVAL;
    }
	return err;
}

static struct file_operations file_ops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = async_ops_ioctl,
};

static int __init init_async_ops(void)
{
    async_ops_proc_entry = proc_create("async_ops_driver", 0666, NULL, &file_ops);
    async_ops_wq = create_workqueue("async_ops");

    printk("Installed async_ops_module\n");
	return 0;
}

static void __exit exit_async_ops(void)
{
    flush_workqueue(async_ops_wq);
    destroy_workqueue(async_ops_wq);
    proc_remove(async_ops_proc_entry);

    printk("Removed async_ops_module\n");
}

MODULE_AUTHOR(" Aditi, Swetang and Yash");
module_init(init_async_ops);
module_exit(exit_async_ops);
MODULE_LICENSE("GPL");