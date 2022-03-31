#include <stm8s.h>
#include "risym.h"
#include <stdio.h>
#include "uart_log.h"

TOPMOST_DATA_T g_data;

static void CLOCK_setup(void)
{
    CLK_HSECmd(ENABLE); // HSE@8.000MHz
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV4); // CPU@2MHz
    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
}

static void GPIO_setup(void)
{
    // LED
    GPIO_Init(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_MODE_OUT_OD_LOW_SLOW);
    GPIO_WriteHigh(LED_GPIO_PORT, LED_GPIO_PIN);

    // IrRecv
    GPIO_Init(IR_GPIO_PORT, IR_GPIO_PIN, GPIO_MODE_IN_FL_IT);
    EXTI_SetExtIntSensitivity(IR_GPIO_EXTI, EXTI_SENSITIVITY_RISE_FALL);

    // UART
    GPIO_Init(UART_GPIO_PORT, UART_GPIO_TX_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(UART_GPIO_PORT, UART_GPIO_RX_PIN, GPIO_MODE_IN_PU_NO_IT);
}

static void TIM3_setup(void)
{
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER3, ENABLE);
    // 12000 is safe enough, for the longest interval between pulses is 9ms(9000)
    TIM3_TimeBaseInit(TIM3_PRESCALER_8, 12000); // 1MHz
    TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);
    TIM3_Cmd(ENABLE);
}

static void UART2_setup(void)
{
    // the baudrate should no more than 8MHz/16 since master clock is at 8MHz(usually 460800 max)
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART2, ENABLE);
    UART2_DeInit();
    UART2_Init(115200, UART2_WORDLENGTH_8D, UART2_STOPBITS_1,
        UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
    UART2_Cmd(ENABLE);
}

static void IrDecoder_setup(void)
{
    g_data.hIrDecoder = ext_ir_decoder_init();
}

void main()
{
    uint16_t count = 0;

    CLOCK_setup();
    IrDecoder_setup();
    GPIO_setup();
    TIM3_setup();
    UART2_setup();
    
    enableInterrupts();

    uart_log("STM8S105K4 start...");

    while(TRUE){
        uint8_t u8Code;
        uint16_t u16Addr;

        if(ext_ir_decoder_decode(g_data.hIrDecoder, &u8Code, &u16Addr)){
            char buf[64];
            GPIO_WriteReverse(LED_GPIO_PORT, LED_GPIO_PIN);
            sprintf(buf, "[%u]STM8 IrRC %d %d", count, u16Addr, (uint16_t)u8Code);
            uart_log(buf);
        }

        while(UART2_GetFlagStatus(UART2_FLAG_TC) == RESET);

        if(count % 100 == 0)
            GPIO_WriteReverse(LED_GPIO_PORT, LED_GPIO_PIN);
        
        count++;
        wfi();
    }
}