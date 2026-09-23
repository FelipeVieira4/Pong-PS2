EE_BIN = pong.elf

EE_OBJS_DIR = obj/

EE_SRC  = $(wildcard *.c utils/*.c player/*.c)
EE_OBJS = $(addprefix $(EE_OBJS_DIR), $(EE_SRC:.c=.o))

EE_INCS += -I$(GSKIT)/include -I$(PS2SDK)/ports/include
EE_LIBS += -L$(GSKIT)/lib -L$(PS2SDK)/ports/lib -lgskit -ldmakit -lpad -lpacket -ldma -lgraph -ldraw -lpacket2 -lmath3d -lkernel -lc

all: $(EE_BIN)
	$(EE_STRIP) --strip-all $(EE_BIN)

# regra que compila qualquer .c (na raiz ou em subpastas) para obj/<mesmo caminho>.o
$(EE_OBJS_DIR)%.o: %.c
	@mkdir -p $(dir $@)
	$(EE_CC) -c $(EE_CFLAGS) $(EE_INCS) -o $@ $<

clean:
	rm -f $(EE_BIN)
	rm -rf $(EE_OBJS_DIR)

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

reset:
	ps2client reset

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal