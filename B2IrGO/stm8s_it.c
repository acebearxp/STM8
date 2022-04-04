#include <stm8s.h>
#include "stm8s_it.h"
#include "risym.h"
#include "ext_ir_decoder.h"

// IRQ5 for EXTI2(EXTI_PORT_GPIOC)
void EXTI2_IRQHandler(void)
{
    if(g_data.u8TIM3Overflow){
        TIM3_SetCounter(0);
        TIM3_Cmd(ENABLE);
        g_data.u8TIM3Overflow = 0;
        ext_ir_decoder_clear(g_data.hIrDecoder);
    }
    else{
        uint16_t u16Value = TIM3_GetCounter();
        TIM3_SetCounter(0);
        if(u16Value < 100) return;
        ext_ir_decoder_append(g_data.hIrDecoder, u16Value);
    }
}

// IRQ15 for TIM3 overflow
void TIM3_IRQHandler(void)
{
    if(g_data.u8TIM3Overflow == 0){
        ext_ir_decoder_append(g_data.hIrDecoder, 0);
        g_data.u8TIM3Overflow = 1;
    }
    else{
        TIM3_Cmd(DISABLE);
    }
    
    TIM3_ClearFlag(TIM3_IT_UPDATE);
}