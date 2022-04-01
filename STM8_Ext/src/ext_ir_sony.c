#include "ext_ir_sony.h"
/* SONY IR protocol
SONY-12 S[0] C[0:6] D[0:4]
SONY-15 S[0] C[0:6] D[0:7]
SONY-20 S[0] C[0:6] D[0:4] E[0:7]
+ HighLevel / - LowLevel / each segment about 600us
S-Start -4+1
bit0 -1+1
bit1 -2+1
*/

static bool decode(uint16_t u16d0, uint16_t u16d1)
{
    const uint16_t u16s = 600; // segment
    const uint16_t u16t = 100; // tolerance
    const uint16_t uLmin = u16s*2-u16t, uLmax = u16s*2+u16t;

    return (u16d0 > uLmin && u16d0 < uLmax);
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

bool decode_sony(uint8_t *pu8Code, uint16_t *pu16AddrCode, ex_ir_decoder_data_t *pdata)
{
    uint8_t u8Code[3];
    // SONY-12
    if(pdata->u8Len >= 26 && pdata->u16Capture[25] == 0){
        u8Code[0] = decode_range(pdata->u16Capture, 2, 14);
        u8Code[1] = decode_range(pdata->u16Capture, 16, 10);
        
        *pu8Code = u8Code[0];
        *pu16AddrCode = u8Code[1];
        pdata->u8Len = 0;
        return TRUE;
    }
    // SONY-20
    else if(pdata->u8Len >= 42 && pdata->u16Capture[41] == 0){
        u8Code[0] = decode_range(pdata->u16Capture, 2, 14);
        u8Code[1] = decode_range(pdata->u16Capture, 16, 10);
        u8Code[2] = decode_range(pdata->u16Capture, 26, 16);
        
        *pu8Code = u8Code[0];
        *pu16AddrCode = (u8Code[2] << 8)|u8Code[1];
        pdata->u8Len = 0;
        return TRUE;
    }
    else{
        *pu8Code = pdata->u8Len;
        *pu16AddrCode = pdata->u8Len;
        if(pdata->u8Len >= 42){
            pdata->u8Len = 0;
        }
    }
    return FALSE;
}