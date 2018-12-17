/*
版权声明：
本代码未经本人许可不得用于商业用途，一经发现，按一人民币一个字符（包含空格和标点）收取费用。一旦你使用了本代码，默认你已经阅读并接受该声明

Copyright Notice：
This code may not be used for commercial purposes without my permission. Once found, it is charged at a rate of ￥1
(including spaces and punctuation). Once you have used this code, by default you have read and accepted the statement.
*/

#ifndef _SHT2X_H_
#define _SHT2X_H_

#include <cstdint>

class SHT2x
{
public:
	SHT2x();
	~SHT2x();
	int init_dev(char *dev, uint16_t slave_addr);
	int reset_dev(void);
	float measure_t(void);
	float measure_h(void);
private:
	int dev_fd;
	uint16_t i2c_addr;
};

#endif /* !_SHT2X_H_ */
