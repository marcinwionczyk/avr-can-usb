//
// Created by marcin on 15.07.2021.
//

#ifndef AVR_CAN_USB_SYSTEM_H
#define AVR_CAN_USB_SYSTEM_H

#include "port.h"
#include <avr/sleep.h>
#include <avr/interrupt.h>


/** Function pointer to callback function called by SPI IRQ.
    NULL=default value: No callback function is to be used.
*/
typedef void (*spi_transfer_done_cb_t)(void);

/** Specify whether the SPI transfer is to be uni- or bidirectional.
    A bidirectional transfer (=SPI_EXCHANGE) causes the received data
    to overwrite the buffer with the data to transmit.
*/
typedef enum spi_transfer_type {
    SPI_EXCHANGE, ///< SPI transfer is bidirectional
    SPI_READ,     ///< SPI transfer reads, writes only 0s
    SPI_WRITE     ///< SPI transfer writes, discards read data
} spi_transfer_type_t;

/** Status of the SPI hardware and SPI bus.*/
typedef enum spi_transfer_status {
    SPI_FREE, ///< SPI hardware is not open, bus is free.
    SPI_IDLE, ///< SPI hardware has been opened, no transfer ongoing.
    SPI_BUSY, ///< SPI hardware has been opened, transfer ongoing.
    SPI_DONE  ///< SPI hardware has been opened, transfer complete.
} spi_transfer_status_t;

#define USART_0_RX_BUFFER_SIZE 256
#define USART_0_TX_BUFFER_SIZE 256
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


/**
 * \brief Set TX2RTS pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void TX2RTS_set_pull_mode(const enum port_pull_mode pull_mode)
{
    PORTA_set_pin_pull_mode(0, pull_mode);
}

/**
 * \brief Set TX2RTS data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void TX2RTS_set_dir(const enum port_dir dir)
{
    PORTA_set_pin_dir(0, dir);
}

/**
 * \brief Set TX2RTS level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void TX2RTS_set_level(const bool level)
{
    PORTA_set_pin_level(0, level);
}

/**
 * \brief Toggle output level on TX2RTS
 *
 * Toggle the pin level
 */
static inline void TX2RTS_toggle_level(void)
{
    PORTA_toggle_pin_level(0);
}

/**
 * \brief Get level on TX2RTS
 *
 * Reads the level on a pin
 */
static inline bool TX2RTS_get_level(void)
{
    return PORTA_get_pin_level(0);
}

/**
 * \brief Set TX1RTS pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void TX1RTS_set_pull_mode(const enum port_pull_mode pull_mode)
{
    PORTA_set_pin_pull_mode(1, pull_mode);
}

/**
 * \brief Set TX1RTS data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void TX1RTS_set_dir(const enum port_dir dir)
{
    PORTA_set_pin_dir(1, dir);
}

/**
 * \brief Set TX1RTS level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void TX1RTS_set_level(const bool level)
{
    PORTA_set_pin_level(1, level);
}

/**
 * \brief Toggle output level on TX1RTS
 *
 * Toggle the pin level
 */
static inline void TX1RTS_toggle_level(void)
{
    PORTA_toggle_pin_level(1);
}

/**
 * \brief Get level on TX1RTS
 *
 * Reads the level on a pin
 */
static inline bool TX1RTS_get_level(void)
{
    return PORTA_get_pin_level(1);
}

/**
 * \brief Set TX0RTS pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void TX0RTS_set_pull_mode(const enum port_pull_mode pull_mode)
{
    PORTA_set_pin_pull_mode(2, pull_mode);
}

/**
 * \brief Set TX0RTS data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void TX0RTS_set_dir(const enum port_dir dir)
{
    PORTA_set_pin_dir(2, dir);
}

/**
 * \brief Set TX0RTS level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void TX0RTS_set_level(const bool level)
{
    PORTA_set_pin_level(2, level);
}

/**
 * \brief Toggle output level on TX0RTS
 *
 * Toggle the pin level
 */
static inline void TX0RTS_toggle_level(void)
{
    PORTA_toggle_pin_level(2);
}

/**
 * \brief Get level on TX0RTS
 *
 * Reads the level on a pin
 */
static inline bool TX0RTS_get_level(void)
{
    return PORTA_get_pin_level(2);
}

/**
 * \brief Set RX1BF pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void RX1BF_set_pull_mode(const enum port_pull_mode pull_mode)
{
    PORTB_set_pin_pull_mode(0, pull_mode);
}

/**
 * \brief Set RX1BF data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void RX1BF_set_dir(const enum port_dir dir)
{
    PORTB_set_pin_dir(0, dir);
}

/**
 * \brief Set RX1BF level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void RX1BF_set_level(const bool level)
{
    PORTB_set_pin_level(0, level);
}

/**
 * \brief Toggle output level on RX1BF
 *
 * Toggle the pin level
 */
static inline void RX1BF_toggle_level(void)
{
    PORTB_toggle_pin_level(0);
}

/**
 * \brief Get level on RX1BF
 *
 * Reads the level on a pin
 */
static inline bool RX1BF_get_level(void)
{
    return PORTB_get_pin_level(0);
}

