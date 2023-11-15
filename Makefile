# inspired by MakeFileTutorial from https://makefiletutorial.com/

NAME=PAINter
AUTHOR=ChristopherHosken
VERSION=v1
DATE=15.11.2023

CC=gcc

LINKER_FLAGS = -lSDL2 -lSDL2_image

painter:
	$(CC) main.c $(LINKER_FLAGS) -o $(NAME) 
	echo "PAINter has been compiled!"

# Copyright © 2023 Christopher Hosken