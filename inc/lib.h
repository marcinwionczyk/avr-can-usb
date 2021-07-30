//
// Created by marcin on 27.07.2021.
//

#ifndef AVR_CAN_USB_LIB_H
#define AVR_CAN_USB_LIB_H
#include <stdint.h>

unsigned char hexCharToByte(char hex);

uint8_t ascii2byte (const uint8_t * val);

uint8_t nibble2ascii(uint8_t nibble);

#endif //AVR_CAN_USB_LIB_H
