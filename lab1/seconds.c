#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <asm/param.h>

#define BUFFER_SIZE 120
#define PROC_NAME "seconds"

/**
 * Function prototypes
 */
ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
};

int proc_init(void){
    struct proc_dir_entry *pde = proc_create(PROC_NAME, 0, NULL, &proc_ops)
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

void proc_exit(void){
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

float seconds_elapsed(void){
    float secs = 0.0;

    secs = (float)jiffies/HZ;
    return secs;
}

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos){
    int rv = 0;
    char buffer[BUFFER_SIZE]
    static int complete = 0;

    if(complete){
        complete = 0;
        return 0;
    }

    complete = 1;
    
    rv = sprintf(buffer, "time elapsed: %lf seconds\n", seconds_elapsed());

    copy_to_user(usr_buf, buffer, rv);

    return rv;
}

module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Seconds Module");
MODULE_AUTHOR("SGG");

