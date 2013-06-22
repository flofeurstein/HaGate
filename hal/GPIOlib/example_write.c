#ifdef GPIOLIB

#include <stdio.h>
#include "gpio.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 138

int main3(void)
{
	int error = 0;
	
	if (gpio_open(PORT, GPIO_WRITE) < 0) {
	    puts("Unable to open PIN file");
	    return -1;
	}
	
	while (!error) {
	    error = gpio_write(PORT,1);
	    sleep(1);
	    error = gpio_write(PORT,0);
	    sleep(1);
	}
	
	gpio_close(PORT);
	return 0;
}

#endif
