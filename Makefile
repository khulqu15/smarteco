CC = g++
PROJECT = ./src/image_processing
SRC = ./src/image_processing.cpp
LIBS = `pkg-config --cflags --libs opencv4`
$(PROJECT) : $(SRC)
	$(CC) $(SRC) -o $(PROJECT) $(LIBS)