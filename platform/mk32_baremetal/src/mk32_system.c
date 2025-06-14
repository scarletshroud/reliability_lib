#include "mk32_system.h"

#include "mk32_baremetal/HAL/peripherals/Include/mik32_hal_timer32.h"

#include "gpio.h"
#include "riscv_csr_encoding.h"
#include "scr1_csr_encoding.h"
#include "mcu32_memory_map.h"
#include "power_manager.h"
#include "pad_config.h"
#include "gpio_irq.h"
#include "epic.h"
#include "csr.h"
#include "uart_lib.h"
#include "xprintf.h"

static TIMER32_HandleTypeDef htimer32_1;
static TIMER32_CHANNEL_HandleTypeDef htimer32_channel0;

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

static void Timer32_1_Init(TIMER32_HandleTypeDef* htimer32_1, TIMER32_CHANNEL_HandleTypeDef* htimer32_channel0) {
    htimer32_1->Instance = TIMER32_1;
    htimer32_1->Top = 0xFFFFFFFF;
    htimer32_1->State = TIMER32_STATE_DISABLE;
    htimer32_1->Clock.Source = TIMER32_SOURCE_PRESCALER;
    htimer32_1->Clock.Prescaler = 0;
    htimer32_1->InterruptMask = 0;
    htimer32_1->CountMode = TIMER32_COUNTMODE_FORWARD;
    HAL_Timer32_Init(&htimer32_1);

    htimer32_channel0->TimerInstance = htimer32_1->Instance;
    htimer32_channel0->ChannelIndex = TIMER32_CHANNEL_0;
    htimer32_channel0->PWM_Invert = TIMER32_CHANNEL_NON_INVERTED_PWM;
    htimer32_channel0->Mode = TIMER32_CHANNEL_MODE_CAPTURE;
    htimer32_channel0->CaptureEdge = TIMER32_CHANNEL_CAPTUREEDGE_RISING;
    htimer32_channel0->OCR = htimer32_1->Top / 2;
    htimer32_channel0->Noise = TIMER32_CHANNEL_FILTER_OFF;
    HAL_Timer32_Channel_Init(&htimer32_channel0);
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

	/* Инициализация работы 32-х разрядного таймера */
	Timer32_1_Init(&htimer32_1, &htimer32_channel0);
	
	HAL_Timer32_Channel_Enable(&htimer32_channel0);
    HAL_Timer32_Value_Clear(&htimer32_1);
    HAL_Timer32_Start(&htimer32_1);
}

uint32_t mk32_get_time_us() {
	return HAL_TIMER32_VALUE_GET(&htimer32_1);
}

/* WDT */

/*

#include "mik32_hal_wdt.h"

WDT_HandleTypeDef hwdt;

void WDT_Init(uint32_t timeout)
{
    hwdt.Instance = WDT;

    hwdt.Init.Clock = HAL_WDT_OSC32K;
    hwdt.Init.ReloadMs = 1000;
    if (HAL_WDT_Init(&hwdt, timeout) != HAL_OK)
    {
        xprintf("ERROR: HAL_WDT_Init\n");
    }
}

*/