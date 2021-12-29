#include "main.h"
#include "rtc.h"
#include "tim.h"
#include "gpio.h"
#include "Timer.h"
#include "Servo.h"
#include "W25Qxx.h"
#include "usart.h"

extern int offset_int[17];
extern uint8_t uart_buffer[256];
extern uint8_t recvTemp;

void control_legs();

int time = 0;
int ms_time = 0;
int second = 0;
int pos = 0;

int left_arm_1 = 90;
int left_arm_2 = 90;
int right_arm_1 = 90;
int right_arm_2 = 90;

int left_arm_up = 90;
int right_arm_up = 90;

float head_angle = 10;

bool flag_A = false;
bool flag_B = false;
bool flag_C = false;
bool flag_D = false;

int positions[8][2] = {{90, 75}, 
					   {90, 105},
					   {90, 75},
					   {90, 90},
					   {90, 75},
					   {90, 105},
					   {90, 75},
					   {90, 105}};

int positions0[8][2] = {{90, 90}, 
					   {90, 90},
					   {90, 90},
					   {90, 90},
					   {90, 90},
					   {90, 90},
					   {90, 90},
					   {90, 90}};

int positions_arm0[8][2] = {{90, 90},
						   {90, 90},
						   {90, 90},
						   {90, 90},
						   {90, 90},
						   {90, 90}};

int positions_arm[8][2] = {{140, 100},
						   {140, 100},
						   {0, 0},
						   {40, 80},
						   {40, 80},
						   {180, 180}};

int positions_arm1[8][2] = {{100, 100},
						   {100, 100},
						   {0, 40},
						   {80, 80},
						   {80, 80},
						   {180, 140}};

int down[4][2] = {{90, 100},
					{90, 100},
					{90, 100},
					{90, 100}};

int positions_legs0[4][2] = {{90, 90},
					{90, 90},
					{90, 90},
					{90, 90}};

Servo legs[4] = {Servo(GPIOB, GPIO_PIN_3, 0, offset_int[2]),
				   Servo(GPIOB, GPIO_PIN_12, 0, offset_int[3]),
				   Servo(GPIOB, GPIO_PIN_5, 0, offset_int[4]),
				   Servo(GPIOB, GPIO_PIN_1, 0, offset_int[5])};

Servo arms[6] = {Servo(GPIOC, GPIO_PIN_5, 0, 0),	//right arm(angle-- >>> up)
				 Servo(GPIOC, GPIO_PIN_6, 0, 0),
				 Servo(GPIOC, GPIO_PIN_7, 0, 0),
				 Servo(GPIOC, GPIO_PIN_10, 0, 0),   //left arm(angle-- >>> down)
				 Servo(GPIOC, GPIO_PIN_11, 0, 0),
                 Servo(GPIOC, GPIO_PIN_8, 0, 0)};

