/**
 * jiffies.c
 *
 * Kernel module that prints the current jiffies value
 * when cat /proc/jiffies is called
 *
 * fanh11@mcmaster.ca
 * Based on John Wiley & Sons - 2018. hello.c
 */

// For kernel programming
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
// For proc read
#include <linux/proc_fs.h>
// For jiffies
#include <linux/jiffies.h>

// Define a constant buffer size for
// the string to be printed
// when cat /proc/jiffies is called
#define BUFFER_SIZE 128
// Defines the name of the proc
#define PROC_NAME "jiffies"

/**
 * Function prototypes
 */
ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
};

/* This function is called when the module is loaded */
int proc_init(void)
{
    // creates the /proc/jiffies entry
    // the following function call is a wrapper for
    // proc_create_data() passing NULL as the last argument
    proc_create(PROC_NAME, 0, NULL, &proc_ops);

    // logs kernel informational message "/proc/jiffies created"
    // to show /proc/jiffies entry is created
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

/* This function is called when the module is removed */
void proc_exit(void)
{
    // removes the /proc/jiffies entry
    remove_proc_entry(PROC_NAME, NULL);

    // logs kernel informational message "/proc/jiffies removed"
    // to show /proc/jiffies entry is removed
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/**
 * This function is called each time the /proc/jiffies is read
 */
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    // use rv to store the number of characters printed
    int rv = 0;
    char buffer[BUFFER_SIZE];
    // use complete as boolean to prevent repeat reads
    static int complete = 0;

    // return 0 to prevent repeated reads if completed
    if (complete)
    {
        complete = 0;
        return 0;
    }

    // set to 1 after the proc is read
    complete = 1;

    // store the number of characters printed in rv
    // sprintf returns the number of characters
    // in buffer (excluding null padding characters)
    rv = sprintf(buffer, "jiffies: %lu\n", jiffies);

    // copies the contents of buffer to userspace usr_buf
    copy_to_user(usr_buf, buffer, rv);

    // return printed string length
    return rv;
}

/* Set the module entry and exit points */
module_init(proc_init);
module_exit(proc_exit);

/* Maintaining original license */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Jiffies Module");
MODULE_AUTHOR("RF");