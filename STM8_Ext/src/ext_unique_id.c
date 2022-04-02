#include "ext_unique_id.h"
#include <stdio.h>

#define UNIQUE_ID_PTR       (0x48cd)
#define UNIQUE_ID_SIZE      (12)
#define UNIQUE_ID_NO_POS    (5)
// must provide 12 bytes buffer
void ext_unique_id_read12(uint8_t *buf)
{
    uint8_t *ptr = (uint8_t*)UNIQUE_ID_PTR;
    uint8_t i;
    for(i=0;i<UNIQUE_ID_SIZE;i++){
        buf[i] = ptr[i];
    }
}

// must provide 20 bytes buffer
void ext_unique_id_readstr(char *buf)
{
    uint8_t *ptr = (uint8_t*)UNIQUE_ID_PTR;
    uint8_t i, pos=0;
    for(i=UNIQUE_ID_NO_POS;i<UNIQUE_ID_SIZE;i++){
        buf[pos] = ptr[i];
        pos++;
    }

    sprintf(buf+pos, "-%02X%04X%04X", (uint16_t)ptr[4], (uint16_t)ptr[0]<<8|ptr[1], (uint16_t)ptr[2]<<8|ptr[3]);
}