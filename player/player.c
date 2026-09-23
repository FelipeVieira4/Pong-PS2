#include <tamtypes.h>
#include <gsKit.h>
#include <libpad.h>

#include "player.h"
#include "../utils/react_utils.h"
#include "../utils/pad_controller.h"

player_s init_player(u8 player_number){
    player_s new_player;

    new_player.pos_y=200;

    if (player_number==1) new_player.pos_x=0;
    else new_player.pos_x=600;

    new_player.p_number=player_number;
    new_player.speed=5;
    new_player.board=0;

    return new_player;
}

void update_player(GSGLOBAL *gsGlobal, player_s *player){
    struct padButtonStatus buttons;
    u32 paddata;

    int state = padGetState(0, 0);
    if (state == PAD_STATE_STABLE || state == PAD_STATE_FINDCTP1) {
        if (padRead(0, 0, &buttons) != 0) {
            paddata = normalize_pad_data(&buttons);

            u8 speed=player->speed;

            if (paddata & PAD_UP&&(player->pos_y-speed>=0))    player->pos_y -= speed;
            if (paddata & PAD_DOWN&&((player->pos_y+PLAYER_HEIGHT)+speed<=gsGlobal->Height))  player->pos_y += speed;
        }
    }
}

void draw_player(GSGLOBAL *gsGlobal,player_s *player){
    draw_react(gsGlobal, player->pos_x, player->pos_y, PLAYER_WIDHT, PLAYER_HEIGHT,GS_SETREG_RGBA(0xFF, 0xFF, 0xFF, 0x00));

    return;
}