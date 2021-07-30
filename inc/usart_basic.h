/**
 * \file
 *
 * \brief USART basic driver.
 *
 (c) 2020 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

#ifndef USART_BASIC_H_INCLUDED
#define USART_BASIC_H_INCLUDED

#include <atmel_start.h>
#include <stdbool.h>

/* USART_0 Ringbuffer */

#define USART_0_RX_BUFFER_SIZE 32
#define USART_0_TX_BUFFER_SIZE 32
#define USART_0_RX_BUFFER_MASK (USART_0_RX_BUFFER_SIZE - 1)
#define USART_0_TX_BUFFER_MASK (USART_0_TX_BUFFER_SIZE - 1)

typedef enum { RX_CB = 1, UDRE_CB } usart_cb_type_t;
typedef void (*usart_cb_t)(void);

int8_t USART_0_init(void);

void USART_0_enable(void);

void USART_0_enable_rx(void);

void USART_0_enable_tx(void);

void USART_0_disable(void);

uint8_t USART_0_get_data(void);

bool USART_0_is_tx_ready(void);

bool USART_0_is_rx_ready(void);

bool USART_0_is_tx_busy(void);

uint8_t USART_0_read(void);

void USART_0_write(uint8_t data);

void USART_0_set_ISR_cb(usart_cb_t cb, usart_cb_type_t type);

#endif /* USART_BASIC_H_INCLUDED */
