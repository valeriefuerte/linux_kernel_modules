#include <linux/init.h>           
#include <linux/module.h>         
#include <linux/device.h>         
#include <linux/kernel.h>         
#include <linux/fs.h>             
#include <asm/uaccess.h>  

#define MYDEV_NAME "solution"
#define CLASS_NAME "test_chardev_class"
#define KBUF_SIZE 250

static char *node_name = "node_name";
module_param(node_name, charp, 0000);
MODULE_PARM_DESC(node_name, "A node name");

static struct class*  chardevClass  = NULL; 
static struct device* chardevDevice = NULL; 

static int my_major = -1;
static char message[KBUF_SIZE];  
static char* messagePtr;

static int solution_open(struct inode *inode, struct file *file)
{
   printk(KERN_INFO "Opening\n");
   messagePtr = message;
   
   return 0;
}

static int solution_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Releasing \n");
    return 0;
}

static ssize_t solution_read( struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{   
    int bytes_read = 0;

    if (*messagePtr == 0)
        return 0;

    while (lbuf && *messagePtr) {
        put_user(*(messagePtr++), buf++);
        lbuf--;
        bytes_read++;
    }

    return bytes_read;
}

static ssize_t solution_write( struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos)
{
    printk(KERN_INFO "Writing: Received %zu characters from the user\n", lbuf);
    return lbuf;
}

static const struct file_operations mycdev_fops = {
    .owner = THIS_MODULE,
    .read = solution_read,
    .write = solution_write,
    .open = solution_open,
    .release = solution_release
};

static int __init init_sulution(void)
{
   my_major = register_chrdev(0, node_name, &mycdev_fops);
   if (my_major < 0){
      printk(KERN_ALERT "DummyChardev: failed to register existing device\n");
      return my_major;
   }

   chardevClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(chardevClass)){                // Check for error and clean up if there is
      unregister_chrdev(my_major, node_name);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(chardevClass);          // Correct way to return an error on a pointer
   }
 
   printk(KERN_INFO "DummyChardev: existing device registered successfuly\n");
   // Register the device driver
   chardevDevice = device_create(chardevClass, NULL, MKDEV(my_major, 0), NULL, node_name);
   if (IS_ERR(chardevDevice)){               // Clean up if there is an error
      class_destroy(chardevClass);           // Repeated code but the alternative is goto statements
      unregister_chrdev(my_major, node_name);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(chardevDevice);
   }

   sprintf(message, "%d\n", my_major);
   return 0;
}

static void __exit cleanup_sulution(void)
{
   device_destroy(chardevClass, MKDEV(my_major, 0));     // remove the device
   class_unregister(chardevClass);                          // unregister the device class
   class_destroy(chardevClass);
   unregister_chrdev(my_major, node_name);             // unregister the major number
   printk(KERN_INFO "DummyChardev: rmmod\n");
}


module_init(init_sulution);
module_exit(cleanup_sulution);

MODULE_LICENSE("GPL");
