
# Source based on MakeFileTutorial and ChatGPT starts here: https://makefiletutorial.com/ https://chat.openai.com/

TARGET=PAINter
VERSION=v3
DATE=21.12.2023

CC = g++
SRCDIR = src
CFLAGS =  -std=c++11 -Wall -Iinclude
LINKER_FLAGS = -lSDL2 -lSDL2_image
BUILDDIR = build

SRCS = $(wildcard $(SRCDIR)/*.cpp) main.cpp
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

$(TARGET): $(OBJS)
	$(CC) ${CFLAGS} $(OBJS) $(LINKER_FLAGS) -o $(TARGET)
	echo "PAINter has been compiled!"

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(BUILDDIR)/*.o $(TARGET)

# Source based on MakeFileTutorial and ChatGPT ends here