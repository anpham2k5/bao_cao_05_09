#include <Arduino.h>
#include "ADCTouchSensor.h"

extern "C" void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  // 1. HSI 8MHz / 2 * 12 = 48MHz
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12; // 48MHz
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  // 2. Bus APB1 = 24MHz, APB2 = 48MHz (Latency = 1)
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  // 3. ADC Clock = 48MHz / 4 = 12MHz
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  SystemCoreClockUpdate();
}

ADCTouchSensor* pressure1 = nullptr;
ADCTouchSensor* pressure2 = nullptr;

float Ref1 = 0, Ref2 = 0;
const unsigned int SAMPLES = 100;

void setup() {
  Serial1.begin(115200);
  delay(2000);
  Serial1.println("--- STM32 HSI 48MHz Khoi dong OK ---");

  pressure1 = new ADCTouchSensor(PA0, PA1, 50);
  pressure2 = new ADCTouchSensor(PA3, PA2, 50);

  analogReadResolution(12);
  
  Ref1 = pressure1->readRaw(500);
  Ref2 = pressure2->readRaw(500);

  Serial1.println("--- Bat dau do ---");
}

void loop() {
  if (pressure1 == nullptr || pressure2 == nullptr) return;

  uint32_t T1 = micros();
  float raw1 = pressure1->readRaw(SAMPLES);
  uint32_t T2 = micros();
  float raw2 = pressure2->readRaw(SAMPLES);
  uint32_t T3 = micros();

  uint32_t T_1 = T2 - T1;
  uint32_t T_2 = T3 - T2;
  float value1 = raw1 - Ref1;
  float value2 = raw2 - Ref2;

  Serial1.print(value1, 2);
  Serial1.print("    ");
  Serial1.print(value2, 2);
  Serial1.print("    ");
  Serial1.print(Ref1, 2);
  Serial1.print("    ");
  Serial1.println(Ref2, 2);

  delay(1000);
}