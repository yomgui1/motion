OS = $(shell uname)

CC = gcc
AR = ar

CFLAGS = -noixemul -g -Wall -Wno-pointer-sign -Wl,--traditional-format -O2 -mmultiple $(INCLUDES)
ARFLAGS = cru

INCLUDES = -I. -Idetectors
LIBS = -lpng

FAST_DIR = detectors/fast
FAST_SRC = \
	$(FAST_DIR)/fast_9_detect.c \
	$(FAST_DIR)/fast_9_detect_limited.c \
	$(FAST_DIR)/fast_9_detect_nonmax.c \
	$(FAST_DIR)/fast_9_score.c \
	$(FAST_DIR)/fast_utils.c \
	$(FAST_DIR)/nonmax.c

ALL_OBJS += $(FAST_SRC:%.c=%.o)

IMT_DIR = imtool
IMT_SRC = \
	$(IMT_DIR)/error.c \
	$(IMT_DIR)/image.c \
	$(IMT_DIR)/handler.c \
	$(IMT_DIR)/handler_png.c
	
ifeq ("$(OS)","MorphOS")
IMT_SRC += $(IMT_DIR)/handler_sys_morphos.c
endif

ALL_OBJS += $(IMT_SRC:%.c=%.o)


all : libmotion.a detectors/tests/test_fast

clean:
	rm libmotion.a $(ALL_OBJS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

libmotion.a: $(ALL_OBJS)
	$(AR) $(ARFLAGS) $@ $^
	
%/test_fast: %/test_fast.o libmotion.a
	$(CC) $(CFLAGS) -o $@ $^ -L. -lmotion $(LIBS) 