#include "Servos.h"

void Uart::init() {
	servos[0] = Servo(GPIOB, GPIO_PIN_4, 0, 5);
}
