#include "main.h"
#include "MFRC522.h"
#include "ssd1306.h"

#include "stm32f4xx_hal_uart.h"
#include "ESP8266.h"

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define	uchar	unsigned char
#define	uint	unsigned int

I2C_HandleTypeDef hi2c1;
SPI_HandleTypeDef hspi1;
UART_HandleTypeDef huart4;



char get;

int l;



void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C1_Init(void);
static void MX_UART4_Init(void);

uint32_t cursor_x = 1;
uint32_t cursor_y = 1;

void print_line(uint8_t *String)
{
    ssd1306_Fill(0);
    ssd1306_UpdateScreen();
    char *line = (char *)malloc(sizeof(char) * 20);
    if (line == NULL)
    {
        return;
    }

    for (uint8_t cursor = 0, line_num = 1; line_num <= 6; cursor+= 19, line_num++)
    {

        strncpy(line, String + cursor, 19);
        line[19] = '\0';

        ssd1306_SetCursor(cursor_x,cursor_y);
        ssd1306_WriteString(line, Font_6x8, 1);
        ssd1306_UpdateScreen();
        cursor_y += 10;

        /*
        if ((line_num - 1) * cursor > strlen(String))
        {
            break;
        }
         */

    }
    HAL_Delay(300);
    cursor_x = 1;
    cursor_y = 1;
}



int main(void)
{

    uchar status, str[MAX_LEN], findex=0,mystr[16];

    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_I2C1_Init();
    MX_UART4_Init();
    ssd1306_Init();
    MFRC522_Init();
    GPIOA->ODR |= 1<<4;
    unsigned char transmit_buf[100] = "AT\r\n";
    unsigned char receive_buf[2000];



    ESP8266_TransmitReceive(&huart4, transmit_buf, receive_buf, sizeof(transmit_buf), sizeof(receive_buf), 2000);
    print_line(receive_buf);

    strcpy((char *)transmit_buf,"AT+GMR\r\n");
    ESP8266_TransmitReceive(&huart4, transmit_buf, receive_buf, sizeof(transmit_buf), sizeof(receive_buf), 2000);
    print_line(receive_buf);

    strcpy((char *)transmit_buf,"AT+CIPSERVER=0\r\n");
    ESP8266_TransmitReceive(&huart4, transmit_buf, receive_buf, sizeof(transmit_buf), sizeof(receive_buf), 2000);
    print_line(receive_buf);


    strcpy(transmit_buf,"AT+RST\r\n");
    ESP8266_TransmitReceive(&huart4, transmit_buf, receive_buf, sizeof(transmit_buf), sizeof(receive_buf), 2000);
    print_line(receive_buf);


    strcpy(transmit_buf,"AT+RESTORE\r\n");
    ESP8266_TransmitReceive(&huart4, transmit_buf, receive_buf, sizeof(transmit_buf), sizeof(receive_buf), 2000);
    print_line(receive_buf);


    strcpy(transmit_buf,"AT+CWMODE?\r\n");
    ESP8266_TransmitReceive(&huart4, transmit_buf, receive_buf, sizeof(transmit_buf), sizeof(receive_buf), 2000);
    print_line(receive_buf);


    strcpy(transmit_buf,"AT+CWMODE=3\r\n");
    ESP8266_TransmitReceive(&huart4, transmit_buf, receive_buf, sizeof(transmit_buf), sizeof(receive_buf), 2000);
    print_line(receive_buf);


    strcpy(transmit_buf,"AT+CWMODE?\r\n");
    ESP8266_TransmitReceive(&huart4, transmit_buf, receive_buf, sizeof(transmit_buf), sizeof(receive_buf), 2000);
    print_line(receive_buf);


    strcpy(transmit_buf,"AT+CWJAP=\"muz\",\"12345678\"\r\n");
    ESP8266_TransmitReceive(&huart4, transmit_buf, receive_buf, sizeof(transmit_buf), sizeof(receive_buf), 5000);
    print_line(receive_buf);


    strcpy(transmit_buf,"AT+CIFSR\r\n");
    ESP8266_TransmitReceive(&huart4, transmit_buf, receive_buf, sizeof(transmit_buf), sizeof(receive_buf), 2000);
    print_line(receive_buf);


    strcpy(transmit_buf,"AT+CIFSR\r\n");
    ESP8266_TransmitReceive(&huart4, transmit_buf, receive_buf, sizeof(transmit_buf), sizeof(receive_buf), 2000);
    print_line(receive_buf);


    strcpy(transmit_buf,"AT+CIFSR\r\n");
    ESP8266_TransmitReceive(&huart4, transmit_buf, receive_buf, sizeof(transmit_buf), sizeof(receive_buf), 2000);
    print_line(receive_buf);


    strcpy(transmit_buf,"AT+CIFSR\r\n");
    ESP8266_TransmitReceive(&huart4, transmit_buf, receive_buf, sizeof(transmit_buf), sizeof(receive_buf), 2000);
    print_line(receive_buf);


    strcpy(transmit_buf,"AT+CIPSTART=\"TCP\",\"192.168.223.24\",8080\r\n");
    ESP8266_TransmitReceive(&huart4, transmit_buf, receive_buf, sizeof(transmit_buf), sizeof(receive_buf), 10000);
    print_line(receive_buf);

    while (1)
    {


        findex++;
        status = MFRC522_Request(PICC_REQIDL, str);

        if (status == MI_OK)
        {

        }


        status = MFRC522_Anticoll(str);
        if (status == MI_OK)
        {

            sprintf((char *)mystr,"%d:%d:%d:%d",(uchar)str[0],(uchar)str[1],(uchar)str[2],(uchar)str[3]);
            ssd1306_SetCursor(3, 10);
            ssd1306_WriteString(mystr, Font_6x8, 1);
            ssd1306_UpdateScreen();

            if (strstr(receive_buf, mystr) != NULL)
            {
                ssd1306_WriteString("AAAAAAAAAAAAAAA", Font_6x8, 1);
                ssd1306_UpdateScreen();
            }

        }
        else
        {

        }
    }
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
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
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
