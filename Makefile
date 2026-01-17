CC = g++
CFLAGS = `pkg-config --cflags sdl2` `pkg-config --cflags SDL2_image` `pkg-config --cflags SDL2_mixer`
LDFLAGS = `pkg-config --libs sdl2` `pkg-config --libs SDL2_image` `pkg-config --libs SDL2_mixer` -lGL -lGLEW
CXXFLAGS = -I./include -std=c++11

SRC_DIR = src
BIN_DIR = bin

SOURCES = main.cpp Camera.cpp Mesh.cpp Shader.cpp Texture.cpp FileLoader.cpp
OBJECTS = $(addprefix $(BIN_DIR)/,$(SOURCES:.cpp=.o))

TARGET = $(BIN_DIR)/gl3d

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

bin/%.o: src/%.cpp | bin
	$(CC) -c $< $(CFLAGS) $(CXXFLAGS) -o $@

bin:
	mkdir -p bin

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf bin

.PHONY: clean run

