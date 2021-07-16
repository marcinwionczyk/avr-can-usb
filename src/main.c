#include "system.h"

int main(void) {
    // Start up the UART and attach it to STDIN/STDOUT.
    mcu_init();
    SS_set_dir(PORT_DIR_OUT);
    SS_set_level(true);
    sysctrl_init();
    EXTERNAL_IRQ_0_init();
    SPI_init();
    USART_init();

    return 0;
}
