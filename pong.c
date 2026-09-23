#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <libpad.h>
#include <gsKit.h>
#include <dmaKit.h>
#include <stdio.h>
#include <string.h>

#include "player/player.h"
#include "ball/ball.h"
#include "utils/pad_controller.h"
#include "utils/react_utils.h"

// Buffer com alinhamento obrigatório de 64 bytes para o DMA
static __attribute__((aligned(64))) char padBuf[256];

int init_pad() {
    load_module_pad();

    // Aguarda o registro dos servicos RPC no IOP
    for (volatile int i = 0; i < 500000; i++);

    padInit(0);
    memset(padBuf, 0, sizeof(padBuf));


    if (padPortOpen(0, 0, padBuf) == 0) {
        printf("[PAD] Erro ao abrir a porta 0, 0!\n");
        return 0;
    }


    // Apenas uma validação se o controle está conectado

    printf("[PAD] Aguardando sincronizacao do controle...\n");
    int state;
    int timeout = 1000;

    while (timeout > 0) {
        state = padGetState(0, 0);

        // PAD_STATE_STABLE ou PAD_STATE_FINDCTP1 indicam controle pronto
        if (state == PAD_STATE_STABLE || state == PAD_STATE_FINDCTP1) {
            printf("[PAD] Controle detectado com sucesso! (Estado: %d)\n", state);
            return 1;
        }

        // Delay para permitir que o barramento SIO2 processe as interrupções
        for (volatile int i = 0; i < 200000; i++);
        timeout--;
    }

    printf("[PAD] Timeout ao conectar! Estado final do controle: %d\n", padGetState(0, 0));
    return 0;
}

int main(int argc, char *argv[]) {
    init_pad();

    // Inicializa o gsKit (Vídeo) e dmaKit (Transferência de Dados)
    GSGLOBAL *gsGlobal = gsKit_init_global();
    gsGlobal->PSM  = GS_PSM_CT32;
    gsGlobal->PSMZ = GS_PSMZ_16S;
    gsGlobal->ZBuffering      = GS_SETTING_OFF;
    gsGlobal->PrimAlphaEnable = GS_SETTING_OFF;

    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);
    dmaKit_chan_init(DMA_CHANNEL_GIF);

    gsKit_init_screen(gsGlobal);
    gsKit_mode_switch(gsGlobal, GS_ONESHOT);

    // Definindo Cores do fundo da tela (R, G, B, A)
    u64 background_color = GS_SETREG_RGBA(0x00, 0x00, 0x00, 0x00);
    u64 ball_color = GS_SETREG_RGBA(0x00, 0xFF, 0x00, 0x00);

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