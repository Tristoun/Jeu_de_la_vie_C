CFLAGS = -Wall -Wextra
SRC = main.c game.c grid.c
TARGET = output

all: $(TARGET)

$(TARGET): $(SRC)
	gcc $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)