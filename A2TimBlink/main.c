/**
 *  Blink LED with internal TIM 
 */
#include <stm8s.h>
#include "risym.h"

static void CLOCK_setup(void)
{
    CLK_DeInit();

    CLK_HSECmd(ENABLE); // HSE@8.000MHz
    CLK_HSICmd(DISABLE);
    CLK_LSICmd(DISABLE);
    while(CLK_GetFlagStatus(CLK_FLAG_HSERDY) == FALSE);

    CLK_ClockSwitchCmd(ENABLE);
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV128); // 62.5kHz
    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);

    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE); // TIM2
}

static void GPIO_setup(void)
{
    GPIO_DeInit(LED_GPIO_PORT);
    GPIO_Init(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_MODE_OUT_OD_LOW_SLOW);
}

static void TIM2_setup(void)
{
    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_256, 15625); // 8MHz/256 = 31.25kHz
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
    TIM2_Cmd(ENABLE);
    enableInterrupts();
}

void main(void)
{
    CLOCK_setup();
    GPIO_setup();
    TIM2_setup();
}
