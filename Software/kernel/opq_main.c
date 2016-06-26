#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h> /* printk() */

#include <linux/slab.h>   /* kmalloc() */
#include <linux/fs.h>     /* everything... */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/cdev.h>
#include <linux/version.h>
#include <linux/uaccess.h>

#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/spi/spi.h> 
#include <linux/workqueue.h>


///GPIO IRQ Stuff stuff:
#define OPQ_DATA_READY_GPIO 18
#define OPQ_DATA_READY_GPIO_DESC           "Data ready pin for the STM32f3"
#define OPQ_DATA_READY_GPIO_DEVICE_DESC    "OPQBox2"
///Character device stuff
#define OPQ_CYCLE_BUFFER_SIZE 408
#define OPQ_CYCLE_BUFFER_COUNT 32
///SPI
#define OPQ_SPI_BUS 0
#define OPQ_SPI_CHAN 0
#define OPQ_SPI_MAX_SPEED 976000       //mhz ish
#define OPQ_SPI_BITS_PER_WORD 8


struct opq_device
{
	//serialize access
	struct mutex * mutex;
	struct cdev cdev;
	//Character device data, this should be connected to the DMA buffer
	void * buffer;  // read buffer
    int head;
    int tail;
    //SPI Handle
    struct spi_device  * spi_device;
    //Irq number for data ready pin
    short int irq_any_gpio;
    struct workqueue_struct *wq;
};

struct opq_device * opq_dev;

//Chardev
loff_t opq_chardev_lseek(struct file *file, loff_t offset, int orig);
int opq_chardev_open(struct inode *inode,struct file *filep);
int opq_chardev_release(struct inode *inode,struct file *filep);
ssize_t opq_chardev_read(struct file *filep,char *buff,size_t count,loff_t *offp );
ssize_t opq_chardev_write(struct file *filep,const char *buff,size_t count,loff_t *offp );
ssize_t opq_chardev_buffered_read(struct file *filep,char *buff,size_t count,loff_t *offp );
ssize_t opq_chardev_buffered_write(struct file *filep,const char *buff,size_t count,loff_t *offp );
int opq_init_chardev(void);
//GPIO IRQ
static irqreturn_t opq_gpio_irq_handler(int irq, void *dev_id, struct pt_regs *regs);
int opq_gpio_int_config(void);
void opq_gpio_int_release(void);
//SPI
int opq_init_spi(void);
void opq_release_spi(void);
static void opq_wq_spi_reader_handler(struct work_struct *w);
static DECLARE_WORK(opq_wq_spi_reader, opq_wq_spi_reader_handler);

//IRQ Handler
static irqreturn_t opq_gpio_irq_handler(int irq, void *dev_id, struct pt_regs *regs) {
    unsigned long flags;
    // disable hard interrupts (remember them in flag 'flags')
    // local_irq_save(flags);
    queue_work(opq_dev->wq, &opq_wq_spi_reader);
    // restore hard interrupts
    //local_irq_restore(flags);
    return IRQ_HANDLED;
}

//This will set up the irq
int opq_init_gpio_int(void) {
   if (gpio_request(OPQ_DATA_READY_GPIO, OPQ_DATA_READY_GPIO_DESC)) {
      printk("OPQ: GPIO request faiure: %s\n", OPQ_DATA_READY_GPIO_DESC);
      return -ENODEV;
   }
   if(gpio_direction_input(OPQ_DATA_READY_GPIO)){
        printk("OPQ: GPIO configuration faiure: %s\n", OPQ_DATA_READY_GPIO_DESC);
        return -ENODEV;
   }
   if ((opq_dev->irq_any_gpio = gpio_to_irq(OPQ_DATA_READY_GPIO)) < 0 ) {
      printk("OPQ: GPIO to IRQ mapping faiure %s\n", OPQ_DATA_READY_GPIO_DESC);
      return-ENODEV;
   }
   printk(KERN_NOTICE "OPQ: mapped interrupt %d\n", opq_dev->irq_any_gpio);

   if (request_irq(opq_dev->irq_any_gpio,
                   (irq_handler_t ) opq_gpio_irq_handler,
                   IRQF_TRIGGER_RISING,
                   OPQ_DATA_READY_GPIO_DESC,
                   OPQ_DATA_READY_GPIO_DEVICE_DESC)) {
      printk("OPQ: Irq Request failure\n");
      return -ENODEV;
   }
   printk(KERN_NOTICE "OPQ: interrupt %d ready\n", opq_dev->irq_any_gpio);
   opq_dev->wq = create_workqueue("opq_queue");
   return 0;
}

//This will release the IRQ
void opq_release_gpio_int(void) { 
   free_irq(opq_dev->irq_any_gpio, OPQ_DATA_READY_GPIO_DEVICE_DESC);
   gpio_free(OPQ_DATA_READY_GPIO);
   cancel_work_sync(&opq_wq_spi_reader);
   destroy_workqueue(opq_dev->wq);
   return;
}

