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
int positions[8][2] = {{90, 75}, 
					   {90, 105},
					   {90, 75},
					   {90, 90},
					   {90, 75},
					   {90, 105},
					   {90, 75},
					   {90, 105}};


Servo servos[8] = {Servo(GPIOB, GPIO_PIN_4, 0, offset_int[0]),
				   Servo(GPIOB, GPIO_PIN_9, 0, offset_int[1]),
				   Servo(GPIOB, GPIO_PIN_3, 0, offset_int[2]),
				   Servo(GPIOB, GPIO_PIN_12, 0, offset_int[3]),
				   Servo(GPIOB, GPIO_PIN_5, 0, offset_int[4]),
				   Servo(GPIOB, GPIO_PIN_1, 0, offset_int[5]),
				   Servo(GPIOB, GPIO_PIN_6, 0, offset_int[6]),
				   Servo(GPIOB, GPIO_PIN_0, 0, offset_int[7]),
};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM2) {
		time = (time + 1) % 200;
		
		if(ms_time == 0) {
			pos = (pos + 1) % 2;
			for (int i = 0; i < 8; i++) {
				servos[i].setAngle(positions[i][pos]);
			}
		}
		
		ms_time = (ms_time + 1) % 6000;
		for (int i = 0; i < 8; i++) {
			servos[i].exe();
		}
		// servos[0].exe();
		// servos[1].exe();
		// servos[2].exe();
		// servos[3].exe();
		// servos[4].exe();
		// servos[5].exe();
	}
}
