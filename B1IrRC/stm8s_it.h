#ifndef __STM8S_IT_H
#define __STM8S_IT_H

#include <stm8s.h>

#ifdef __INTELLISENSE__
void EXTI2_IRQHandler(void);
#else
@far @interrupt void EXTI2_IRQHandler(void);
#endif

#endif