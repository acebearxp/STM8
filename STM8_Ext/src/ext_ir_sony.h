#ifndef __EXT_IR_SONY_H
#define __EXT_IR_SONY_H

#include "ext_ir_decoder.h"
#include "ext_ir_defs.h"

bool decode_sony(uint8_t *pu8Code, uint16_t *pu16AddrCode, ex_ir_decoder_data_t *pdata);

#endif // __EXT_IR_SONY_H