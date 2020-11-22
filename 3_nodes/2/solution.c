#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/cdev.h>


#define MYDEV_NAME "solution"
#define KBUF_SIZE  (size_t)256

struct sesesion_data
{
    char buf[KBUF_SIZE];
    size_t read_counter;
    bool is_first_cat;
    size_t id;
    unsigned long size;
};

static dev_t first;
static unsigned int count = 1;
static int my_major = 240, my_minor = 0;
static struct cdev *my_cdev;
static size_t session_number = 0;


static int mychrdev_open(struct inode *inode, struct file *file)
{
    struct sesesion_data *kbuf = NULL;
    // printk(KERN_INFO "kernel_mooc Opening device id: %lu\n", session_number);
    file->private_data = NULL;

    kbuf= kmalloc(sizeof(struct sesesion_data), GFP_KERNEL);
    if (!kbuf)
        return -ENOMEM;
    
    kbuf->read_counter = 0;   
    kbuf->is_first_cat = true;
    kbuf->id = session_number;
    kbuf->size = sprintf(kbuf->buf, "%lu", kbuf->id);
    file->private_data = kbuf;
    session_number++;

    return 0;
}

static int mychrdev_release(struct inode *inode, struct file *file)
{
    char tempbuf[10];

    struct sesesion_data *kbuf = file->private_data;
    memcpy(tempbuf, kbuf->buf, 10);
    // printk(KERN_INFO "kernel_mooc Releasing device id: %lu\n", kbuf->id);

    if(kbuf)
        kfree(kbuf);
    kbuf = NULL;
    file->private_data = NULL;

    return 0;
}

static ssize_t mychrdev_read( struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
    struct sesesion_data *kbuf = file->private_data;
    int nbytes = 0;
    int bytes_to_read = 0;

    {
            bytes_to_read = kbuf->size - *ppos;
            bytes_to_read = (bytes_to_read >= 0 ? bytes_to_read : 0 );

            nbytes = bytes_to_read - copy_to_user(buf, kbuf->buf + *ppos, bytes_to_read);
            *ppos = *ppos + nbytes;

            if(nbytes)
                kbuf->is_first_cat = false;

            return nbytes;      
    }

    return nbytes;
}

static ssize_t mychrdev_write( struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos)
{
    struct sesesion_data *kbuf = NULL;
    int nbytes = 0;
    kbuf = file->private_data;
    // printk(KERN_INFO "kernel_mooc Write device id %lu size = %lu, ppos = %d", kbuf->id, lbuf, (int)*ppos);
    nbytes = lbuf - copy_from_user(kbuf->buf + *ppos, buf, lbuf);
    *ppos = *ppos + nbytes;
    kbuf->size = ( kbuf->size >= *ppos ? kbuf->size : *ppos );
    printk(KERN_INFO "kernel_mooc End of write device id %lu size: %d", kbuf->id, (int)kbuf->size);
    return nbytes;
}

static loff_t mychrdev_lseek(struct file *file, loff_t offset, int orig)
{
    loff_t testpos;
    char op[30];

    switch(orig)
    {
        case SEEK_SET:
            sprintf(op, "%s", "SEEK_SET");
            testpos = offset;
            break;
        case SEEK_CUR:
            sprintf(op, "%s", "SEEK_CUR");
            testpos = file->f_pos + offset;
            break;
        case SEEK_END:
            sprintf(op, "%s", "SEEK_END");
            testpos = ((struct sesesion_data *)file->private_data)->size + offset;
            break;
        default:    
            return -EINVAL;
    }   
// printk(KERN_INFO "kernel_mooc Seek(%s) device id %lu lseek from  %d on %d\n", op,
    // ((struct sesesion_data *)file->private_data)->id, (int)file->f_pos, (int)offset);

    testpos = testpos < KBUF_SIZE ? testpos : KBUF_SIZE;
    testpos = testpos >= 0 ? testpos : 0;
    file->f_pos = testpos;
    // printk(KERN_INFO "kernel_mooc Seek device id %lu to %ld position\n", 
    //     ((struct sesesion_data *)file->private_data)->id, (long)testpos);
    return testpos;
}

static const struct file_operations mycdev_fops = {
    .owner = THIS_MODULE,
    .read = mychrdev_read,
    .write = mychrdev_write,
    .open = mychrdev_open,
    .release = mychrdev_release,
    .llseek = mychrdev_lseek
};

static int __init init_chrdev(void)
{
    first = MKDEV(my_major, my_minor);  
    register_chrdev_region(first, count, MYDEV_NAME);   

    my_cdev = cdev_alloc();

    cdev_init(my_cdev, &mycdev_fops);
    cdev_add(my_cdev, first, count);

    return 0;
}

static void __exit cleanup_chrdev(void)
{
    if(my_cdev)
        cdev_del(my_cdev);

    unregister_chrdev_region(first, count);
}

module_init(init_chrdev);
module_exit(cleanup_chrdev);

MODULE_LICENSE("GPL");
