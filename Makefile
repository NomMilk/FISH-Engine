CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
INCLUDES = -I./Libraries/Include
LDFLAGS = -lglfw -lGL -ldl -lsfml-audio -lsfml-system -lassimp -ltinyxml2

# Directories
BIN_DIR = bin
OBJ_DIR = $(BIN_DIR)/obj

# Resources
RESOURCES = Shaders/Default.vert Shaders/Default.frag

# Shared source files (common engine code)
COMMON_SRCS = \
	BoxCollider.cpp \
	Camera.cpp \
	EditorCamera.cpp \
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

# Program entry points
RUNTIME_MAIN = Main.cpp
EDITOR_MAIN  = EditorMain.cpp

# Object lists
COMMON_OBJS  = $(addprefix $(OBJ_DIR)/,$(COMMON_SRCS:.cpp=.o))
COMMON_OBJS := $(COMMON_OBJS:.c=.o)

RUNTIME_OBJS = $(COMMON_OBJS) $(OBJ_DIR)/$(RUNTIME_MAIN:.cpp=.o)
EDITOR_OBJS  = $(COMMON_OBJS) $(OBJ_DIR)/$(EDITOR_MAIN:.cpp=.o)

# Output binaries
TARGET_RUNTIME = $(BIN_DIR)/runtime
TARGET_EDITOR  = $(BIN_DIR)/editor

# Create bin + obj directory always
$(shell mkdir -p $(BIN_DIR) $(OBJ_DIR))

# Default build
all: $(TARGET_RUNTIME) $(TARGET_EDITOR) copy_resources

# Copy resources
copy_resources: $(RESOURCES)
	cp $(RESOURCES) $(BIN_DIR)/
	mkdir -p $(BIN_DIR)/models
	cp -r models/* $(BIN_DIR)/models/

# Runtime build
$(TARGET_RUNTIME): $(RUNTIME_OBJS)
	$(CXX) $(RUNTIME_OBJS) -o $(TARGET_RUNTIME) $(LDFLAGS)

# Editor build
$(TARGET_EDITOR): $(EDITOR_OBJS)
	$(CXX) $(EDITOR_OBJS) -o $(TARGET_EDITOR) $(LDFLAGS)

# Compilation rules
$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: %.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build output
clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean copy_resources
