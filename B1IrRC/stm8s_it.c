#include <stm8s.h>
#include "stm8s_it.h"
#include "risym.h"

// IRQ5 for EXTI2(EXTI_PORT_GPIOC)
void EXTI2_IRQHandler(void)
{
    GPIO_WriteReverse(LED_GPIO_PORT, LED_GPIO_PIN);
}