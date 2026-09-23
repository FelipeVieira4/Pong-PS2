#ifndef PAD_CONTROLLER_H
#define PAD_CONTROLLER_H

#include <libpad.h>

typedef struct padButtonStatus padButtonStatus;

bool load_module_pad(void);

bool validade_pad();
u32 normalize_pad_data(padButtonStatus *buttons);


#endif