//
// Created by marcin on 16.07.2021.
//

#include "system.h"
#include <avr/io.h>
#include <stdio.h>

typedef struct SPI_0_descriptor_s {
    spi_transfer_status_t status;
    uint8_t *data;
    uint8_t size;
    spi_transfer_type_t type;
    spi_transfer_done_cb_t cb;
} SPI_0_descriptor_t;

static SPI_0_descriptor_t SPI_0_desc;

const uint8_t usart_0_tx_buffer_size = (uint8_t) USART_0_TX_BUFFER_SIZE;

int USART_0_printCHAR(char character, FILE *stream) {
    USART_0_write(character);
    return 0;
}

FILE USART_0_stream = FDEV_SETUP_STREAM(USART_0_printCHAR, NULL, _FDEV_SETUP_WRITE);

static uint8_t USART_0_rxbuf[USART_0_RX_BUFFER_SIZE];
static volatile uint8_t USART_0_rx_head;
static volatile uint8_t USART_0_rx_tail;
static volatile uint8_t USART_0_rx_elements;
static uint8_t USART_0_txbuf[USART_0_TX_BUFFER_SIZE];
static volatile uint8_t USART_0_tx_head;
static volatile uint8_t USART_0_tx_tail;
static volatile uint8_t USART_0_tx_elements;

void USART_0_default_rx_isr_cb(void);

void (*USART_0_rx_isr_cb)(void) = &USART_0_default_rx_isr_cb;

void USART_0_default_udre_isr_cb(void);

void (*USART_0_udre_isr_cb)(void) = &USART_0_default_udre_isr_cb;

void USART_0_default_rx_isr_cb(void) {
    uint8_t data;
    uint8_t tmphead;

    /* Read the received data */
    data = UDR0;
    /* Calculate buffer index */
    tmphead = (USART_0_rx_head + 1) & USART_0_RX_BUFFER_MASK;

    if (tmphead == USART_0_rx_tail) {
        /* ERROR! Receive buffer overflow */
    } else {
        /* Store new index */
        USART_0_rx_head = tmphead;

        /* Store received data in buffer */
        USART_0_rxbuf[tmphead] = data;
        USART_0_rx_elements++;
    }
}

void USART_0_default_udre_isr_cb(void) {
    uint8_t tmptail;

    /* Check if all data is transmitted */
    if (USART_0_tx_elements != 0) {
        /* Calculate buffer index */
        tmptail = (USART_0_tx_tail + 1) & USART_0_TX_BUFFER_MASK;
        /* Store new index */
        USART_0_tx_tail = tmptail;
        /* Start transmission */
        UDR0 = USART_0_txbuf[tmptail];
        USART_0_tx_elements--;
    }

    if (USART_0_tx_elements == 0) {
        /* Disable UDRE interrupt */
        UCSR0B &= ~(1 << UDRIE0);
    }
}

/**
 * \brief Set call back function for USART_0
 *
 * \param[in] cb The call back function to set
 *
 * \param[in] type The type of ISR to be set
 *
 * \return Nothing
 */
void USART_0_set_ISR_cb(usart_cb_t cb, usart_cb_type_t type) {
    switch (type) {
        case RX_CB:
            USART_0_rx_isr_cb = cb;
            break;
        case UDRE_CB:
            USART_0_udre_isr_cb = cb;
            break;
        default:
            // do nothing
            break;
    }
}


/* Interrupt service routine for RX complete */
ISR(USART0_RX_vect) {
    if (USART_0_rx_isr_cb != NULL)
        (*USART_0_rx_isr_cb)();
}

/* Interrupt service routine for Data Register Empty */
ISR(USART0_UDRE_vect)
{
    if (USART_0_udre_isr_cb != NULL)
        (*USART_0_udre_isr_cb)();
}

bool USART_0_is_tx_ready()
{
    return (USART_0_tx_elements != usart_0_tx_buffer_size);
}

bool USART_0_is_rx_ready()
{
    return (USART_0_rx_elements != 0);
}

bool USART_0_is_tx_busy()
{
    return (!(UCSR0A & (1 << TXC0)));
}

