# inspired by MakeFileTutorial from https://makefiletutorial.com/

TARGET=PAINter
AUTHOR=Christopher Hosken
VERSION=v2
DATE=11.12.2023

CC = g++
SRCDIR = src
CFLAGS = -std=c++11 -Wall
LINKER_FLAGS = -lSDL2 -lSDL2_image
BUILDDIR = build
INCDIR = src

SRCS = main.cpp $(SRCDIR)/*.cpp
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

CFLAGS += -I$(INCDIR)

$(TARGET): $(OBJS)
	$(CC) ${CFLAGS} $(OBJS) $(LINKER_FLAGS) -o $(TARGET)
	echo "PAINter has been compiled!"

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(BUILDDIR)/*.o $(TARGET)

# Copyright © 2023 Christopher Hosken

#https://stackoverflow.com/questions/54951379/gtk-3-native-file-chooser-allowed-file-types-and-default-file-name