#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/cdev.h>


#define MYDEV_NAME "solution"
#define BUFFER_SIZE 250

// IOCTL definitions
#define IOC_MAGIC 'k'

#define SUM_LENGTH _IOWR(IOC_MAGIC, 1, char*)
#define SUM_CONTENT _IOWR(IOC_MAGIC, 2, char*)
#define MAX_STRING_SIZE 20

static size_t  openCount=0;
static size_t  bytes_written=0;
static int  majorNumber=240;   
static char message[BUFFER_SIZE];  
static char* messagePtr;

static int sum_content = 0;
static int sum_length = 0;

static int solution_open(struct inode *inode, struct file *file){
   printk(KERN_INFO "Openning\n");
   openCount++;
   sprintf(message, "%zu\n", openCount, bytes_written);
   messagePtr = message;
   
   return 0;
}

static int solution_release(struct inode *inode, struct file *file)
{
   printk(KERN_INFO "Releasing\n");
   return 0;
}

static ssize_t solution_read(struct file *file, char *buf, size_t lbuf, loff_t *offset){
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

static ssize_t solution_write(struct file *file, const char *buf, size_t lbuf, loff_t *offset){
  printk(KERN_INFO "Writing %zu characters from the user\n", lbuf);
  bytes_written += lbuf;
  return lbuf;
}

static long solution_unlocked_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
  if (cmd != SUM_LENGTH && cmd != SUM_CONTENT){
    printk(KERN_INFO "unknown command %d\n", cmd);
    return -1;
  }

  int result = -1;
  char argument [MAX_STRING_SIZE];
  strncpy_from_user( argument, (char*)arg, MAX_STRING_SIZE );
  
  if (cmd == SUM_LENGTH)
  {
    printk(KERN_INFO "SUM_LENGTH\n");
    sum_length += strlen(argument);
    result = sum_length;
  } else if (cmd == SUM_CONTENT)
  {
    printk(KERN_INFO "SUM_CONTENT\n");
    char* end;
    sum_content += (int)simple_strtol(argument, &end, 10);   
    result = sum_content;
  }
  printk(KERN_INFO "IOCTL (%s) == %d\n", argument, result); 

  return result;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = solution_read,
    .write = solution_write,
    .open = solution_open,
    .release = solution_release,
    .unlocked_ioctl = solution_unlocked_ioctl,
};

static int __init init_sulution(void)
{
   int result = register_chrdev(majorNumber, MYDEV_NAME, &fops);
   if (result < 0){
      printk(KERN_ALERT "Failed to register existing device\n");
      return result;
   }
   printk(KERN_INFO "Init device\n");
   return 0;
}

static void __exit cleanup_sulution(void)
{
   unregister_chrdev(majorNumber, MYDEV_NAME);             // unregister the major number
   printk(KERN_INFO "Cleanup\n");
}

module_init(init_sulution);
module_exit(cleanup_sulution);

MODULE_LICENSE("GPL");
