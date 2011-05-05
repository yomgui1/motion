OS = $(shell uname)

CC = gcc
AR = ar

CFLAGS = $(OPTS)
CPPFLAGS = 2>&1 -g -Wall $(WARNS) -O2 $(OPTS) $(INCLUDES) $(DEFINES)
ARFLAGS = cru

INCLUDES = -I. -Idetectors -Imatchers
LIBS = -lpng -lsyscall

ifeq ("$(OS)","MorphOS")
OPTS = -mmultiple -noixemul
WARNS = -Wno-pointer-sign -Wl,--traditional-format
DEFINES = -D__MORPHOS_SHAREDLIBS=1
endif

###############################################################################

FAST_DIR = detectors/fast
FAST_SRC = \
	$(FAST_DIR)/fast_9_detect.c \
	$(FAST_DIR)/fast_9_detect_limited.c \
	$(FAST_DIR)/fast_9_detect_nonmax.c \
	$(FAST_DIR)/fast_9_score.c \
	$(FAST_DIR)/fast_utils.c \
	$(FAST_DIR)/nonmax.c

FAST_OBJS = $(FAST_SRC:%.c=%.o)

ALL_OBJS += $(FAST_OBJS)
ALL_INCS += $(FAST_INCS)

$(FAST_DIR)/%.o: $(FAST_DIR)/%.c $(FAST_INCS)

###############################################################################

MAT_DIR = math
MAT_SRC = \
	$(MAT_DIR)/array.c \
	$(MAT_DIR)/matrix.c \
	$(MAT_DIR)/convolution.c
MAT_INCS = \
	$(MAT_DIR)/array.h \
	$(MAT_DIR)/matrix.h \
	$(MAT_DIR)/convolution.h

MAT_OBJS = $(MAT_SRC:%.c=%.o)

ALL_OBJS += $(MAT_OBJS)
ALL_INCS += $(MAT_INCS)

$(MAT_DIR)/%.o: $(MAT_DIR)/%.c $(MAT_INCS)

###############################################################################

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

IMT_OBJS = $(IMT_SRC:%.c=%.o)

ALL_OBJS += $(IMT_OBJS)
ALL_INCS += $(IMT_INCS)

$(IMT_DIR)/%.o: $(IMT_DIR)/%.c $(IMT_INCS)

###############################################################################

KLT_DIR = matchers/klt
KLT_SRC = \
	$(KLT_DIR)/klt.c
KLT_INCS = \
	$(KLT_DIR)/klt.h

KLT_OBJS = $(KLT_SRC:%.c=%.o)

ALL_OBJS += $(KLT_OBJS)
ALL_INCS += $(KLT_INCS)

$(KLT_DIR)/%.o: $(KLT_DIR)/%.c $(KLT_INCS)

###############################################################################

all : libmotion.a

tests: \
	detectors/fast/tests \
	math/tests \
	imtool/tests \
	matchers/klt/tests

clean:
	-rm -f libmotion.a $(ALL_OBJS)

.c.o:
	$(CC) $(CPPFLAGS) -c -o $@ $<

libmotion.a: $(ALL_OBJS)
	$(AR) $(ARFLAGS) $@ $^

%/tests: %/tests.o libmotion.a
	$(CC) $(CFLAGS) -o $@ $< -L. -lmotion $(LIBS)