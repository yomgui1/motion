OS = $(shell uname)

CC = gcc
AR = ar

CFLAGS = $(OPTS)
CPPFLAGS = -g -Wall $(WARNS) -O2 $(OPTS) $(INCLUDES) $(DEFINES)
ARFLAGS = cru

INCLUDES = -I. -Idetectors
LIBS = -lpng

ifeq ("$(OS)","MorphOS")
OPTS = -mmultiple -noixemul
WARNS = -Wno-pointer-sign -Wl,--traditional-format
DEFINES = -D__MORPHOS_SHAREDLIBS=1
endif

FAST_DIR = detectors/fast
FAST_SRC = \
	$(FAST_DIR)/fast_9_detect.c \
	$(FAST_DIR)/fast_9_detect_limited.c \
	$(FAST_DIR)/fast_9_detect_nonmax.c \
	$(FAST_DIR)/fast_9_score.c \
	$(FAST_DIR)/fast_utils.c \
	$(FAST_DIR)/nonmax.c

ALL_OBJS += $(FAST_SRC:%.c=%.o)

MAT_DIR = math
MAT_SRC = \
	$(MAT_DIR)/array.c \
	$(MAT_DIR)/convolution.c

MAT_INCS = \
	$(MAT_DIR)/array.h \
	$(MAT_DIR)/convolution.h

ALL_OBJS += $(MAT_SRC:%.c=%.o)
ALL_INCS += $(MAT_INCS)

IMT_DIR = imtool
IMT_SRC = \
	$(IMT_DIR)/error.c \
	$(IMT_DIR)/image.c \
	$(IMT_DIR)/handler.c \
	$(IMT_DIR)/handler_png.c

ifeq ("$(OS)","MorphOS")
IMT_SRC += $(IMT_DIR)/handler_sys_morphos.c
else
IMT_SRC += $(IMT_DIR)/handler_sys_generic.c
endif

ALL_OBJS += $(IMT_SRC:%.c=%.o)


all : libmotion.a

tests: \
	detectors/fast/tests \
	math/tests \
	imtool/tests

clean:
	rm libmotion.a $(ALL_OBJS)

.c.o:
	$(CC) $(CPPFLAGS) -c -o $@ $<

libmotion.a: $(ALL_OBJS)
	$(AR) $(ARFLAGS) $@ $^

%/tests: %/tests.o libmotion.a $(ALL_INCS)
	$(CC) $(CFLAGS) -o $@ $< -L. -lmotion $(LIBS)