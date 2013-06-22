#ifdef GPIOLIB

#include <stdio.h>
#include "gpio.h"

#define PORT 138

int main(void)
{
	if (gpio_open(PORT, GPIO_READ) < 0) {
	    puts("Unable to open PIN file");
	    return -1;
	}

	while (1) {
		printf("%d\n", gpio_read(PORT));
	}

	gpio_close(PORT);
	return 0;
}

#endif
