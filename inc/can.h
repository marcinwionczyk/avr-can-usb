//
// Created by marcin on 27.07.2021.
//

#include <stdint.h>

#ifndef AVR_CAN_USB_CAN_H
#define AVR_CAN_USB_CAN_H

/* special address description flags for the CAN_ID */
#define CAN_EFF_FLAG 0x80000000UL /* EFF/SFF is set in the MSB */
#define CAN_RTR_FLAG 0x40000000UL /* remote transmission request */
#define CAN_ERR_FLAG 0x20000000UL /* error message frame */

/* valid bits in CAN ID for frame formats */
#define CAN_SFF_MASK 0x000007FFUL /* standard frame format (SFF) */
#define CAN_EFF_MASK 0x1FFFFFFFUL /* extended frame format (EFF) */
#define CAN_ERR_MASK 0x1FFFFFFFUL /* omit EFF, RTR, ERR flags */

typedef uint32_t canid_t;

#define CAN_SFF_ID_BITS     11
#define CAN_EFF_ID_BITS     29

#define CAN_MAX_DLC 8
#define CAN_MAX_DLEN 8

struct can_frame {
    canid_t can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
    uint8_t can_dlc; /* frame payload length in byte (0 .. CAN_MAX_DLEN) */
    uint8_t data[CAN_MAX_DLEN] __attribute__((aligned(8)));
};

#endif //AVR_CAN_USB_CAN_H
