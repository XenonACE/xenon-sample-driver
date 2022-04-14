#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#include <linux/proc_fs.h>
#include <linux/slab.h>

#define DRIVERNAME "[XE STATS] "

#define DRIVER_MAX_USER_SIZE 1024

static struct proc_dir_entry *button_proc = NULL;

static char data_buffer[DRIVER_MAX_USER_SIZE];

ssize_t button_read(struct file *file, char __user *user, size_t size, loff_t *off) {
	//copy_to_user(user, "Hello!\n", 7);
	//return 7;
	
	char simplebuff[] = "This device stats:\nModel: Xenon 1.0\nOS: ACE v0.1.0\nBranch: unstable";
	size_t len = strlen(simplebuff);

	return simple_read_from_buffer(user, size, off, simplebuff, len);
}

ssize_t button_write(struct file *file, const char __user *user, size_t size, loff_t *off) {
	memset(data_buffer, 0x0, sizeof(data_buffer));

	if (size > DRIVER_MAX_USER_SIZE) {
		size = DRIVER_MAX_USER_SIZE;
	}

	copy_from_user(data_buffer, user, size);

	printk(DRIVERNAME "You said '%s'!\n", data_buffer);
	return size;
}

static const struct proc_ops buttons_proc_fops = {
	.proc_read = button_read,
	.proc_write = button_write,
};

static int __init driver_init(void) {
	printk(DRIVERNAME "Initializing Driver!\n");

	// Create an entry in the proc-fs
	button_proc = proc_create("xenon-stats", 0666, NULL, &buttons_proc_fops);
	if (button_proc == NULL) {
		return -1;
	}

	return 0;
}

static void __exit driver_exit(void) {
	printk(DRIVERNAME "Cleaning up driver!\n");
	proc_remove(button_proc);
	return;
}

module_init(driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("77Z");
MODULE_DESCRIPTION("Static description about this phone");
MODULE_VERSION("1.0");
