#include <stm8s.h>
#include "stm8s_it.h"
#include "risym.h"

// IRQ5 for EXTI2(EXTI_PORT_GPIOC)
void EXTI2_IRQHandler(void)
{
    uint16_t u16Value = TIM3_GetCounter();
    if(u16Value > 12000){
        TIM3_SetCounter(0);
    }
    else{
        ext_ir_decoder_append(g_data.hIrDecoder, u16Value);
    }
}