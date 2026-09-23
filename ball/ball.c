#include <gsKit.h>

#include "ball.h"
#include "../utils/react_utils.h"

ball_s init_ball(GSGLOBAL *gsGlobal){
    ball_s new_ball;

    new_ball.y_dir=1;
    new_ball.x_dir=1;
    
    new_ball.speed=5;

    react_s ball_react;
    ball_react.pos_x=gsGlobal->Width/2;
    ball_react.pos_y=gsGlobal->Height/2;
    ball_react.size=BALL_SIZE;

    new_ball.react=ball_react;
    
    return new_ball;
}

void update_ball(GSGLOBAL *gsGlobal, ball_s *ball, player_s *player_left, player_s *player_right){
    ball->react.pos_x += ball->speed * ball->x_dir;
    ball->react.pos_y += ball->speed * ball->y_dir;

    //if (ball->react.pos_y-ball->speed<=0) ball->y_dir=1;
    if (ball->react.pos_y <= 0){
        ball->y_dir = 1;
    }
    if (ball->react.pos_y + ball->react.size >= gsGlobal->Height){
        ball->y_dir = -1;
    }

    if (check_collision(player_left, &ball->react)){
        ball->x_dir = 1;
    }
    if (check_collision(player_right, &ball->react)){
        ball->x_dir = -1;
    }

    if (ball->react.pos_x <= 0){
        *ball = init_ball(gsGlobal);
        ball->x_dir = 1;
    }

    if (ball->react.pos_x + ball->react.size >= gsGlobal->Width){
        *ball = init_ball(gsGlobal);
        ball->x_dir = -1;
    }
}