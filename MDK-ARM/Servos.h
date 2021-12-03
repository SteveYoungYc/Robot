#include "stm32f1xx_hal.h"
#include "Timer.h"
#include "W25Qxx.h"
#include "Servo.h"

class Servos {
private:
	Servo servos[17];
public:
	void init();
};
