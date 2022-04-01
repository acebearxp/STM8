#include "uart_log.h"
#include "string.h"

void uart_log(char *szbuf)
{
    size_t size = strlen(szbuf)+1;
    uart_send(szbuf, size);
}

void uart_send(uint8_t *buf, size_t size)
{
    uint8_t u8Sent = 0;
    uint16_t u16Wait = 0;
    
    while(u8Sent < size){
        if(UART2_GetFlagStatus(UART2_FLAG_TXE) == SET){
            UART2_SendData8(buf[u8Sent]);
            u8Sent++;
            u16Wait = 0;
        }
        else{
            if(u16Wait > 10000) break;
            u16Wait++;
        }
    }
}