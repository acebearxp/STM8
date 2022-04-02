#include <stm8s.h>
#include "risym.h"
#include <stdio.h>
#include "ext_unique_id.h"
#include "ext_uart.h"

#define BUF_MAX_SIZE    (64)

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
    g_data.u8TIM3Overflow = 1;
}

static void IrDecoder_setup(void)
{
    g_data.hIrDecoder = ext_ir_decoder_init();
}

void main()
{
    uint16_t count = 0;
    char buf[BUF_MAX_SIZE];
    char id[20];

    CLOCK_setup();
    IrDecoder_setup();
    GPIO_setup();
    TIM3_setup();
    ext_uart_init(115200);
    
    enableInterrupts();

    ext_unique_id_readstr(id);
    ext_uart_log(buf, BUF_MAX_SIZE, "STM8S105K4 %s start...", id);

    while(TRUE){
        uint8_t u8Code;
        uint16_t u16Addr;

        if(ext_ir_decoder_decode(g_data.hIrDecoder, &u8Code, &u16Addr)){
            char buf[64];
            const uint8_t lenX12 = 6;
            size_t len;

            GPIO_WriteReverse(LED_GPIO_PORT, LED_GPIO_PIN);
            buf[0] = 0x12;
            buf[1] = lenX12;
            buf[2] = (uint8_t)(u16Addr>>8);
            buf[3] = (uint8_t)u16Addr;
            buf[4] = u8Code;
            buf[5] = 0x12;
            len = sprintf(buf+lenX12, "[%u]STM8 IrRC %d %d", count, u16Addr, (uint16_t)u8Code);
            ext_uart_send(buf, lenX12+len+1);
        }

        while(UART2_GetFlagStatus(UART2_FLAG_TC) == RESET);
        
        GPIO_WriteHigh(LED_GPIO_PORT, LED_GPIO_PIN);
        count++;
        wfi();
    }
}