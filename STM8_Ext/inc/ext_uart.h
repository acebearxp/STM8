#ifndef __EXT_UART_H
#define __EXT_UART_H

#include <stdio.h>
#include <stm8s.h>

void ext_uart_init(uint32_t u32baudrate);
void ext_uart_deinit();

void ext_uart_send(uint8_t *buf, size_t size);
size_t ext_uart_log(char *szbuf, size_t size, const char *fmt, ...);

#endif // __EXT_UART_H