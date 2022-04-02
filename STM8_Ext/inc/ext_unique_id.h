#ifndef __EXT_UNIQUE_ID_H
#define __EXT_UNIQUE_ID_H

#include <stm8s.h>

// must provide 12 bytes buffer
void ext_unique_id_read12(uint8_t *buf);
// must provide 20 bytes buffer
void ext_unique_id_readstr(char *buf);

#endif __EXT_UNIQUE_ID_H