# PokerSim Makefile.
# http://www.github.com/mbabic/PokerSim
# Marko Tomislav Babic

# Directories.
BIN = bin
SRC = src

# Compiler and compiler options.
CC = cc
CFLAGS = -c
LDFLAGS = 
# NOTE: Edit the line below to point to the include directory of your
# installation of PokerSource.
LIB_INCLUDE = /usr/local/include/poker-eval
#-----------------------------------------------------------------------------
POKER_LIB = poker-eval
THREADING_LIB = pthread

SOURCES =  $(SRC)/main.c $(SRC)/simulator.h $(SRC)/simulator.c $(SRC)/stats.h
SOURCES += $(SRC)/stats.c $(SRC)/utils.h $(SRC)/args.h $(SRC)/parse_input.c
SOURCES += $(SRC)/parse_input.h
OBJS = $(BIN)/main.o $(BIN)/stats.o $(BIN)/simulator.o $(BIN)/parse_input.o 
EXE = hand_evaluator

all: CFLAGS += -O3 -Wno-unused-result
all: $(EXE)

debug: CFLAGS += -g -DDEBUG -Wall
debug: $(EXE)

clean:
	rm $(BIN)/*.o $(BIN)/hand_evaluator

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -l$(POKER_LIB) -l$(THREADING_LIB) -o $(BIN)/$@

$(OBJS): $(SOURCES)
	$(CC) $(CFLAGS) -I$(LIB_INCLUDE) $(subst bin,src,$(subst .o,.c,$@)) -o $@



