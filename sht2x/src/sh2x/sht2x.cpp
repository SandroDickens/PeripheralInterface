#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "sht2x.hpp"

#define CRC_POLY            0x131U   /* P(x)=x^8+x^5+x^4+1 = 100110001 */
#define MEASURE_RETRY_WAIT  85000   /* 85ms = maximum measurement time */

enum SHT2x_CMD
{
	CMD_TRIG_TEMP_POLL = 0xF3, /* command trig. temp meas. no hold master */
	CMD_TRIG_HUMI_POLL = 0xF5, /* command trig. humidity meas. no hold master */
	CMD_USER_REG_WRITE = 0xE6, /* command writing user register */
	CMD_USER_REG_READ  = 0xE7, /* command reading user register */
	CMD_SOFT_RESET     = 0xFE, /* command soft reset */
	CMD_MEASURE_READ   = 0x00, /* read measured value (for software convenience) */
};

enum SHT2x_OPTION
{
	OPT_RES_12_14BIT = 0x00, /* RH=12bit, T=14bit */
	OPT_RES_8_12BIT  = 0x01, /* RH= 8bit, T=12bit */
	OPT_RES_10_13BIT = 0x80, /* RH=10bit, T=13bit */
	OPT_RES_11_11BIT = 0x81  /* RH=11bit, T=11bit */
};

int measure_read(int fd, SHT2x_CMD cmd, uint16_t *value);

uint8_t calc_crc(const uint8_t data[], uint8_t size);

int check_crc(uint8_t data[], uint8_t size, uint8_t crc);

float calc_humi(uint16_t value);

float calc_temp(uint16_t value);

std::mutex SHT2x::instMutex;
SHT2x *SHT2x::instance = nullptr;

SHT2x::SHT2x()
{
	dev_fd = -1;
	i2c_addr = 0x00;
}

SHT2x::~SHT2x()
{
	if (dev_fd != -1)
	{
		close(dev_fd);
		dev_fd = -1;
	}
	i2c_addr = 0x00;
}

int SHT2x::init_dev(const char *dev, uint16_t slave_addr)
{
	/* */
	if ((dev == nullptr) || (dev_fd != -1))
	{
		return -1;
	}
	i2c_addr = slave_addr;
	dev_fd = open(dev, O_RDWR | O_NONBLOCK);
	if (dev_fd == -1)
	{
		perror("can't open devive!");
		return -1;
	} else
	{
		/* 7 bit addrs */
		int res = ioctl(dev_fd, I2C_TENBIT, 0);
		if (res != -1)
		{
			res = ioctl(dev_fd, I2C_SLAVE, i2c_addr);
			if (res != -1)
			{
				return 0;
			} else
			{
				return -1;
			}
		} else
		{
			return -1;
		}
	}
}

int SHT2x::reset_dev()
{
	SHT2x_CMD cmd = CMD_SOFT_RESET;
	if (1 != write(dev_fd, &cmd, 1))
	{
		perror("i2c device reset error!");
		return -1;
	} else
	{
		usleep(15000);
		return 0;
	}
}

float SHT2x::getTemperature()
{
	float temp = -273.0f;
	SHT2x_CMD cmd = CMD_TRIG_TEMP_POLL;
	if (1 != write(dev_fd, &cmd, 1))
	{
		perror("i2c send cmd error!");
	} else
	{
		cmd = CMD_MEASURE_READ;
		uint16_t value = 0x00;
		if (-1 != measure_read(dev_fd, cmd, &value))
		{
			temp = calc_temp(value);
		}
	}
	return temp;
}

float SHT2x::getHumidity()
{
	float humi = 0.00f;
	SHT2x_CMD cmd = CMD_TRIG_HUMI_POLL;
	if (1 != write(dev_fd, &cmd, 1))
	{
		perror("i2c send cmd error!");
	} else
	{
		cmd = CMD_MEASURE_READ;
		uint16_t value = 0x00;
		if (-1 != measure_read(dev_fd, cmd, &value))
		{
			humi = calc_humi(value);
		}
	}
	return humi;
}

int measure_read(int fd, SHT2x_CMD cmd, uint16_t *value)
{
	uint8_t buf[3];

	for (uint8_t i = 0; i < 2; i++)
	{
		if (read(fd, buf, 3) != 3)
		{
			usleep(MEASURE_RETRY_WAIT);
			continue;
		}
		if (check_crc(buf, 2, buf[2]))
		{
			perror("ERROR: CRC validation");
			return -1;
		}
		if (value == nullptr)
		{
			perror("ERROR: invalid function call");
			return -1;
		}
		*value = buf[0] << 8U | buf[1];
		return 0;
	}
	perror("ERROR: i2c read");
	return -1;
}

uint8_t calc_crc(const uint8_t data[], uint8_t size)
{
	uint16_t crc = 0;

	for (uint8_t i = 0; i < size; i++)
	{
		crc ^= (data[i]);
		for (uint8_t j = 0; j < 8; j++)
		{
			crc = (crc & 0x80U) ? ((crc << 1U) ^ CRC_POLY) : (crc << 1U);
		}
	}
	return crc;
}

int check_crc(uint8_t data[], uint8_t size, uint8_t crc)
{
	return (calc_crc(data, size) == crc) ? 0 : -1;
}

float calc_temp(uint16_t value)
{
	if ((value & 0x2U) != 0)
	{
		perror("ERROR: invalid value");
		exit(EXIT_FAILURE);
	}
	return -46.85F + (175.72F * float(value & 0xFFFCU)) / 0x10000;
}

float calc_humi(uint16_t value)
{
	if ((value & 0x2U) == 0)
	{
		perror("ERROR: invalid value");
		exit(EXIT_FAILURE);
	}
	return -6 + (125.0F * float(value & 0xFFFCU)) / 0x10000;
}
