#include "ext_ir_decoder.h"
#include <stdlib.h>
#include "ext_ir_defs.h"
#include "ext_ir_nec.h"
#include "ext_ir_sony.h"

ext_ir_decoder_handler_t ext_ir_decoder_init(void)
{
    ex_ir_decoder_data_t *pdata = malloc(sizeof(ex_ir_decoder_data_t));
    return pdata;
}

void ext_ir_decoder_deinit(ext_ir_decoder_handler_t hIrDecoder)
{
    ex_ir_decoder_data_t *pdata = hIrDecoder;
    free(pdata);
}

uint8_t ext_ir_decoder_get_len(ext_ir_decoder_handler_t hIrDecoder)
{
    ex_ir_decoder_data_t *pdata = hIrDecoder;
    return pdata->u8Len;
}

uint8_t ext_ir_decoder_append(ext_ir_decoder_handler_t hIrDecoder, uint16_t u16Capture)
{
    ex_ir_decoder_data_t *pdata = hIrDecoder;

    if(pdata->u8Len < _EXT_IR_MAX_CAPTURE){
        if(u16Capture != 0 || pdata->u8Len != 0){
            pdata->u16Capture[pdata->u8Len] = u16Capture;
            pdata->u8Len++;
        }
    }
}

void ext_ir_decoder_clear(ext_ir_decoder_handler_t hIrDecoder)
{
    ex_ir_decoder_data_t *pdata = hIrDecoder;
    pdata->u8Len = 0;
}

bool ext_ir_decoder_decode(ext_ir_decoder_handler_t hIrDecoder, uint8_t *pu8Code, uint16_t *pu16Addr)
{
    bool bOK = FALSE;
    ex_ir_decoder_data_t *pdata = hIrDecoder;

    if(pdata->u8Len > 0){
        if(pdata->u16Capture[0] > 5000){
            // NEC
            bOK = decode_nec(pu8Code, pu16Addr, pdata);
        }
        else if(pdata->u16Capture[0] > 2000){
            // SONY
            bOK = decode_sony(pu8Code, pu16Addr, pdata);
        }
        else{
            pdata->u8Len = 0;
        }
    }
    return bOK;
}