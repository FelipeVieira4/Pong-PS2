#include <sifrpc.h>
#include <loadfile.h>
#include <stdbool.h>
#include <libpad.h>

#include "pad_controller.h"

bool load_module_pad(void){
    SifInitRpc(0);
    SifLoadModule("rom0:SIO2MAN", 0, NULL);
    SifLoadModule("rom0:PADMAN", 0, NULL);

    return true;
}

u32 normalize_pad_data(padButtonStatus *buttons){
    // Inverte os bits (No PS2, 0 = Pressionado, 1 = Solto)
    return 0xffff ^ buttons->btns; 
}