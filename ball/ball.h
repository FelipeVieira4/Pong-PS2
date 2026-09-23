#ifndef BALL_H
#define BALL_H

#include <gsKit.h>
#include <stdint.h>

#include "../player/player.h"
#include "../utils/react_utils.h"

#define BALL_SIZE 25

typedef struct{
    /* data */
    u8 speed;
    int8_t x_dir;
    int8_t y_dir;

    react_s react;
}ball_s;

ball_s init_ball(GSGLOBAL *gsGlobal);
void update_ball(GSGLOBAL *gsGlobal, ball_s *ball, player_s *player_left, player_s *player_right);

#endif