CFLAGS = -Wall -Wextra
SRC = main.c game.c grid.c
TARGET = output

all: $(TARGET)

$(TARGET): $(SRC)
	gcc $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	del /f /q $(TARGET).exe 2>nul || true