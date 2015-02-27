/**
Author:
Matt Hunter
Noah Orr

9/24/14

Header file for GPIO intreface library.  Gives access to Sysfs gpio interface.
**/

#ifndef bbb_gpio
#define bbb_gpio

#include <vector>

using namespace std;

enum DIRECTION {
	INPUT = 0,
	OUTPUT = 1
};

enum PIN_VALUE {
	LOW = 0,
	HIGH = 1
};

typedef struct pins_t {
	const char *name;
	const char *key;
	unsigned int gpio;
	int pwm_mux_mode;
	int ain;
	int isAllocatedByDefault;
} pins_t;

class GPIO {
private:
	static GPIO *instance;
	vector<int> pinsInUse;

	GPIO();
	
	int getGpioByKey(const char *key);

public:
	~GPIO();

	static GPIO *getInstance();

	int setup(const char *key, DIRECTION direction);
	int setValue(unsigned int pin, PIN_VALUE value);
	PIN_VALUE getValue(unsigned int pin);

	void clean();
};

#endif // bbb_gpio
