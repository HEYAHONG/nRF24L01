PROJECT =nRF24L01


#tool
SDCC=sdcc
PACKIHX=packihx
S51=s51


#object
SRC = $(wildcard *.c)

OBJ1_SRC=$(patsubst %.c,%.rel,$(SRC))
OBJ_SRC=$(patsubst %.asm,%.rel,$(OBJ1_SRC))

ASM_DST=$(patsubst %.rel,%.asm,$(OBJ_SRC))
LST_DST=$(patsubst %.rel,%.lst,$(OBJ_SRC))
RST_DST=$(patsubst %.rel,%.rst,$(OBJ_SRC))
SYM_DST=$(patsubst %.rel,%.sym,$(OBJ_SRC))

#CFLags
CFLAGS +=  -mmcs51 --model-small     



all: $(PROJECT).hex 
	 -rm -rf $(OBJ_SRC) $(ASM_DST) $(LST_DST) $(RST_DST) $(SYM_DST)


$(PROJECT).hex:$(PROJECT).ihx
	$(PACKIHX) $^ > $@ 


$(PROJECT).ihx:$(OBJ_SRC)
	$(SDCC) $(CFLAGS)  $^ -o $(PROJECT).ihx

%.rel:%.c
	$(SDCC) -c $(CFLAGS) $^ -o $@
%.rel:%.asm
	$(SDCC) -c $(CFLAGS) $^ -o $@

sim: $(SOLUTION).hex
	$(S51) $^


clean:
	-rm -rf $(OBJ_SRC) $(ASM_DST) $(LST_DST) $(RST_DST) $(SYM_DST)
	-rm -rf $(PROJECT).ihx $(PROJECT).hex $(PROJECT).lk $(PROJECT).mem  $(PROJECT).map

