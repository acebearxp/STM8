#ifndef __UART_LOG_H
#define __UART_LOG_H

#include <stdio.h>
#include <stm8s.h>

void uart_send(uint8_t *buf, size_t size);
void uart_log(char *szbuf);

#endif // __UART_LOG_H