#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <libpad.h>
#include <gsKit.h>
#include <dmaKit.h>
#include <stdio.h>
#include <string.h>
#include <sbv_patches.h>
#include <iopcontrol.h>

#include "player/player.h"
#include "ball/ball.h"
#include "utils/pad_controller.h"
#include "utils/react_utils.h"

// Buffer com alinhamento obrigatório de 64 bytes para o DMA
static __attribute__((aligned(64))) char padBuf[256];

int init_pad() {
    // 1. Reset real do IOP (limpa o estado deixado pelo OPL)
    SifInitRpc(0);
    while (!SifIopReset("", 0)) {}
    while (!SifIopSync()) {}
    SifInitRpc(0);
    SifLoadFileInit();

    // 2. Patches para carregar módulos da ROM
    sbv_patch_enable_lmb();
    sbv_patch_disable_prefix_check();

    // 3. Drivers do controle
    int id1 = SifLoadModule("rom0:XSIO2MAN", 0, NULL);
    int id2 = SifLoadModule("rom0:XPADMAN", 0, NULL);
    if (id1 < 0 || id2 < 0) {
        id1 = SifLoadModule("rom0:SIO2MAN", 0, NULL);
        id2 = SifLoadModule("rom0:PADMAN", 0, NULL);
    }
    if (id1 < 0 || id2 < 0) return 0; 

    padInit(0);
    memset(padBuf, 0, sizeof(padBuf));

    if (padPortOpen(0, 0, padBuf) == 0) {
        printf("[PAD] Erro ao abrir a porta!\n");
        return 0;
    }

    // 4. Aguarda sincronização do controle
    int state;
    int timeout = 1000;
    while (timeout > 0) {
        state = padGetState(0, 0);
        if (state == PAD_STATE_STABLE || state == PAD_STATE_FINDCTP1) {
            printf("[PAD] Controle pronto! Estado: %d\n", state);
            return 1;
        }
        for (volatile int i = 0; i < 100000; i++);
        timeout--;
    }

    printf("[PAD] Timeout no controle!\n");
    return 0;
}

int main(int argc, char *argv[]) {
    GSGLOBAL *gsGlobal = gsKit_init_global();

    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);
    dmaKit_chan_init(DMA_CHANNEL_GIF);

    gsGlobal->PSM  = GS_PSM_CT32;
    gsGlobal->PSMZ = GS_PSMZ_16S;
    gsGlobal->ZBuffering      = GS_SETTING_OFF;
    gsGlobal->PrimAlphaEnable = GS_SETTING_OFF;

    gsKit_init_screen(gsGlobal);
    gsKit_mode_switch(gsGlobal, GS_ONESHOT);

    // tela vermelha = GS ok, ainda não iniciou o pad
    gsKit_clear(gsGlobal, GS_SETREG_RGBA(0xFF, 0x00, 0x00, 0x00));
    gsKit_queue_exec(gsGlobal);
    gsKit_sync_flip(gsGlobal);

    init_pad();

    // tela verde = pad terminou (com sucesso ou não)
    gsKit_clear(gsGlobal, GS_SETREG_RGBA(0x00, 0xFF, 0x00, 0x00));
    gsKit_queue_exec(gsGlobal);
    gsKit_sync_flip(gsGlobal);

    // Definindo Cores do fundo da tela (R, G, B, A)
    u64 background_color = GS_SETREG_RGBA(0x00, 0x00, 0x00, 0x00);
    u64 ball_color = GS_SETREG_RGBA(0x00, 0xFF, 0x00, 0x80);

    // Players
    player_s player_left=init_player(1);
    player_s player_right=init_player(2);

    ball_s ball=init_ball(gsGlobal);

    for (;;) {
        update_player(gsGlobal, &player_left);
        update_player(gsGlobal, &player_right);
        
        update_ball(gsGlobal,&ball,&player_left,&player_right);

        // Renderização
        gsKit_clear(gsGlobal, background_color);

        draw_player(gsGlobal,&player_left);
        draw_player(gsGlobal,&player_right);

        draw_react_pro(gsGlobal, &ball.react, ball_color);

        gsKit_queue_exec(gsGlobal);
        gsKit_sync_flip(gsGlobal);
    }

    return 0;
}