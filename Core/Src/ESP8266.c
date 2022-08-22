//
// Created by Muzaffer on 22.08.2022.
//

#include "ESP8266.h"

void delete_white_space(uint8_t *rxbuffer, uint8_t rxbuflen)
{
    for (int i = 0; i < rxbuflen; ++i)
    {
        if (isspace(rxbuffer[i]) && isspace(rxbuffer[i + 1]))
        {
            strcpy((char *)rxbuffer + i, (char *)rxbuffer + i + 2);
        }
    }
}


HAL_StatusTypeDef ESP8266_TransmitReceive(UART_HandleTypeDef *huart, uint8_t *txbuffer, uint8_t *rxbuffer, uint8_t txbuflen, uint8_t rxbuflen, uint32_t timeout)
{
    memset(rxbuffer, '\0', rxbuflen);
    HAL_StatusTypeDef txstatus = HAL_UART_Transmit(huart, txbuffer, txbuflen, timeout);

    HAL_StatusTypeDef rxstatus = HAL_UART_Receive(huart, rxbuffer, rxbuflen, timeout);

    for (int i = 0; i < rxbuflen;i ++)
    {
        if (!isprint(rxbuffer[i]))
        {
            rxbuffer[i] = ' ';
        }
    }

    delete_white_space(rxbuffer, rxbuflen);

    if (txstatus == HAL_BUSY || rxstatus == HAL_BUSY)
    {
        return HAL_BUSY;
    }
    else if (txstatus == HAL_ERROR || rxstatus == HAL_ERROR)
    {
        return  HAL_ERROR;
    }

    return HAL_OK;
}