#ifndef __RISYM_H
#define __RISYM_H

#include "ext_ir_decoder.h"

// STM8S105K4T6  HSE@8.000M risym dev-board
#define LED_GPIO_PORT   (GPIOE)
#define LED_GPIO_PIN    (GPIO_PIN_5)

#define IR_GPIO_PORT    (GPIOC)
#define IR_GPIO_PIN     (GPIO_PIN_2)
#define IR_GPIO_EXTI    (EXTI_PORT_GPIOC)

#define UART_GPIO_PORT      (GPIOD)
#define UART_GPIO_TX_PIN    (GPIO_PIN_5)
#define UART_GPIO_RX_PIN    (GPIO_PIN_6)

#ifdef __INTELLISENSE__
    #define _asm ___asm
#endif

typedef struct _TOPMOST_DATA
{
    ext_ir_decoder_handler_t hIrDecoder;
} TOPMOST_DATA_T;

extern TOPMOST_DATA_T g_data;

#endif