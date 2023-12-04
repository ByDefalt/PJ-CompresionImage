CC = gcc
CFLAGS = -Wall -W -Wextra -Werror -Wunreachable-code -lm $(shell pkg-config --cflags glib-2.0 gtk+-3.0)
LDFLAGS = -W -lm $(shell pkg-config --libs glib-2.0 gtk+-3.0)

SRC = main.c calcul_fonction_zpixel.c calcul_fonction_pixel_image.c calcul_fonction_arbre_zpixel.c
OBJ = $(SRC:.c=.o)
HEADERS = fonction_zpixel.h fonction_pixel_image.h fonction_arbre_zpixel.h stack2.h
LIBS = stack2.o

TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean