/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <port.h>

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

/**
 * \brief Toggle output level on RESET
 *
 * Toggle the pin level
 */
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

/**
 * \brief Set MOSI pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void MOSI_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTB_set_pin_pull_mode(5, pull_mode);
}

/**
 * \brief Set MOSI data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void MOSI_set_dir(const enum port_dir dir)
{
	PORTB_set_pin_dir(5, dir);
}

/**
 * \brief Set MOSI level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void MOSI_set_level(const bool level)
{
	PORTB_set_pin_level(5, level);
}

/**
 * \brief Toggle output level on MOSI
 *
 * Toggle the pin level
 */
static inline void MOSI_toggle_level(void)
{
	PORTB_toggle_pin_level(5);
}

/**
 * \brief Get level on MOSI
 *
 * Reads the level on a pin
 */
static inline bool MOSI_get_level(void)
{
	return PORTB_get_pin_level(5);
}

/**
 * \brief Set MISO pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void MISO_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTB_set_pin_pull_mode(6, pull_mode);
}

/**
 * \brief Set MISO data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void MISO_set_dir(const enum port_dir dir)
{
	PORTB_set_pin_dir(6, dir);
}

/**
 * \brief Set MISO level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void MISO_set_level(const bool level)
{
	PORTB_set_pin_level(6, level);
}

/**
 * \brief Toggle output level on MISO
 *
 * Toggle the pin level
 */
static inline void MISO_toggle_level(void)
{
	PORTB_toggle_pin_level(6);
}

/**
 * \brief Get level on MISO
 *
 * Reads the level on a pin
 */
static inline bool MISO_get_level(void)
{
	return PORTB_get_pin_level(6);
}

/**
 * \brief Set SCK pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void SCK_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTB_set_pin_pull_mode(7, pull_mode);
}

/**
 * \brief Set SCK data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void SCK_set_dir(const enum port_dir dir)
{
	PORTB_set_pin_dir(7, dir);
}

/**
 * \brief Set SCK level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void SCK_set_level(const bool level)
{
	PORTB_set_pin_level(7, level);
}

/**
 * \brief Toggle output level on SCK
 *
 * Toggle the pin level
 */
static inline void SCK_toggle_level(void)
{
	PORTB_toggle_pin_level(7);
}

/**
 * \brief Get level on SCK
 *
 * Reads the level on a pin
 */
static inline bool SCK_get_level(void)
{
	return PORTB_get_pin_level(7);
}

/**
 * \brief Set USART_RX pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void USART_RX_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTD_set_pin_pull_mode(0, pull_mode);
}

/**
 * \brief Set USART_RX data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void USART_RX_set_dir(const enum port_dir dir)
{
	PORTD_set_pin_dir(0, dir);
}

/**
 * \brief Set USART_RX level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void USART_RX_set_level(const bool level)
{
	PORTD_set_pin_level(0, level);
}

/**
 * \brief Toggle output level on USART_RX
 *
 * Toggle the pin level
 */
static inline void USART_RX_toggle_level(void)
{
	PORTD_toggle_pin_level(0);
}

/**
 * \brief Get level on USART_RX
 *
 * Reads the level on a pin
 */
static inline bool USART_RX_get_level(void)
{
	return PORTD_get_pin_level(0);
}

/**
 * \brief Set USART_TX pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void USART_TX_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTD_set_pin_pull_mode(1, pull_mode);
}

/**
 * \brief Set USART_TX data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void USART_TX_set_dir(const enum port_dir dir)
{
	PORTD_set_pin_dir(1, dir);
}

/**
 * \brief Set USART_TX level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void USART_TX_set_level(const bool level)
{
	PORTD_set_pin_level(1, level);
}

/**
 * \brief Toggle output level on USART_TX
 *
 * Toggle the pin level
 */
static inline void USART_TX_toggle_level(void)
{
	PORTD_toggle_pin_level(1);
}

/**
 * \brief Get level on USART_TX
 *
 * Reads the level on a pin
 */
static inline bool USART_TX_get_level(void)
{
	return PORTD_get_pin_level(1);
}

#endif /* ATMEL_START_PINS_H_INCLUDED */
