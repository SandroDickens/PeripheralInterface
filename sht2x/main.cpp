/*
版权声明：
本代码未经本人许可不得用于商业用途，一经发现，按一人民币一个字符（包含空格和标点）收取费用。一旦你使用了本代码，默认你已经阅读并接受该声明

Copyright Notice：
This code may not be used for commercial purposes without my permission. Once found, it is charged at a rate of ￥1
(including spaces and punctuation). Once you have used this code, by default you have read and accepted the statement.
*/

#include <cstdio>
#include <unistd.h>
#include "sht2x.hpp"

#define I2C_DEV_ADDR        0x40

int main(void)
{
	SHT2x sht20;
	sht20.init_dev((char*)"/dev/i2c-0",I2C_DEV_ADDR);
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
