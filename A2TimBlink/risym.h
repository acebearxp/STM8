#ifndef __RISYM_H
#define __RISYM_H

// STM8S105K4T6  HSE@8.000M risym dev-board
#define LED_GPIO_PORT    (GPIOE)
#define LED_GPIO_PIN     (GPIO_PIN_5)

#ifdef __INTELLISENSE__
    #define _asm ___asm
#endif

#endif