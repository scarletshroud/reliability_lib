#include "mk32_system.h"

#include <gpio.h>
#include "riscv_csr_encoding.h"
#include "scr1_csr_encoding.h"
#include "mcu32_memory_map.h"
#include <power_manager.h>
#include "pad_config.h"
#include <gpio_irq.h>
#include <epic.h>
#include <csr.h>
#include "uart_lib.h"
#include "xprintf.h"

extern unsigned long __TEXT_START__;

void trap_handler() {
	if ( EPIC->RAW_STATUS & (1<<EPIC_GPIO_IRQ_INDEX))
	{
		if ( GPIO_IRQ->INTERRUPT & (1<<7) )
		{
			GPIO_0->OUTPUT ^= (0b1)<<(10);
			GPIO_IRQ->CLEAR = (1 << 7);
		}
		EPIC->CLEAR = (1<<EPIC_GPIO_IRQ_INDEX);
	}
}

void xputc(char c)
{
	UART_WriteByte(UART_1, c);
	UART_WaitTransmission(UART_1);
}

void mk32_system_init() {
    // interrupt vector init
	write_csr(mtvec, &__TEXT_START__);

	PM->CLK_APB_P_SET =   PM_CLOCK_APB_P_GPIO_0_M
						| PM_CLOCK_APB_P_GPIO_1_M
						| PM_CLOCK_APB_P_GPIO_2_M
						| PM_CLOCK_APB_P_GPIO_IRQ_M
						;
	PM->CLK_APB_M_SET =   PM_CLOCK_APB_M_PAD_CONFIG_M
						| PM_CLOCK_APB_M_EPIC_M
						;
	PM->CLK_AHB_SET	  =   PM_CLOCK_AHB_SPIFI_M
						;

	UART_Init(UART_1, 3333, UART_CONTROL1_TE_M | UART_CONTROL1_RE_M | UART_CONTROL1_M_8BIT_M, 0, 0);

    // LEDs P0.9 init as Output
	GPIO_0->DIRECTION_OUT =  1<<(9);
    // LEDs P0.10 init as Output
	GPIO_0->DIRECTION_OUT =  1<<(10);

	// P1.15 init as Input
	GPIO_1->DIRECTION_IN =  1<<(15);

	// interrupt generation setup on P1.15
	GPIO_IRQ->LINE_MUX = GPIO_IRQ_LINE_MUX(3, 7);	// mux=3 --> line=7
	GPIO_IRQ->EDGE = 1 << 7;						// EDGE mode
	GPIO_IRQ->LEVEL_CLEAR = 1 << 7;					// falling edge
	GPIO_IRQ->ENABLE_SET = 1 << 7;					// enable 7th line

	// EPIC interrupt reception setup
	EPIC->MASK_LEVEL_SET = 1 << EPIC_GPIO_IRQ_INDEX;

	// global interrupt enable
	set_csr(mstatus, MSTATUS_MIE);
    set_csr(mie, MIE_MEIE);
}
