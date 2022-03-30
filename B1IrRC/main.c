#include <stm8s.h>
#include "risym.h"

static void CLOCK_setup(void)
{
    CLK_HSECmd(ENABLE); // HSE@8.000MHz
    CLK_ClockSwitchCmd(ENABLE);
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV8); // 1MHz
    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);

    // CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE); // TIM2
}

static void GPIO_setup(void)
{
    GPIO_Init(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_MODE_OUT_OD_LOW_SLOW);
    GPIO_WriteHigh(LED_GPIO_PORT, LED_GPIO_PIN);

    GPIO_Init(IR_GPIO_PORT, IR_GPIO_PIN, GPIO_MODE_IN_FL_IT);
    EXTI_SetExtIntSensitivity(IR_GPIO_EXTI, EXTI_SENSITIVITY_RISE_FALL);
}

void main()
{
    CLOCK_setup();
    GPIO_setup();

    enableInterrupts();
}