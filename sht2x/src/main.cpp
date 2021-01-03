#include <cstdio>
#include <unistd.h>
#include "sh2x/sht2x.hpp"

#define I2C_DEV_ADDR        0x40

int main()
{
	SHT2x *sht20 = SHT2x::getInstance("/dev/i2c-0", I2C_DEV_ADDR);
	if (sht20 != nullptr)
	{
		for (int i = 0; i < 30; i++)
		{
			float temp = sht20->getTemperature();
			float humi = sht20->getHumidity();
			printf("T:%2.3f, H:%2.4f\n", temp, humi);
			sleep(1);
		}
	}
	return 0;
}
