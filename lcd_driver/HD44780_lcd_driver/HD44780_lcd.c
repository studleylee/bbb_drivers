#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/gpio.h>
#include "hd44780.h"

#define AUTHOR "23ars <ardeleanasm@gmail.com>"
#define DRIVER_DESC "LCD Driver"
#define DEVICE_NAME "hd44780_lcd_driver"

MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("hd44780");
/*Private functions*/


/*Exported functions*/
extern u8 hd44780_init_interface(struct SLcdPins *,const ELcdMode);
EXPORT_SYMBOL(hd44780_init_interface);

extern void hd44780_uninit_interface(const struct SLcdPins *,const ELcdMode);
EXPORT_SYMBOL(hd44780_uninit_interface);

extern void hd44780_execute(struct SLcdPins *,const u8 ,const ELcdMode);
EXPORT_SYMBOL(hd44780_execute);

/*Extern functions*/
u8
hd44780_init_interface(struct SLcdPins *s_lcd_pins,const ELcdMode mode)
{
	u8 error_code=0x00;
	u8 i;
/*request rs and en pins*/
	error_code+=gpio_request(s_lcd_pins->en,"sysfs");
	error_code+=gpio_request(s_lcd_pins->rs,"sysfs");
/*set direction output for rs and en*/
	error_code+=gpio_direction_output(s_lcd_pins->en,0);
	error_code+=gpio_direction_output(s_lcd_pins->rs,0);
/*export rs and en*/
	error_code+=gpio_export(s_lcd_pins->en,0);
	error_code+=gpio_export(s_lcd_pins->rs,0);
/*do the previous operations for data lines*/
	if(Mode_4_bits==mode){
		for(i=4;i<8;i++){
			error_code+=gpio_request(s_lcd_pins->data[i].port,"sysfs");
			error_code+=gpio_direction_output(s_lcd_pins->data[i].port,0);
			error_code+=gpio_export(s_lcd_pins->data[i].port,0);
		}
	}else if(Mode_8_bits==mode){
		for(i=0;i<8;i++){
			error_code+=gpio_request(s_lcd_pins->data[i].port,"sysfs");
			error_code+=gpio_direction_output(s_lcd_pins->data[i].port,0);
			error_code+=gpio_export(s_lcd_pins->data[i].port,0);
		}
	}
	/*Uuups, we have a big problem now... error_code is not checked if is 0 or not. But, don't care for now!*/
	return error_code;
	
}



void
hd44780_uninit_interface(const struct SLcdPins *s_lcd_pins,const ELcdMode mode)
{
	u8 i;
	gpio_unexport(s_lcd_pins->rs);
	gpio_free(s_lcd_pins->rs);
	gpio_unexport(s_lcd_pins->en);
	gpio_free(s_lcd_pins->en);
	if(Mode_4_bits==mode){
		for(i=4;i<8;i++){
			gpio_unexport(s_lcd_pins->data[i].port);
			gpio_free(s_lcd_pins->data[i].port);
		}
	}
	else if(Mode_8_bits==mode){
		for(i=0;i<8;i++){
			gpio_unexport(s_lcd_pins->data[i].port);
			gpio_free(s_lcd_pins->data[i].port);
		}
	}
}

void 
hd44780_execute(struct SLcdPins *s_lcd_pins,const u8 command,const ELcdMode mode)
{

}




static int
__init hd44780_init(void)
{
//	printk(KERN_INFO,"Driver %s loaded.Build on %s %s\n",DEVICE_NAME,__DATE__,__TIME__);

	return 0;
}

static void
__exit hd44780_exit(void)
{
//	printk(KERN_INFO,"Driver %s unloaded. Build on %s %s\n",DEVICE_NAME,__DATE__,__TIME__);
}


module_init(hd44780_init);
module_exit(hd44780_exit);