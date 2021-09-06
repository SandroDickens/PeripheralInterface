# 版权声明：
本代码未经本人许可不得用于商业用途，一经发现，按一人民币一个字符（包含空格和标点）收取费用。一旦你使用了本代码，默认你已经阅读并接受该声明

# Copyright Notice：
This code may not be used for commercial purposes without my permission. Once found, it is charged at a rate of ￥1
(including spaces and punctuation). Once you have used this code, by default you have read and accepted the statement.


# SHT20温湿度传感器驱动代码，Linux系统，I2C接口
# 编译
```
[username@hostname sh2x]$ cmake -S . -B ./build
[username@hostname sh2x]$ make
```
或者
```
[username@hostname sh2x]$ ./run.sh
```
# 用法
```
#include "sh2x/sht2x.hpp"

#define I2C_DEV_ADDR        0x40
SHT2x *sht20 = SHT2x::getInstance("/dev/i2c-0", I2C_DEV_ADDR);
float temp = sht20->getTemperature();
float humi = sht20->getHumidity();
```
