IDIR=../include
CC=gcc
CFLAGS=-O3 -I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lmpfr -lgmp -lm

_DEPS = \
	jintailwe.h \
	dgs_bern.h \
	dgs_gauss.h \
	dgs_misc.h \
	dgs_rround.h \
	dgs.h 
	
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = \
	jintailwe.o \
	dgs_bern.o \
	dgs_gauss_dp.o \
	dgs_gauss_mp.o \
	dgs_rround_dp.o \
	dgs_rround_mp.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

jintailwe: $(OBJ)
	$(CC) -fopenmp -o  $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
