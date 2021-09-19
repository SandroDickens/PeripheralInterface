#ifndef _SHT2X_H_
#define _SHT2X_H_

#include <cstdint>
#include <mutex>

class SHT2x
{
public:
	static SHT2x *getInstance(const char *dev, uint16_t slave_addr)
	{
		if (instance == nullptr)
		{
			instMutex.lock();
			if (instance == nullptr)
			{
				instance = new SHT2x();
				if (0 != instance->init_dev(dev, slave_addr))
				{
					delete instance;
					instance = nullptr;
				}
			}
			instMutex.unlock();
		}
		return instance;
	}

	float getTemperature();

	float getHumidity();

	SHT2x(const SHT2x &obj) = delete;

	SHT2x &operator=(const SHT2x &) = delete;

	class InnerGarbo
	{
	public:
		virtual ~InnerGarbo()
		{
			if (SHT2x::instance != nullptr)
			{
				delete SHT2x::instance;
				SHT2x::instance = nullptr;
			}
		}
	};

private:
	SHT2x();

	~SHT2x();

	int init_dev(const char *dev, uint16_t slave_addr);

	int reset_dev();

private:
	int dev_fd;
	uint16_t i2c_addr;
	static std::mutex instMutex;
	static SHT2x *instance;
	static InnerGarbo innerGarbo;
};

#endif /* !_SHT2X_H_ */
