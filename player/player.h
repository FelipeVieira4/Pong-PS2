#ifndef PLAYER_H
#define PLAYER_H

#include <tamtypes.h>
#include <stdbool.h>

#include "../utils/react_utils.h"

#define PLAYER_WIDHT 18
#define PLAYER_HEIGHT 98

typedef struct{
    u16 pos_x;
    u16 pos_y;

    u8 speed;
    u8 p_number;

    u8 board;
}player_s;

player_s init_player(u8 player_number);
void update_player(GSGLOBAL *gsGlobal,player_s *player);
void draw_player(GSGLOBAL *gsGlobal, player_s *player);

bool check_collision(player_s *player_s,react_s *react);

#endif