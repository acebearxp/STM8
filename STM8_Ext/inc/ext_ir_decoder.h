/**
 * 
 */
#ifndef __EXT_IR_DECODER_H
#define __EXT_IR_DECODER_H

#include <stm8s.h>

typedef void* ext_ir_decoder_handler_t;

ext_ir_decoder_handler_t ext_ir_decoder_init(void);
void ext_ir_decoder_deinit(ext_ir_decoder_handler_t hIrDecoder);

#ifndef __INTELLISENSE__
@inline
#endif
uint8_t ext_ir_decoder_get_len(ext_ir_decoder_handler_t hIrDecoder);

uint8_t ext_ir_decoder_append(ext_ir_decoder_handler_t hIrDecoder, uint16_t u16Capture);

void ext_ir_decoder_clear(ext_ir_decoder_handler_t hIrDecoder);

bool ext_ir_decoder_decode(ext_ir_decoder_handler_t hIrDecoder, uint8_t *pu8Code, uint16_t *pu16Addr);

#endif // __EXT_IR_DECODER_H