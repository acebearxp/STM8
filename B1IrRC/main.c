#include <stm8s.h>
#include "risym.h"
#include <stdio.h>

TOPMOST_DATA_T g_data;

static void CLOCK_setup(void)
{
    CLK_HSECmd(ENABLE); // HSE@8.000MHz
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV4); // 2MHz
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
    TIM3_TimeBaseInit(TIM3_PRESCALER_8, 0xFFFF); // 1MHz
    TIM3_Cmd(ENABLE);
}

static void UART2_setup(void)
{
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART2, ENABLE);
    UART2_Init(9600, UART2_WORDLENGTH_8D, UART2_STOPBITS_1,
        UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
    UART2_Cmd(ENABLE);
}

static void IrDecoder_setup(void)
{
    g_data.hIrDecoder = ext_ir_decoder_init();
}

void main()
{
    CLOCK_setup();
    IrDecoder_setup();
    GPIO_setup();
    TIM3_setup();
    UART2_setup();
    
    enableInterrupts();

    while(TRUE){
        char buf[256];
        int nLen, nSent, nWait;
        uint8_t u8Code = ext_ir_decoder_decode(g_data.hIrDecoder);
        if(u8Code > 0){
            GPIO_WriteReverse(LED_GPIO_PORT, LED_GPIO_PIN);
            nLen = sprintf(buf, "STM8 IrRC : %u", u8Code);
            nSent = 0, nWait = 0;
            while(nSent < nLen+1){
                if(UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET){
                    UART2_SendData8(buf[nSent]);
                    nSent++;
                    nWait = 0;
                }
                else{
                    if(nWait > 10000) break;
                    nWait++;
                }
            }
        }
    }
}