#ifndef __RISYM_H
#define __RISYM_H

// STM8S105K4T6  HSE@8.000M risym dev-board
#define LED_GPIO_PORT   (GPIOE)
#define LED_GPIO_PIN    (GPIO_PIN_5)

#define IR_GPIO_PORT    (GPIOC)
#define IR_GPIO_PIN     (GPIO_PIN_2)
#define IR_GPIO_EXTI    (EXTI_PORT_GPIOC)

#ifdef __VSCODE__
    #define _asm ___asm
#endif

#endif