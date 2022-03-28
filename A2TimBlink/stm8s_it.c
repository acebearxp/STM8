#include <stm8s.h>
#include "stm8s_it.h"
#include "risym.h"

void TIM2_UPD_IRQHandler(void)
{
    GPIO_WriteReverse(LED_GPIO_PORT, LED_GPIO_PIN);
    TIM2_ClearFlag(TIM2_FLAG_UPDATE);
}