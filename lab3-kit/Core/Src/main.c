/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "config.h"
#include "fixedpt.h"

/**
  * @brief  The application entry point.
  * @retval int
  */
TIM_HandleTypeDef htim16;
uint16_t start_time, current_time;
float sin_lut[32]; // TODO: Swap this to double
int length = 100;

float square(float x) {
  float sum = 0;
  for (int i = 0; i < length; i++) {
    sum += (float)1/(2 * i + 1) * sin((2 * i + 1) * x);
  }
  return (float)4 / M_PI * sum;
}

fixedpt square_fixedpt(fixedpt x) {
  fixedpt sum = 0;
  for (int i = 0; i < length; i++) {
    sum = FXD_ADD(
      sum, 
      FXD_MUL(
        FXD_FROM_INT(1)/FXD_FROM_INT(2 * i + 1),
        FXD_FROM_FLOAT(sin(FXD_TO_FLOAT(FXD_MUL(
          FXD_FROM_INT(2 * i + 1), 
          x
        ))))
      )
    );
  }
  return FXD_MUL(FXD_DIV(FIXEDPT_PI, FXD_FROM_INT(4)), sum);
}

float sawtooth(float x) {
  float sum = 0;
  for (int i = 1; i <= length; i++) {
    sum += (float)1 / i * sin(i * x);
  }
  return (float)1/2 - sum;
}

fixedpt sawtooth_fixedpt(fixedpt x) {
  fixedpt sum = 0;
  for (int i = 1; i <= length; i++) {
    sum = FXD_ADD(
      sum,
      FXD_DIV(
        FIXEDPT_ONE, 
        FXD_MUL(FXD_FROM_INT(i), FXD_FROM_FLOAT(sin(FXD_FROM_INT(i) * x)))
      )
    );
  }
  return FXD_SUB(FXD_DIV(FIXEDPT_ONE, FXD_FROM_INT(2)), sum);
}

float triangular(float x) {
  float sum = 0;
  for (int i = 0; i < length; i++) {
    if (i % 2 == 0) {
      sum += sin((2 * i + 1) * x) / ((2 * i + 1) * (2 * i + 1));
    } else {
      sum -= sin((2 * i + 1) * x) / ((2 * i + 1) * (2 * i + 1));
    }
  }
  return 8 / (M_PI * M_PI) * sum;
}

fixedpt triangular_fixedpt(fixedpt x) {
  fixedpt sum = 0;
  for (int i = 0; i < length; i++) {
    if (i % 2 == 0) {
      sum = FXD_ADD(
        sum,
        FXD_DIV(
          FXD_FROM_FLOAT(
            sin(FXD_MUL(
              (FXD_ADD(
                FXD_MUL(FXD_FROM_INT(2) * FXD_FROM_INT(i)),
                FIXEDPT_ONE
              )), 
              x
            ))
          ),
          ((2 * i + 1) * (2 * i + 1))
        )
      );
      
    } else {
      sum = FXD_SUB(
        sum,
        FXD_DIV(
          FXD_FROM_FLOAT(
            sin(FXD_MUL(
              (FXD_ADD(
                FXD_MUL(FXD_FROM_INT(2) * FXD_FROM_INT(i)),
                FIXEDPT_ONE
              )), 
              x
            ))
          ),
          ((2 * i + 1) * (2 * i + 1))
        )
      );
    }
  }
  return 8 / (M_PI * M_PI) * sum;
}

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DAC_Init();
  MX_TIM6_Init();
  MX_USART3_Init();


  // Part 1
  HAL_TIM_Base_Start(&htim16);
  start_time = __HAL_TIM_GET_COUNTER(&htim16);

  uint16_t sum = 0;
  for (uint16_t i = 0; i < 100; i++) {
    sum += i;
  }
  current_time = __HAL_TIM_GET_COUNTER(&htim16);
  uint16_t duration = current_time - start_time;

  printf("%u\n", (unsigned int)duration);

  
    uint16_t i = 0;
    start_time = __HAL_TIM_GET_COUNTER(&htim16);
    while (i * 0.01 <= M_PI  * 2) {
      float sin_i = sin(i); // TODO: Change to floating point
      printf("%u\n", (unsigned int)i);
      i++;
      // sin_lut[i] = sin(i); // TODO: Add for assignment 2
    }
    current_time = __HAL_TIM_GET_COUNTER(&htim16);
    duration = current_time - start_time;

    printf("%u\n", (unsigned int)duration);
  // }

}
