#ifndef __STM8S_IT_H
#define __STM8S_IT_H

#include <stm8s.h>

#ifdef __VSCODE__
void TIM2_UPD_IRQHandler(void);
#else
@far @interrupt void TIM2_UPD_IRQHandler(void);
#endif

#endif