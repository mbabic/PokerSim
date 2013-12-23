# Garbage poker simulator makefile.

# Directories.
BIN = bin
SRC = src

# Compiler and compiler options.
CC = cc
CFLAGS = -c -Wall

SOURCES = $(SRC)/main.c
OBJS = $(BIN)/main.o
EXE = hand_evaluator

all: $(CFLAGS) += -O3
all: $(SOURCES) $(EXE)

clean:
	rm $(BIN)/*.o $(BIN)/hand_evaluator

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(BIN)/$@

$(OBJS): $(SOURCES)
	$(CC) $(CFLAGS) $(subst bin,src,$(subst .o,.c,$@))  -o $@

