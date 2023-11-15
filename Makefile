# inspired by MakeFileTutorial from https://makefiletutorial.com/

NAME=PAINter
AUTHOR=ChristopherHosken
VERSION=v1
DATE=15.11.2023

CC=gcc

painter:
	$(CC) -lSDL2 -o $(NAME) main.c
	echo "PAINter has been compiled!"

# Copyright © 2023 Christopher Hosken