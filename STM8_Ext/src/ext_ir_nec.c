#include "ext_ir_nec.h"
/* NEC IR protocol
NEC-CODE S[0] A[0:7] ~A[0:7] C[0:7] ~C[0:7] E[0]
NEC-REPEAT ????
+ HighLevel / - LowLevel / each segment about 600us
S-Start -16+8
bit0 -1-1
bit1 -1+3
*/
#include <stdio.h>
#include "uart_log.h"

static bool decode(uint16_t u16d0, uint16_t u16d1)
{
    const uint16_t u16s = 563; // segment
    const uint16_t u16t = 100; // tolerance
    const uint16_t uLmin = u16s*3-u16t, uLmax = u16s*3+u16t;

    return (u16d1 > uLmin && u16d1 < uLmax);
}

static uint8_t decode_range(uint16_t *pu16buf, uint8_t u8s, uint8_t u8len)
{
    int8_t i;
    uint8_t u8value = 0;
    uint16_t u16d0, u16d1;
    bool b1;
    for(i=u8len/2-1;i>=0;i--){
        u16d0 = pu16buf[u8s+2*i+0];
        u16d1 = pu16buf[u8s+2*i+1];
        b1 = decode(u16d0, u16d1);
        u8value = (u8value << 1) | (b1 & 0x01);
    }
    return u8value;
}

bool decode_nec(uint8_t *pu8Code, uint16_t *pu16AddrCode, ex_ir_decoder_data_t *pdata)
{
    // normal 16x:8x|8addr~8addr|8code~8code|1x:0x
    if(pdata->u8Len >= 67){
        uint8_t u8Code[4];
        u8Code[0] = decode_range(pdata->u16Capture, 2, 16);
        u8Code[1] = decode_range(pdata->u16Capture, 18, 16);
        u8Code[2] = decode_range(pdata->u16Capture, 34, 16);
        u8Code[3] = decode_range(pdata->u16Capture, 50, 16);
        if((u8Code[0]&u8Code[1])==0){
            *pu16AddrCode = u8Code[0];
        }
        else{
            *pu16AddrCode = (u8Code[1] << 8) | u8Code[0];
        }
        
        *pu8Code = u8Code[2];
        pdata->u8Len = 0;
        return ((u8Code[2] ^ u8Code[3]) == 0xff);
    }
    // repeated 16x:4x|1x:0x
    else if(pdata->u8Len >= 4 && pdata->u16Capture[3] == 0){
        *pu8Code = 0xff;
        *pu16AddrCode = 0xffff;
        pdata->u8Len = 0;
        return TRUE;
    }
    else{
        *pu8Code = pdata->u8Len;
        *pu16AddrCode = pdata->u8Len;
        if(pdata->u8Len >= 67){
            pdata->u8Len = 0;
        }
    }
    return FALSE;
}