/**
 * \brief Read one character from USART_0
 *
 * Function will block if a character is not available.
 *
 * \return Data read from the USART_0 module
 */
uint8_t USART_0_read(void)
{
    uint8_t tmptail;

    /* Wait for incoming data */
    while (USART_0_rx_elements == 0)
        ;
    /* Calculate buffer index */
    tmptail = (USART_0_rx_tail + 1) & USART_0_RX_BUFFER_MASK;
    /* Store new index */
    USART_0_rx_tail = tmptail;
    ENTER_CRITICAL();
    USART_0_rx_elements--;
    EXIT_CRITICAL();

    /* Return data */
    return USART_0_rxbuf[tmptail];
}

/**
 * \brief Write one character to USART_0
 *
 * Function will block until a character can be accepted.
 *
 * \param[in] data The character to write to the USART
 *
 * \return Nothing
 */
void USART_0_write(const uint8_t data)
{
    uint8_t tmphead;

    /* Calculate buffer index */
    tmphead = (USART_0_tx_head + 1) & USART_0_TX_BUFFER_MASK;
    /* Wait for free space in buffer */
    while (USART_0_tx_elements == usart_0_tx_buffer_size)
        ;
    /* Store data in buffer */
    USART_0_txbuf[tmphead] = data;
    /* Store new index */
    USART_0_tx_head = tmphead;
    ENTER_CRITICAL();
    USART_0_tx_elements++;
    EXIT_CRITICAL();
    /* Enable UDRE interrupt */
    UCSR0B |= (1 << UDRIE0);
}

/**
 * \brief Initialize USART interface
 * If module is configured to disabled state, the clock to the USART is disabled
 * if this is supported by the device's clock system.
 *
 * \return Initialization status.
 * \retval 0 the USART init was successful
 * \retval 1 the USART init was not successful
 */
int8_t USART_0_init()
{

    // Module is in UART mode

    /* Enable USART0 */
    PRR0 &= ~(1 << PRUSART0);
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    UCSR0A = USE_2X << U2X0 /*  */
             | 0 << MPCM0;  /* Multi-processor Communication Mode: disabled */

    UCSR0B = 1 << RXCIE0    /* RX Complete Interrupt Enable: enabled */
             | 0 << UDRIE0  /* USART Data Register Empty Interupt Enable: disabled */
             | 1 << RXEN0   /* Receiver Enable: enabled */
             | 1 << TXEN0   /* Transmitter Enable: enabled */
             | 0 << UCSZ02; /*  */

    // UCSR0C = (0 << UMSEL01) | (0 << UMSEL00) /*  */
    //		 | (0 << UPM01) | (0 << UPM00) /* Disabled */
    //		 | 0 << USBS0 /* USART Stop Bit Select: disabled */
    //		 | (1 << UCSZ01) | (1 << UCSZ00); /* 8-bit */

    uint8_t x;

    /* Initialize ringbuffers */
    x = 0;

    USART_0_rx_tail     = x;
    USART_0_rx_head     = x;
    USART_0_rx_elements = x;
    USART_0_tx_tail     = x;
    USART_0_tx_head     = x;
    USART_0_tx_elements = x;

#if defined(__GNUC__)
    stdout = &USART_0_stream;
#endif

    return 0;
}

/**
 * \brief Enable RX and TX in USART_0
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the RX and TX enable-bits in the USART control register
 *
 * \return Nothing
 */
void USART_0_enable()
{
    UCSR0B |= ((1 << TXEN0) | (1 << RXEN0));
}

/**
 * \brief Enable RX in USART_0
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the RX enable-bit in the USART control register
 *
 * \return Nothing
 */
void USART_0_enable_rx()
{
    UCSR0B |= (1 << RXEN0);
}

/**
 * \brief Enable TX in USART_0
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the TX enable-bit in the USART control register
 *
 * \return Nothing
 */
void USART_0_enable_tx()
{
    UCSR0B |= (1 << TXEN0);
}

/**
 * \brief Disable USART_0
 * 1. Disables the USART module by clearing the enable-bit(s) in the USART control register
 * 2. If supported by the clock system, disables the clock to the USART
 *
 * \return Nothing
 */
