#include <cstdio>
#include <unistd.h>
#include "sht2x.hpp"

#define I2C_DEV_ADDR        0x40

int main(void)
{
	SHT2x sht20;
	sht20.init_dev("/dev/i2c-1",I2C_DEV_ADDR);
	sht20.reset_dev();
	float temp = 0.0f, humi = 0.0f;
	for(int i = 0; i<30; ++i)
	{
		temp = sht20.measure_t();
		humi = sht20.measure_h();
		printf("T: %0.2f, H: %0.2f\n",temp, humi);
		sleep(1);
	}
	return 0;
}