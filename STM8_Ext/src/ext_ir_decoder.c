#include "ext_ir_decoder.h"
#include <stdlib.h>

#define _EXT_IR_MAX_CAPTURE     192

typedef struct _ext_ir_decoder_data
{
    uint8_t u8Len;
    uint16_t u16Capture[_EXT_IR_MAX_CAPTURE];
} ex_ir_decoder_data_t;

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

uint8_t ext_ir_decoder_append(ext_ir_decoder_handler_t hIrDecoder, uint16_t u16Capture)
{
    ex_ir_decoder_data_t *pdata = hIrDecoder;
    if(pdata->u8Len < _EXT_IR_MAX_CAPTURE){
        pdata->u16Capture[pdata->u8Len] = u16Capture;
        pdata->u8Len++;
    }
}

void ext_ir_decoder_clear(ext_ir_decoder_handler_t hIrDecoder)
{
    ex_ir_decoder_data_t *pdata = hIrDecoder;
    pdata->u8Len = 0;
}