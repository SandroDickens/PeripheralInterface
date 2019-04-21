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
