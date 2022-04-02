#include "ext_uart.h"
#include <stdarg.h>

void ext_uart_init(uint32_t u32baudrate)
{
    // the baudrate shouldn't greater than 1/16 of master clock
    uint32_t u32Freq = CLK_GetClockFreq();
    assert_param(u32baudrate < u32Freq / 16);

    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART2, ENABLE);

    UART2_DeInit();
    UART2_Init(u32baudrate, UART2_WORDLENGTH_8D, UART2_STOPBITS_1,
        UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TX_ENABLE);
    
    UART2_Cmd(ENABLE);
}

void ext_uart_deinit()
{
    UART2_Cmd(DISABLE);
    UART2_DeInit();
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART2, DISABLE);
}

void ext_uart_send(uint8_t *buf, size_t size)
{
    size_t sent = 0;
    while(sent < size){
        if(UART2_GetFlagStatus(UART2_FLAG_TXE) == SET){
            UART2_SendData8(buf[sent]);
            sent++;
        }
    }
}

size_t ext_uart_log(char *szbuf, size_t size, const char *fmt, ...)
{
    uint32_t len;
    va_list pArgs;

    va_start(pArgs, fmt);
    len = vsprintf(szbuf, fmt, pArgs);
    va_end(pArgs);
    assert_param(len < size);

    ext_uart_send(szbuf, len+1);
    return len;
}