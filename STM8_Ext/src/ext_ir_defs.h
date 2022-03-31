#ifndef __EXT_IR_DEFS_H
#define __EXT_IR_DEFS_H

#include <stm8s.h>
#define _EXT_IR_MAX_CAPTURE     192

typedef struct _ext_ir_decoder_data
{
    uint8_t u8Len;
    uint16_t u16Capture[_EXT_IR_MAX_CAPTURE];
} ex_ir_decoder_data_t;

#endif // __EXT_IR_DEFS_H