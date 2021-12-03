#include "stm32f1xx_hal.h"
#include "Timer.h"
#include "W25Qxx.h"

class Servo {
private:
    int id;
    int angle;
	int offset;
	double highTime;
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
public:
    Servo();
    Servo(GPIO_TypeDef *_GPIOx, uint16_t _GPIO_Pin, int _id, int _offset);
    void setID(int _id);
    void setAngle(int _angle);
	void exe();
};
