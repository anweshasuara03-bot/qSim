CC = gcc
CFLAGS = -Wall -O2
SRC = src/qSim.c
BIN = bin/qSim
INCLUDE = -Iinclude

all: $(BIN)

$(BIN): $(SRC)
        $(CC) $(CFLAGS) $(INCLUDE) $(SRC) -o $(BIN) -lm

run: $(BIN)
        ./$(BIN) 100 4 60 2.3

clean:
        rm -f $(BIN)