///SPI stuff

int opq_init_spi(void){
    struct spi_board_info spi_device_info = {
        .modalias = "spi:opq",
        .max_speed_hz = OPQ_SPI_MAX_SPEED,
        .bus_num = OPQ_SPI_BUS,
        .chip_select = OPQ_SPI_CHAN,
        .mode = 0,
    };
    int ret;
    struct spi_master *master;
    master = spi_busnum_to_master( spi_device_info.bus_num );
    if( !master ){
        printk("OPQ: could not allocate SPI master\n");
        return -ENODEV;
    }

    opq_dev->spi_device = spi_new_device( master, &spi_device_info );
    if( !opq_dev->spi_device ){
        printk("OPQ: could not allocate SPI device\n");
        return -ENODEV;
    }
    opq_dev->spi_device->bits_per_word = OPQ_SPI_BITS_PER_WORD;
    ret = spi_setup( opq_dev->spi_device );
    printk(KERN_NOTICE "OPQ: spi %d:%d ready\n",OPQ_SPI_BUS,OPQ_SPI_CHAN);
    return 0;
}

void opq_release_spi(void){
   spi_unregister_device( opq_dev->spi_device );
}

//Bottom half og the IRQ Handler
static void opq_wq_spi_reader_handler(struct work_struct *w){
    //mutex_lock(opq_dev->mutex);
    spi_read(opq_dev->spi_device, opq_dev->buffer, OPQ_CYCLE_BUFFER_SIZE);
    //mutex_unlock(opq_dev->mutex);
    printk("hello\n");
}
//Chardev

int opq_chardev_major=-1;

int opq_init_chardev()
{
    struct file_operations opq_chardev_fops;
	dev_t tmpchrdev;

	opq_chardev_fops.open =  opq_chardev_open;
	opq_chardev_fops.read = opq_chardev_read;
	opq_chardev_fops.write = opq_chardev_write;
	opq_chardev_fops.release = opq_chardev_release;
	opq_chardev_fops.llseek = opq_chardev_lseek;
	if(alloc_chrdev_region (&tmpchrdev,0,1,"opq_chardev")<0)
	{
		printk("opq driver: couldnt get major/minor number.\n");
		return -1;
	}
	opq_chardev_major=MAJOR(tmpchrdev);
	cdev_init ( &opq_dev->cdev, &opq_chardev_fops);
	opq_dev->cdev.owner = THIS_MODULE;
	opq_dev->cdev.ops = &opq_chardev_fops;
	if(cdev_add(&opq_dev->cdev, MKDEV(opq_chardev_major, 0), 1) < 0)
	{
		printk("opq driver: Failed to initialize character device\n");
		return -1;
	}
	printk("opq driver: Initialized character device with major number %d\n", opq_chardev_major);
	return 0;
}

void opq_release_chardev(void)
{
	cdev_del(&opq_dev->cdev);
	unregister_chrdev_region (MKDEV(opq_chardev_major, 0),1);
}

int opq_chardev_open(struct inode *inode,struct file *filep)
{
	return 0;
}

int opq_chardev_release(struct inode *inode,struct file *filep)
{
	return 0;
}



ssize_t opq_chardev_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    mutex_lock(opq_dev->mutex);
    if(count > OPQ_CYCLE_BUFFER_SIZE)
        count = OPQ_CYCLE_BUFFER_SIZE;
    
    //Read SPI here
	if(copy_to_user(buf, opq_dev->buffer, count) != 0)
	{
		printk("KERN_ERROR" "OPQ: copy to user failed\n");
	}
	mutex_unlock(opq_dev->mutex);
	return count;
}


ssize_t opq_chardev_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	return -EINVAL;
}

loff_t opq_chardev_lseek(struct file *file, loff_t offset, int orig)
{
    return -EINVAL;
}


static int __init opq_module_init(void)
{
    opq_dev = kmalloc(sizeof(struct opq_device), GFP_KERNEL);
    opq_dev->mutex = kmalloc(sizeof(struct mutex), GFP_KERNEL);
    opq_dev->buffer = kmalloc(OPQ_CYCLE_BUFFER_SIZE,GFP_KERNEL);
    mutex_init(opq_dev->mutex);
    //opq_init_chardev();
    opq_init_spi();
    opq_init_gpio_int();
    printk (KERN_NOTICE "OPQ: OPQ driver loaded!\n");
    return 0;
}

static void __exit opq_module_exit(void)
{
    //opq_release_chardev();
    opq_release_spi();
    opq_release_gpio_int();
    printk (KERN_NOTICE "OPQ: OPQ driver exiting.\n");    
    kfree(opq_dev->mutex);
    kfree(opq_dev->buffer);
    kfree(opq_dev);
    return;
}

static const struct of_device_id opq_match[] = {
  { .compatible = "opq", },
  {}
};
MODULE_DEVICE_TABLE(of, opq_match);


module_init(opq_module_init);
module_exit(opq_module_exit);

MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:opq");
