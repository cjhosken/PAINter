# inspired by MakeFileTutorial from https://makefiletutorial.com/

NAME=PAINter
AUTHOR=ChristopherHosken
VERSION=v1
DATE=23.11.2023

CC=clang++

LINKER_FLAGS = -lSDL2 -lSDL2_image

painter:
	$(CC) main.cpp $(LINKER_FLAGS) -o $(NAME) 
	echo "PAINter has been compiled!"

# Copyright © 2023 Christopher Hosken