/**
 * \brief Set RX0BF pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void RX0BF_set_pull_mode(const enum port_pull_mode pull_mode)
{
    PORTB_set_pin_pull_mode(1, pull_mode);
}

/**
 * \brief Set RX0BF data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void RX0BF_set_dir(const enum port_dir dir)
{
    PORTB_set_pin_dir(1, dir);
}

/**
 * \brief Set RX0BF level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void RX0BF_set_level(const bool level)
{
    PORTB_set_pin_level(1, level);
}

/**
 * \brief Toggle output level on RX0BF
 *
 * Toggle the pin level
 */
static inline void RX0BF_toggle_level(void)
{
    PORTB_toggle_pin_level(1);
}

/**
 * \brief Get level on RX0BF
 *
 * Reads the level on a pin
 */
static inline bool RX0BF_get_level(void)
{
    return PORTB_get_pin_level(1);
}

/**
 * \brief Set INT pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void INT_set_pull_mode(const enum port_pull_mode pull_mode)
{
    PORTB_set_pin_pull_mode(2, pull_mode);
}

/**
 * \brief Set INT data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void INT_set_dir(const enum port_dir dir)
{
    PORTB_set_pin_dir(2, dir);
}

/**
 * \brief Set INT level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void INT_set_level(const bool level)
{
    PORTB_set_pin_level(2, level);
}

/**
 * \brief Toggle output level on INT
 *
 * Toggle the pin level
 */
static inline void INT_toggle_level(void)
{
    PORTB_toggle_pin_level(2);
}

/**
 * \brief Get level on INT
 *
 * Reads the level on a pin
 */
static inline bool INT_get_level(void)
{
    return PORTB_get_pin_level(2);
}

/**
 * \brief Set RESET pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void RESET_set_pull_mode(const enum port_pull_mode pull_mode)
{
    PORTB_set_pin_pull_mode(3, pull_mode);
}

/**
 * \brief Set RESET data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void RESET_set_dir(const enum port_dir dir)
{
    PORTB_set_pin_dir(3, dir);
}

/**
 * \brief Set RESET level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void RESET_set_level(const bool level)
{
    PORTB_set_pin_level(3, level);
}

static inline void RESET_toggle_level(void)
{
    PORTB_toggle_pin_level(3);
}

/**
 * \brief Get level on RESET
 *
 * Reads the level on a pin
 */
static inline bool RESET_get_level(void)
{
    return PORTB_get_pin_level(3);
}

/**
 * \brief Set SS pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void SS_set_pull_mode(const enum port_pull_mode pull_mode)
{
    PORTB_set_pin_pull_mode(4, pull_mode);
}

/**
 * \brief Set SS data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void SS_set_dir(const enum port_dir dir)
{
    PORTB_set_pin_dir(4, dir);
}

/**
 * \brief Set SS level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void SS_set_level(const bool level)
{
    PORTB_set_pin_level(4, level);
}

/**
 * \brief Toggle output level on SS
 *
 * Toggle the pin level
 */
static inline void SS_toggle_level(void)
{
    PORTB_toggle_pin_level(4);
}

/**
 * \brief Get level on SS
 *
 * Reads the level on a pin
 */
static inline bool SS_get_level(void)
{
    return PORTB_get_pin_level(4);
}

static inline void mcu_init(void)
{
    /* On AVR devices all peripherals are enabled from power on reset, this
     * disables all peripherals to save power. Driver shall enable
     * peripheral if used */

    PRR0 = (1 << PRSPI) | (1 << PRTIM2) | (1 << PRTIM0) | (1 << PRTIM1) | (1 << PRTWI) | (1 << PRUSART1)
           | (1 << PRUSART0) | (1 << PRADC);

    /* Set all pins to low power mode */
    PORTA_set_port_dir(0xff, PORT_DIR_OFF);
    PORTB_set_port_dir(0xff, PORT_DIR_OFF);
    PORTC_set_port_dir(0xff, PORT_DIR_OFF);
    PORTD_set_port_dir(0xff, PORT_DIR_OFF);
}

/**
 * \brief Set sleep mode to use when entering sleep state
 *
 * \param mode Sleep mode
 */
static inline void sleep_set_mode(uint8_t mode)
{
    SMCR = mode | (SMCR & ~((1 << SM0) | (1 << SM1) | (1 << SM2)));
}

extern void protected_write_io(void *addr, uint8_t magic, uint8_t value);

/*
 * \brief Initialize sysctrl interface
 *
 * \param[in] hw The pointer to hardware instance
 *
 * \return Initialization status.
 */
static inline int8_t sysctrl_init(void)
{
    /* Set up system clock prescaler according to configuration */
    protected_write_io((void *)&CLKPR, 1 << CLKPCE, (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0));

    SMCR = (0 << SM2) | (0 << SM1) | (0 << SM0) | // Idle
           (0 << SE);

    MCUCR = (0 << PUD);

    return 0;
}


int8_t EXTERNAL_IRQ_0_init(void);
int8_t SPI_init(void);
int8_t USART_init(void);
int8_t system_init(void);

#endif //AVR_CAN_USB_SYSTEM_H
