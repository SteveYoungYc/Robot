#include "Servo.h"

Servo::Servo(GPIO_TypeDef *_GPIOx, uint16_t _GPIO_Pin, int _id, int _offset) {
	GPIOx = _GPIOx;
	GPIO_Pin = _GPIO_Pin;
    id = _id;
	offset = _offset;
}

void Servo::setID(int _id) {
    id = _id;
}

void Servo::setAngle(int _angle) {
	if(_angle < 0 || _angle > 180) {
		return;
	}
	angle = _angle - offset;
	highTime = angle / 1.8 + 25;
}

void Servo::exe() {
	if(time < highTime) {
		HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
	}
}
