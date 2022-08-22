//
// Created by Muzaffer on 22.08.2022.
//

#ifndef RFID_ESP8266_H
#define RFID_ESP8266_H


#include "main.h"
#include <string.h>
#include <ctype.h>

HAL_StatusTypeDef ESP8266_TransmitReceive(UART_HandleTypeDef *huart, uint8_t *txbuffer, uint8_t *rxbuffer, uint8_t txbuflen, uint8_t rxbuflen, uint32_t timeout);

#endif //RFID_ESP8266_H
