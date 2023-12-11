# inspired by MakeFileTutorial from https://makefiletutorial.com/

NAME=PAINter
AUTHOR=Christopher Hosken
VERSION=v2
DATE=11.12.2023

CC=g++
LINKER_FLAGS = -lSDL2 -lSDL2_image

SRCS = main.cpp src/pntr_vector2i.cpp src/gui/pntr_button.cpp src/gui/pntr_canvas.cpp src/gui/pntr_circle.cpp src/gui/pntr_colordialog.cpp src/gui/pntr_gui.cpp src/gui/pntr_panel.cpp src/gui/pntr_slider.cpp src/gui/pntr_widget.cpp 



painter:
	$(CC) ${SRCS} $(LINKER_FLAGS) -o $(NAME)
	echo "PAINter has been compiled!"


# Copyright © 2023 Christopher Hosken

#https://stackoverflow.com/questions/54951379/gtk-3-native-file-chooser-allowed-file-types-and-default-file-name