void USART_0_disable()
{
    UCSR0B &= ~((1 << TXEN0) | (1 << RXEN0));
}

/**
 * \brief Get recieved data from USART_0
 *
 * \return Data register from USART_0 module
 */
uint8_t USART_0_get_data()
{
    return UDR0;
}

int8_t EXTERNAL_IRQ_0_init(void) {

    // Set pin direction to input
    TX2RTS_set_dir(PORT_DIR_IN);

    TX2RTS_set_pull_mode(
            // <y> Pull configuration
            // <id> pad_pull_config
            // <PORT_PULL_OFF"> Off
            // <PORT_PULL_UP"> Pull-up
            PORT_PULL_OFF);

    // Set pin direction to input
    TX1RTS_set_dir(PORT_DIR_IN);

    TX1RTS_set_pull_mode(
            // <y> Pull configuration
            // <id> pad_pull_config
            // <PORT_PULL_OFF"> Off
            // <PORT_PULL_UP"> Pull-up
            PORT_PULL_OFF);

    // Set pin direction to input
    TX0RTS_set_dir(PORT_DIR_IN);

    TX0RTS_set_pull_mode(
            // <y> Pull configuration
            // <id> pad_pull_config
            // <PORT_PULL_OFF"> Off
            // <PORT_PULL_UP"> Pull-up
            PORT_PULL_OFF);

    // Set pin direction to input
    RX1BF_set_dir(PORT_DIR_IN);

    RX1BF_set_pull_mode(
            // <y> Pull configuration
            // <id> pad_pull_config
            // <PORT_PULL_OFF"> Off
            // <PORT_PULL_UP"> Pull-up
            PORT_PULL_OFF);

    // Set pin direction to input
    RX0BF_set_dir(PORT_DIR_IN);

    RX0BF_set_pull_mode(
            // <y> Pull configuration
            // <id> pad_pull_config
            // <PORT_PULL_OFF"> Off
            // <PORT_PULL_UP"> Pull-up
            PORT_PULL_OFF);

    // Set pin direction to input
    INT_set_dir(PORT_DIR_IN);

    INT_set_pull_mode(
            // <y> Pull configuration
            // <id> pad_pull_config
            // <PORT_PULL_OFF"> Off
            // <PORT_PULL_UP"> Pull-up
            PORT_PULL_OFF);

    // Set pin direction to input
    RESET_set_dir(PORT_DIR_IN);

    RESET_set_pull_mode(
            // <y> Pull configuration
            // <id> pad_pull_config
            // <PORT_PULL_OFF"> Off
            // <PORT_PULL_UP"> Pull-up
            PORT_PULL_OFF);

    PCICR = (1 << PCIE0) | (1 << PCIE1); // Enable pin change interrupts 0, 1
    PCMSK1 = (1 << PCINT8) | (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11); // Pin change enable 8, 9, 10, 11
    PCMSK0 = (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2);
    return 0;
}

int8_t SPI_init() {
    PORTB_set_pin_dir(6, PORT_DIR_IN);
    PORTB_set_pin_pull_mode(6, PORT_PULL_OFF);
    PORTB_set_pin_dir(5, PORT_DIR_OUT);
    PORTB_set_pin_level(5, false);
    PORTB_set_pin_dir(7, PORT_DIR_OUT);
    PORTB_set_pin_level(7, false);
    PRR0 &= ~(1 << PRSPI);
    SPCR = (1 << SPE) | (0 << DORD) | (1 << MSTR) | (0 << CPOL) | (0 << CPHA) | (1 << SPIE) | (0 << SPR1) | (0 << SPR0);
    SPI_0_desc.status = SPI_FREE;
    SPI_0_desc.cb = NULL;
    return 0;
}

int8_t USART_init() {
    PORTD_set_pin_dir(0, PORT_DIR_IN);
    PORTD_set_pin_pull_mode(0, PORT_PULL_OFF);
    PORTD_set_pin_dir(1, PORT_DIR_OUT);
    PORTD_set_pin_level(1, false);
    USART_0_init();
    return 0;
}
