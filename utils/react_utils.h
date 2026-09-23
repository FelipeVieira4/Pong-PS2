#ifndef REACT_UTILS_H
#define REACT_UTILS_H

#include <tamtypes.h>
#include <gsKit.h>

typedef struct
{
    /* data */
    u16 pos_x;
    u16 pos_y;

    u8 size;
}react_s;

void draw_react(GSGLOBAL *gsGlobal, u16 pos_x, u16 pos_y, u16 widht, u16 height,u64 color);
void draw_react_pro(GSGLOBAL *gsGlobal, react_s *react, u64 color);

#endif