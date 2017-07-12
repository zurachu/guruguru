# Piece �p makefile

# �g���q�̒�`

.SUFFIXES:
.SUFFIXES:  .o .s .c

# �����R�}���h�E�I�v�V�����̃}�N��

CC = pcc33
CFLAGS = -c -g -gp=0x0 -near -O2 -Wall
AS = pcc33
ASFLAGS = -c -g -gp=0x0 -near
LD = pcc33
LDFLAGS = -g -ls -lm

# �����K��

.c.o:
	$(CC) $(CFLAGS) $<

.s.o:
	$(AS) $(ASFLAGS) $<

# �\���t�@�C���E�����t�@�C���̃}�N��

PRGNAME = guruguru
FILENAME = guruguru
CAPTION = ���邮��P/ECE
OBJS =	main.o Lcd.o PrecisionTimer.o

$(PRGNAME).srf : $(OBJS)
	$(LD) $(LDFLAGS) -e$(PRGNAME).srf $(OBJS)

# �ˑ��֌W

# �t���b�V���������݃C���[�W����
pex : $(PRGNAME).srf
	ppack -e $(PRGNAME).srf -o$(FILENAME).pex -n$(CAPTION)

# �N���[���A�b�v
clean:
	del $(PRGNAME).srf $(FILENAME).pex $(PRGNAME).sym $(PRGNAME).map *.o

