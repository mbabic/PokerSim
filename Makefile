# Garbage poker simulator makefile.

# Directories.
BIN = bin
SRC = src

# Compiler and compiler options.
CC = cc
CFLAGS = -c -Wall
LDFLAGS = 
LIB_INCLUDE = /usr/local/include/poker-eval
LIB = poker-eval

SOURCES =  $(SRC)/main.c $(SRC)/simulator.h $(SRC)/simulator.c $(SRC)/stats.h
SOURCES += $(SRC)/stats.c $(SRC)/utils.h
OBJS = $(BIN)/main.o $(BIN)/stats.o $(BIN)/simulator.o
EXE = hand_evaluator

all: CFLAGS += -O3
all: $(EXE)

debug: CFLAGS += -g -DDEBUG
debug: $(EXE)

clean:
	rm $(BIN)/*.o $(BIN)/hand_evaluator

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -l$(LIB) \
	    -o $(BIN)/$@

$(OBJS): $(SOURCES)
	$(CC) $(CFLAGS) -I$(LIB_INCLUDE) $(subst bin,src,$(subst .o,.c,$@))  \
	    -o $@


