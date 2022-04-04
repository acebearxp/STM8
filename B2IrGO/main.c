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

    // LEGO
    GPIO_Init(LEGO_GPIO_PORT, LEGO_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_WriteLow(LEGO_GPIO_PORT, LEGO_GPIO_PIN);

    // IrRecv
    GPIO_Init(IR_GPIO_PORT, IR_GPIO_PIN, GPIO_MODE_IN_FL_IT);
    EXTI_SetExtIntSensitivity(IR_GPIO_EXTI, EXTI_SENSITIVITY_RISE_FALL);
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

    CLOCK_setup();
    IrDecoder_setup();
    GPIO_setup();
    TIM3_setup();
    
    enableInterrupts();

    while(TRUE){
        uint8_t u8Code;
        uint16_t u16Addr;

        if(ext_ir_decoder_decode(g_data.hIrDecoder, &u8Code, &u16Addr)){
            GPIO_WriteReverse(LED_GPIO_PORT, LED_GPIO_PIN);

            if(u8Code == 116){
                // UP
                GPIO_WriteHigh(LEGO_GPIO_PORT, LEGO_GPIO_PIN);
            }
            else if(u8Code == 117){
                // DOWN
                GPIO_WriteLow(LEGO_GPIO_PORT, LEGO_GPIO_PIN);
            }
        }

        

        //GPIO_WriteHigh(LED_GPIO_PORT, LED_GPIO_PIN);
        count++;
        wfi();
    }
}