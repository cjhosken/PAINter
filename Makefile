# inspired by MakeFileTutorial from https://makefiletutorial.com/

NAME=PAINter
AUTHOR=Christopher Hosken
VERSION=v1
DATE=04.12.2023

CC=g++
LINKER_FLAGS = -lSDL2 -lSDL2_image  `pkg-config --cflags --libs gtk+-3.0`

painter:
	$(CC) main.cpp $(LINKER_FLAGS) -o $(NAME)
	echo "PAINter has been compiled!"

gtk_test:
	$(CC) gtk_test.cpp $(LINKER_FLAGS) -o gtk_test
	echo "test has been compiled!"

# Copyright © 2023 Christopher Hosken

#https://stackoverflow.com/questions/54951379/gtk-3-native-file-chooser-allowed-file-types-and-default-file-name