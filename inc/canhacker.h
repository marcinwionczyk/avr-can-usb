//
// Created by marcin on 27.07.2021.
//

#ifndef AVR_CAN_USB_CANHACKER_H
#define AVR_CAN_USB_CANHACKER_H

#include <stdio.h>
#include "can.h"
#include "mcp2515.h"

#define CAN_MIN_DLEN 1
#define HEX_PER_BYTE 2
#define MIN_MESSAGE_DATA_HEX_LENGTH CAN_MIN_DLEN * HEX_PER_BYTE
#define MAX_MESSAGE_DATA_HEX_LENGTH CAN_MAX_DLEN * HEX_PER_BYTE
#define MIN_MESSAGE_LENGTH 5

#define CANHACKER_CMD_MAX_LENGTH 26

#define CANHACKER_SERIAL_RESPONSE     "N0001\r"
#define CANHACKER_SW_VERSION_RESPONSE "v0107\r"
#define CANHACKER_VERSION_RESPONSE    "V1010\r"

enum ERROR {
    ERROR_OK,
    ERROR_CONNECTED,
    ERROR_NOT_CONNECTED,
    ERROR_UNKNOWN_COMMAND,
    ERROR_INVALID_COMMAND,
    ERROR_ERROR_FRAME_NOT_SUPPORTED,
    ERROR_BUFFER_OVERFLOW,
    ERROR_SERIAL_TX_OVERRUN,
    ERROR_LISTEN_ONLY,
    ERROR_MCP2515_INIT,
    ERROR_MCP2515_INIT_CONFIG,
    ERROR_MCP2515_INIT_BITRATE,
    ERROR_MCP2515_INIT_SET_MODE,
    ERROR_MCP2515_SEND,
    ERROR_MCP2515_READ,
    ERROR_MCP2515_FILTER,
    ERROR_MCP2515_ERRIF,
    ERROR_MCP2515_MERRF
};

void CanHacker(FILE* stream, FILE* debugStream);
void setClock(enum CAN_CLOCK clock);
enum ERROR receiveCommand(const char *buffer, int length);
enum ERROR receiveCanFrame(const struct can_frame *frame);
enum ERROR sendFrame(const struct can_frame *frame);
enum ERROR enableLoopback(void);
enum ERROR disableLoopback(void);
enum ERROR pollReceiveCan(void);
enum ERROR receiveCan(enum RXBn rxBuffer);
enum ERROR processInterrupt(void);
FILE* getInterfaceStream(void);


#endif //AVR_CAN_USB_CANHACKER_H
