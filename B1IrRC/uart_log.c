#include "uart_log.h"
#include "string.h"

void uart_log(char *szbuf)
{
    uint8_t u8Sent = 0;
    uint16_t u16Wait = 0, size = strlen(szbuf)+1;
    
    while(u8Sent < size){
        if(UART2_GetFlagStatus(UART2_FLAG_TXE) == SET){
            UART2_SendData8(szbuf[u8Sent]);
            u8Sent++;
            u16Wait = 0;
        }
        else{
            if(u16Wait > 10000) break;
            u16Wait++;
        }
    }
}