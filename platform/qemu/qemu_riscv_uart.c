#include "qemu_riscv_uart.h"

static void uart_putc(char c) {
    while (!UART0_FF_THR_EMPTY) {
    }
    
    UART0_DR = c;                           
}
 
void qemu_riscv_uart_enable(void) {
    UART0_FCR = UARTFCR_FFENA;
}

void qemu_riscv_uart_puts(const char *str) {
    while (*str) {                        
        uart_putc(*str++);                    
    }
}