void servo_init() {
	for (int i = 0; i < 6; i++) {
		arms[i].setAngle(positions_arm0[i][0]);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM2) {
		time = (time + 1) % 200;
		//ms_time = (ms_time + 1) % 10;
		if(ms_time % 1000 == 0) {
			pos = (pos + 1) % 2;
			/*
			for (int i = 0; i < 8; i++) {
				servos[i].setAngle(positions0[i][pos]);
			}*/

			second = (second + 1) % 10;
			flag_A = false;
			flag_B = false;
			flag_C = false;
			flag_D = false;
			for (int i = 0; i < 5; i++) {
				if (uart_buffer[i] == 'L') {
					flag_A = true;
					break;
				} else if (uart_buffer[i] == 'M') {
					flag_B = true;
					break;
				} else if (uart_buffer[i] == 'R') {
					flag_C = true;
					break;
				} else if (uart_buffer[i] == 'O' && uart_buffer[i + 1] == 'D') {
					flag_D = true;
					break;
				}
			}
			if (flag_A) {
				for (int i = 0; i < 6; i++) {
					//arms[i].setAngle(positions_arm[i][pos]);
				}
				for (int i = 0; i < 4; i++) {
					//legs[i].setAngle(down[i][pos]);
				}
				right_arm_up -= 9;
				left_arm_up += 9;				
				arms[2].setAngle(right_arm_up);
				arms[5].setAngle(left_arm_up);
				head_angle -= 0.1;
				//arms[0].setAngle(90);
				//arms[3].setAngle(90);
				//arms[1].setAngle(180);
				//arms[4].setAngle(0);

			} else if (flag_B) {
				for (int i = 0; i < 6; i++) {
					//arms[i].setAngle(positions_arm1[i][pos]);
				}
				right_arm_up += 9;
				left_arm_up -= 9;				
				arms[2].setAngle(right_arm_up);
				arms[5].setAngle(left_arm_up);
				//arms[0].setAngle(90);
				//arms[3].setAngle(90);
				//arms[1].setAngle(180);
				//arms[4].setAngle(0);

			} else if (flag_C) {
				right_arm_1 += 9;
				right_arm_2 += 9;
				left_arm_1 -= 9;
				left_arm_2 -= 9;				
				head_angle += 0.1;
				arms[0].setAngle(right_arm_1);
				arms[1].setAngle(right_arm_2);
				arms[3].setAngle(left_arm_1);
				arms[4].setAngle(left_arm_2);

		    }  else if (flag_D) {
				right_arm_1 -= 9;
				right_arm_2 -= 9;
				left_arm_1 += 9;
				left_arm_2 += 9;				
				arms[0].setAngle(right_arm_1);
				arms[1].setAngle(right_arm_2);
				arms[3].setAngle(left_arm_1);
				arms[4].setAngle(left_arm_2);

		    } else {
				for (int i = 0; i < 6; i++) {
					//arms[i].setAngle(positions_arm0[i][pos]);
				}
				for (int i = 0; i < 4; i++) {
					//legs[i].setAngle(positions_legs0[i][pos]);
				}
			}
			if (right_arm_up < 0) right_arm_up = 0;
			if (right_arm_up > 180) right_arm_up = 180;
			if (left_arm_up < 0) left_arm_up = 0;
			if (left_arm_up > 180) left_arm_up = 180;
			
			if (right_arm_1 < 0) right_arm_1 = 0;
			if (right_arm_1 > 144) right_arm_1 = 144;
			if (right_arm_2 < 0) right_arm_2 = 0;
			if (right_arm_2 > 180) right_arm_2 = 180;
			
			if (left_arm_1 < 36) left_arm_1 = 36;
			if (left_arm_1 > 180) left_arm_1 = 180;
			if (left_arm_2 < 0) left_arm_2 = 0;
			if (left_arm_2 > 180) left_arm_2 = 180;
			if (head_angle < 5) head_angle = 5;
			if (head_angle > 25) head_angle = 25;
		}
		
		ms_time = (ms_time + 1) % 10000;
		for (int i = 0; i < 4; i++) {
			// legs[i].exe();
		}

		for (int i = 0; i < 6; i++) {
			// arms[i].exe();
		}
		control_legs();
	}
}
// s + data + LRC_Check + e
// 0-------------l-2-----l-1
uint8_t length = 5;
uint8_t recvStart;
uint8_t bufferCnt = 0;
uint8_t LRC_Check;
uint8_t receivingCnt;
uint16_t receiveCnt;
uint8_t key_val = 0;



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	/*
	if (recvStart) {
		if (bufferCnt == length - 1) {
			uart_buffer[bufferCnt] = recvTemp;
			bufferCnt = 0;
			recvStart = 0;
		} else if (bufferCnt > length) {
			bufferCnt = 0;
			recvStart = 0;
		} else {
			uart_buffer[bufferCnt] = recvTemp;
			bufferCnt++;
		}
	} else {
		if (recvTemp == 'D') {
			recvStart = 1;
			bufferCnt = 0;
			uart_buffer[0] = recvTemp;
			bufferCnt++;
		}
	}
	*/
	/*
	uart_buffer[bufferCnt] = recvTemp;
	bufferCnt++;
	HAL_UART_Receive_DMA(&huart3, &recvTemp, 1);
	*/
	HAL_UART_Receive_DMA(&huart3, uart_buffer, 1);
	// HAL_UART_Receive_DMA(&huart3, &recvTemp, 1);
}

void control_legs() {
	if(time < 15) {
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
	}
	else {
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
	}
	/*
	if(time < 14) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	}
	if(time < 15) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	}
	if(time < 14) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
	}
	
	if(time < 16) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	}
	
	if(time < 15) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	}
	if(time < 15) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	}
	*/
	if(time < 17) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	}
	if(time < 13) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	}
	if(time < head_angle) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
	}
}
