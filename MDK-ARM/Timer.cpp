#include "main.h"
#include "rtc.h"
#include "tim.h"
#include "gpio.h"
#include "Timer.h"
#include "Servo.h"
#include "W25Qxx.h"

extern int offset_int[17];

int time = 0;
int ms_time = 0;
int second = 0;
int pos = 0;
int postions[8] = {90, 60, 90, 60, 90, 60, 90, 60};
Servo servos[2] = {Servo(GPIOB, GPIO_PIN_4, 0, offset_int[0]),
					Servo(GPIOB, GPIO_PIN_9, 0, offset_int[1])};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM2) {
		time = (time + 1) % 200;
		//ms_time = (ms_time + 1) % 10;
		ms_time = (ms_time + 1) % 4000;
		if(ms_time == 0) second++;
		if(ms_time == 0) {
			pos = (pos + 1) % 8;
			//servos[0].setAngle(90);
		}
		servos[0].exe();
		//servos[1].exe();
	}
}
