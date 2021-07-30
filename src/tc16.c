//
// Created by marcin on 29.07.2021.
//

#include <tc16.h>
#include <utils.h>

/**
 * \brief Initialize TIMER_0 interface
 *
 * \return Initialization status.
 */
int8_t TIMER_0_init()
{

    /* Enable TC1 */
    PRR0 &= ~(1 << PRTIM1);

    TCCR1A = (1 << COM1A1) | (0 << COM1A0)   /* Clear OCA on Compare Match */
            | (0 << COM1B1) | (0 << COM1B0) /* Normal port operation, OCB disconnected */
            | (0 << WGM11) | (0 << WGM10);  /* TC16 Mode 12 CTC */

            TCCR1B = (1 << WGM13) | (1 << WGM12)                /* TC16 Mode 12 CTC */
                    | 0 << ICNC1                               /* Input Capture Noise Canceler: disabled */
                    | 0 << ICES1                               /* Input Capture Edge Select: disabled */
                    | (0 << CS12) | (1 << CS11) | (0 << CS10); /* IO clock divided by 8 */

                    // ICR1 = 0x0; /* Top counter value: 0x0 */

                    OCR1A = 0x733; /* Output compare A: 0x733 */

                    // OCR1B = 0x0; /* Output compare B: 0x0 */

                    // GTCCR = 0 << TSM /* Timer/Counter Synchronization Mode: disabled */
                    //		 | 0 << PSRASY /* Prescaler Reset Timer/Counter2: disabled */
                    //		 | 0 << PSRSYNC; /* Prescaler Reset: disabled */

                    TIMSK1 = 0 << OCIE1B   /* Output Compare B Match Interrupt Enable: disabled */
                            | 1 << OCIE1A /* Output Compare A Match Interrupt Enable: enabled */
                            | 0 << ICIE1  /* Input Capture Interrupt Enable: disabled */
                            | 0 << TOIE1; /* Overflow Interrupt Enable: disabled */

                            return 0;
}
