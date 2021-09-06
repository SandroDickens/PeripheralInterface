#include <cstdio>
#include <unistd.h>
#include "sh2x/sht2x.hpp"

#define I2C_DEV_ADDR        0x40

int main()
{
	SHT2x *sht20 = SHT2x::getInstance("/dev/i2c-0", I2C_DEV_ADDR);
	if (sht20 != nullptr)
	{
		float temp = sht20->getTemperature();
		float humi = sht20->getHumidity();
		printf("%2.2f, %2.2f\n", temp, humi);
		return 0;
	}
	else
	{
		return -1;
	}
}
