/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "pdm2pcm.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
#include "FreeRTOS.h"
#include"string.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "task.h"
#include "semphr.h"
extern"C"
{
	#include "ILI9341_STM32_Driver.h"
	#include "ILI9341_GFX.h"
}
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/** Audio buffers, pointers and selectors */
typedef struct
{
  int16_t *buffers[2];
  uint8_t buf_select;
  volatile uint8_t buf_ready;
  volatile uint32_t buf_count;
  uint32_t n_samples;
} inference_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PDM_BUF_LEN 256
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

I2S_HandleTypeDef hi2s2;
DMA_HandleTypeDef hdma_spi2_rx;

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

TIM_HandleTypeDef htim14;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
// Settings
static bool debug_nn = false; // Set this to true to see e.g. features generated from the raw signal

// Globals
static inference_t inference;
static volatile bool record_ready = false;
uint16_t PDMBuf[PDM_BUF_LEN];
uint16_t PCMBuf[PDM_BUF_LEN/8];
uint8_t rxstate = 0;
SemaphoreHandle_t Key;
ei_impulse_result_t result = { 0 };
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2S2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_CRC_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM14_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void LCD(void *pvParameters);
void Keyword_Spotting(void *pvParameters);
static int get_audio_signal_data(size_t offset, size_t length, float *out_ptr);
static void audio_buffer_inference_callback(uint32_t n_bytes, uint32_t offset);
bool ei_microphone_inference_record(void);
bool ei_microphone_inference_end(void);
void ei_printf(const char *format, ...);
void vprint(const char *fmt, va_list argp);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2S2_Init();
  MX_USART2_UART_Init();
  MX_CRC_Init();
  MX_TIM14_Init();
  MX_PDM2PCM_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  Key=xSemaphoreCreateBinary();
  xSemaphoreGive( Key );
  TaskHandle_t HandleKWS =NULL;
  xTaskCreate(Keyword_Spotting, "Keyword_Spotting", 1024, NULL, 1,&HandleKWS);
  TaskHandle_t HandleLCD =NULL;
  xTaskCreate(LCD, "LCD",512, NULL, 1,&HandleLCD);
  vTaskStartScheduler();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 144;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_CRC_DR_RESET(&hcrc);
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief I2S2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S2_Init(void)
{

  /* USER CODE BEGIN I2S2_Init 0 */

  /* USER CODE END I2S2_Init 0 */

  /* USER CODE BEGIN I2S2_Init 1 */

  /* USER CODE END I2S2_Init 1 */
  hi2s2.Instance = SPI2;
  hi2s2.Init.Mode = I2S_MODE_MASTER_RX;
  hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s2.Init.DataFormat = I2S_DATAFORMAT_24B;
  hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
  hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_32K;
  hi2s2.Init.CPOL = I2S_CPOL_LOW;
  hi2s2.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S2_Init 2 */

  /* USER CODE END I2S2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 8400 - 1;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 65535;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LCD_CS_Pin|LCD_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : OTG_FS_PowerSwitchOn_Pin LCD_CS_Pin LCD_DC_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin|LCD_CS_Pin|LCD_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_RST_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                           Audio_RST_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/**
 * @brief      Inferencing Task
 *
 * @return     void
 */
void LCD(void *pvParameters)
{
	ILI9341_Init();
	int oldx=0;
	int oldy=0;
	int randx=0;
	int randy=0;
	int min_x = 5;
	int max_x = 150;
	int min_y = 152;
	int max_y = 300;
	int time=0;
	static char BufferText[50];
	float max=0;
	size_t max_id;
	int print_results = -(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW);
	ILI9341_FillScreen(WHITE);
	srand(time);
	while(1)
	{
		   if( xSemaphoreTake(Key, portMAX_DELAY ) == pdTRUE)
		   {
//			ei_printf("LCD Take\n\r\r\n");
			if(++print_results >= (EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW ))
			{
			  ILI9341_SetRotation(SCREEN_VERTICAL_2);
			  // Comment this section out if you don't want to see the raw scores
			  sprintf(BufferText,"Result:");
			  ILI9341_DrawText(BufferText, FONT3, 1, 10, BLACK, WHITE);
			  sprintf(BufferText,"(MFCC: %dms, NN: %dms)", result.timing.dsp, result.timing.classification);
			  ILI9341_DrawText(BufferText, FONT3, 1, 30, BLACK, WHITE);
			  for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++)
			  {
				  sprintf(BufferText," %s: %.5f     ", result.classification[ix].label, result.classification[ix].value*100);
				  ILI9341_DrawText(BufferText, FONT3, 1, 50+(ix*20), BLACK, WHITE);
			      if (result.classification[ix].value> max)
			      {
			           max = result.classification[ix].value;
			           max_id=ix;
			      }
			  }

//			  ILI9341_DrawHLine(0, 120, 240, NAVY);
			  sprintf(BufferText,"Predict: %s         ",result.classification[max_id].label);
			  ILI9341_DrawText(BufferText, FONT4,40, 130,RED,WHITE);
			  ILI9341_DrawHLine(0, 150, 240, NAVY);
			  if(max_id==(size_t)2)
			  {
				  sprintf(BufferText,"          ");
				  ILI9341_DrawText(BufferText, FONT4,oldx, oldy ,WHITE,WHITE);
				  for(int i=0;i<3;i++)
				  {
					  unsigned int color = rand() % (0xF81F - 0x0000 + 0x0001) + 0x0000;
					  ILI9341_DrawFilledCircle(120,245, 20+10*i, color);
				  }
				  ILI9341_DrawFilledCircle(120,245, 45, WHITE);
			  }
			  else
			  {
				  sprintf(BufferText,"          ");
				  ILI9341_DrawText(BufferText, FONT4,oldx, oldy ,WHITE,WHITE);
				  randx = rand() % (max_x - min_x + 1) + min_x;
				  randy = rand() % (max_y - min_y + 1) + min_y;
				  sprintf(BufferText,"Wake up ??");
				  unsigned int color = rand() % (0xF81F - 0x0000 + 0x0001) + 0x0000;
				  ILI9341_DrawText(BufferText, FONT4,randx, randy ,color,WHITE);
				  oldx=randx;
				  oldy=randy;
			  }
			  print_results = 0;
			  max=0;
			}
			   if( xSemaphoreGive( Key ) != pdTRUE )
			   {
					ei_printf("LCD Task Give Fail\r\n");
				   // We would not expect this call to fail because we must have
				   // obtained the semaphore to get here.
			   }
			   time++;
			   if(time==5000)
				   time=0;
				vTaskDelay(1);
		   }
	}
}
void Keyword_Spotting(void *pvParameters)
{
	//Declare
	HAL_StatusTypeDef hal_res;
//	int print_results = -(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW);
	ei_printf("Inferencing settings:\r\n");
	ei_printf("\tInterval: %.2f ms.\r\n", (float)EI_CLASSIFIER_INTERVAL_MS);
	ei_printf("\tFrame size: %d\r\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
	ei_printf("\tSample length: %d ms.\r\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT / 16);
	ei_printf("\tNo. of classes: %d\r\n", sizeof(ei_classifier_inferencing_categories) / sizeof(ei_classifier_inferencing_categories[0]));
	// Create double buffer
	inference.buffers[0] = (int16_t *)pvPortMalloc(EI_CLASSIFIER_SLICE_SIZE * sizeof(int16_t));
	if(inference.buffers[0] == NULL)
	{
	  ei_printf("ERROR: Could not create buffer 1. Likely ran out of heap memory.\r\n");
	}
	  inference.buffers[1] = (int16_t *)pvPortMalloc(EI_CLASSIFIER_SLICE_SIZE * sizeof(int16_t));
	if(inference.buffers[1] == NULL)
	{
	  ei_printf("ERROR: Could not create buffer 2. Likely ran out of heap memory.\r\n");
	}
	  // Set inference parameters
	  inference.buf_select = 0;
	  inference.buf_count  = 0;
	  inference.n_samples  = EI_CLASSIFIER_SLICE_SIZE;
	  inference.buf_ready  = 0;

	  // Start receiving I2S audio data
	  hal_res =   HAL_I2S_Receive_DMA(&hi2s2, &PDMBuf[0],PDM_BUF_LEN/2);
	  if (hal_res != HAL_OK)
	  {
	    ei_printf("ERROR: Could not initialize I2S microphone.\r\n");
	  }
	  // Start doing inference
	  record_ready = true;

	  if (record_ready)
	  {
	    ei_printf("Listening!\r\n");
	  }
	  while(1)
	  {
		   if( xSemaphoreTake(Key, portMAX_DELAY ) == pdTRUE)
		   {
//		    ei_printf("Inference Take\n\r\r\n");
			bool m = ei_microphone_inference_record();
			if (!m)
			{
			  ei_printf("ERROR: Audio buffer overrun\r\n");
			  break;
			}
			// Do classification (i.e. the inference part)
			signal_t signal;
			signal.total_length = EI_CLASSIFIER_SLICE_SIZE;
			signal.get_data = &get_audio_signal_data;
			EI_IMPULSE_ERROR r = run_classifier_continuous(&signal, &result, debug_nn);
			if (r != EI_IMPULSE_OK)
			{
			ei_printf("ERROR: Failed to run classifier (%d)\r\n", r);
			break;
			}
			if (result.classification[2].value > 0.4)
			{
			  ei_printf("Yes!!!\r\n");
			  ei_printf("Confidence:%.5f\r\n",result.classification[(size_t)2].value);
			}
		   if( xSemaphoreGive( Key ) != pdTRUE )
		   {
				ei_printf("KeyWord Spotting Task Give Fail\r\n");
			   // We would not expect this call to fail because we must have
			   // obtained the semaphore to get here.
		   }
			vTaskDelay(1);
		   }

	  }
}
/**
 * @brief      Wait for a full buffer
 *
 * @return     In case of an buffer overrun return false
 */
bool ei_microphone_inference_record(void)
{
  bool ret = true;

  // Check to see if the buffer has overrun
  if (inference.buf_ready == 1) {
      ret = false;
  }

  // %%%TODO: make this non-blocking
  while (inference.buf_ready == 0)
  {
	    if (rxstate==1) {
	    	PDM_Filter(&PDMBuf[0],&PCMBuf[0], &PDM1_filter_handler);
	    	audio_buffer_inference_callback(PDM_BUF_LEN/8, 0);
	    	rxstate=0;

	    }

	    if (rxstate==2) {
	    	PDM_Filter(&PDMBuf[PDM_BUF_LEN/2],&PCMBuf[0], &PDM1_filter_handler);
	    	audio_buffer_inference_callback(PDM_BUF_LEN/8, 0);
	    	rxstate=0;
	    }
//    continue;
  }

  inference.buf_ready = 0;

  return ret;
}

/**
 * @brief      Stop audio sampling, release sampling buffers
 *
 * @return     false on error
 */
bool ei_microphone_inference_end(void)
{
  // Stop I2S
  HAL_I2S_DMAStop(&hi2s2);

  // Free up double buffer
  record_ready = false;
  free(inference.buffers[0]);
  free(inference.buffers[1]);

  return true;
}

/**
 * @brief      Copy sample data in selected buf and signal ready when buffer is full
 *
 * @param[in]  n_bytes  Number of bytes to copy
 * @param[in]  offset   offset in sampleBuffer
 */
static void audio_buffer_inference_callback(uint32_t n_samples, uint32_t offset)
{
  // Copy samples from I2S buffer to inference buffer. Convert 24-bit, 32kHz
  // samples to 16-bit, 16kHz
	  for (uint32_t i = 0; i < n_samples; i++) {
	    inference.buffers[inference.buf_select][inference.buf_count++] =
	        (int16_t)(PCMBuf[i]);
      if (inference.buf_count >= inference.n_samples) {
      inference.buf_select ^= 1;
      inference.buf_count = 0;
      inference.buf_ready = 1;
    }
  }
//ei_printf("Inference Buffer:%d/%d                  \r",inference.buf_count,inference.n_samples);
}

/**
 * Get raw audio signal data
 */
static int get_audio_signal_data(size_t offset, size_t length, float *out_ptr)
{
  numpy::int16_to_float(&inference.buffers[inference.buf_select ^ 1][offset], out_ptr, length);

  return 0;
}

/**
 * Low-level print function that uses UART to print status messages.
 */
void vprint(const char *fmt, va_list argp)
{
  char string[200];
  if(0 < vsprintf(string, fmt, argp)) // build string
  {
      HAL_UART_Transmit(&huart2, (uint8_t*)string, strlen(string), 0xffffff);
  }
}

/**
 * Wrapper for vprint. Use this like you would printf to print messages to the serial console.
 */
void ei_printf(const char *format, ...)
{
  va_list myargs;
  va_start(myargs, format);
  vprint(format, myargs);
  va_end(myargs);
}

/**
 * Called when the first half of the receive buffer is full
 */
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
	rxstate = 1;
//  if (record_ready == true)
//  {
////	ei_printf("First_half FULL\r\n");
//    audio_buffer_inference_callback(I2S_BUF_LEN / 2, 0);
//  }
}

/**
 * Called when the second half of the receive buffer is full
 */
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	rxstate = 2;
//HAL_StatusTypeDef hal_res;
//  if (record_ready == true)
//  {
////	ei_printf("Second_half FULL\r\n");
//    audio_buffer_inference_callback(I2S_BUF_LEN / 2, I2S_BUF_LEN / 2);
//    HAL_I2S_DMAPause(&hi2s2);
//    HAL_I2S_DMAResume(&hi2s2);
////    hal_res =  HAL_I2S_Receive_DMA(&hi2s2, i2s_buf, (I2S_BUF_LEN / ELEMENTS_PER_WORD));
////    if (hal_res != HAL_OK)
////    {
////      ei_printf("ERROR: Could not initialize I2S microphone 11.\r\n");
////    }
//  }
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

