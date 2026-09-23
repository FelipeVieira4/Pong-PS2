#include <gsKit.h>

#include "react_utils.h"

void draw_react(GSGLOBAL *gsGlobal, u16 pos_x, u16 pos_y, u16 widht, u16 height,u64 color){
    gsKit_prim_quad(gsGlobal,
        (float) pos_x,(float) pos_y,
        (float) pos_x+widht,(float) pos_y,
        (float) pos_x,(float) pos_y+height,
        (float) pos_x+widht,(float) pos_y+height,
        0,color);

    return;
}
void draw_react_pro(GSGLOBAL *gsGlobal, react_s *react, u64 color){

    u8 size=react->size;

    gsKit_prim_quad(gsGlobal,
        (float) react->pos_x,(float) react->pos_y,
        (float) react->pos_x+size,(float) react->pos_y,
        (float) react->pos_x,(float) react->pos_y+size,
        (float) react->pos_x+size,(float) react->pos_y+size,
        0,color);
    
    return;
}