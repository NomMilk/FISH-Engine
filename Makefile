CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
INCLUDES = -I./Libraries/Include
LDFLAGS = -lglfw -lGL -ldl -lsfml-audio -lsfml-system -lassimp -ltinyxml2

# Source files
SRCS = Main.cpp \
       BoxCollider.cpp \
       Camera.cpp \
       Texture.cpp \
       VBO.cpp \
       EBO.cpp \
       shaderClass.cpp \
       SoundManager.cpp \
       Model.cpp \
       modelLoader.cpp \
	   SceneManager.cpp \
       glad.c \
       stb.cpp

# Resource files
RESOURCES = Shaders/Default.vert Shaders/Default.frag

# Object files
OBJS = $(SRCS:.cpp=.o)
OBJS := $(OBJS:.c=.o)

# Directories
BIN_DIR = bin
OBJ_DIR = $(BIN_DIR)/obj

# Executable name
TARGET_RUNTIME = $(BIN_DIR)/runtime
TARGET_EDITOR = $(BIN_DIR)/editor

# Create necessary directories
$(shell mkdir -p $(BIN_DIR) $(OBJ_DIR))

# Default target
all: $(TARGET_RUNTIME) $(TARGET_EDITOR) copy_resources

# Copy resources to bin directory
copy_resources: $(RESOURCES)
	cp $(RESOURCES) $(BIN_DIR)/
	mkdir -p $(BIN_DIR)/models
	cp -r models/* $(BIN_DIR)/models/

# Linking
$(TARGET_RUNTIME): $(addprefix $(OBJ_DIR)/,$(OBJS))
	$(CXX) $(addprefix $(OBJ_DIR)/,$(OBJS)) -o $(TARGET_RUNTIME) $(LDFLAGS)

$(TARGET_EDITOR): $(addprefix $(OBJ_DIR)/,$(OBJS))
	$(CXX) $(addprefix $(OBJ_DIR)/,$(OBJS)) -o $(TARGET_EDITOR) $(LDFLAGS)

# Compilation
$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: %.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean
clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean copy_resources 
