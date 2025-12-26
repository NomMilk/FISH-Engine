OS ?= $(shell uname -s)

ifeq ($(OS), windows)
	CXX = x86_64-w64-mingw32-g++
	EXE_EXT = .exe
else
	CXX = g++
	EXE_EXT = 
endif

CXXFLAGS = -Wall -Wextra -std=c++17
INCLUDES = -I./Libraries/Include

ifeq ($(OS), windows)
	LDFLAGS = -lglfw3 -lopengl32 -lgdi32 -lsfml-audio -lsfml-system -lassimp -ltinyxml2
else
	LDFLAGS = -lglfw -lGL -ldl -lsfml-audio -lsfml-system -lassimp -ltinyxml2
endif

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
	EngineUserHooks.cpp \
	UserMain.cpp \
	glad.c \
	stb.cpp

# Program entry points
RUNTIME_MAIN = Main.cpp
EDITOR_MAIN  = EditorMain.cpp

# Object lists
COMMON_OBJS  = $(addprefix $(OBJ_DIR)/,$(COMMON_SRCS:.cpp=.o))
COMMON_OBJS := $(COMMON_OBJS:.c=.o)

RUNTIME_OBJS = $(COMMON_OBJS) $(OBJ_DIR)/$(RUNTIME_MAIN:.cpp=.o)
EDITOR_IMGUI_SRCS = Libraries/Include/imgui/imgui.cpp Libraries/Include/imgui/imgui_draw.cpp Libraries/Include/imgui/imgui_widgets.cpp Libraries/Include/imgui/imgui_tables.cpp Libraries/Include/imgui/imgui_impl_glfw.cpp Libraries/Include/imgui/imgui_impl_opengl2.cpp
EDITOR_IMGUI_OBJS = $(addprefix $(OBJ_DIR)/,$(notdir $(EDITOR_IMGUI_SRCS:.cpp=.o)))
EDITOR_OBJS  = $(COMMON_OBJS) $(EDITOR_IMGUI_OBJS) $(OBJ_DIR)/$(EDITOR_MAIN:.cpp=.o)

# Output binaries
TARGET_RUNTIME = $(BIN_DIR)/runtime$(EXE_EXT)
TARGET_EDITOR  = $(BIN_DIR)/editor$(EXE_EXT)

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

$(OBJ_DIR)/imgui_impl_glfw.o: Libraries/Include/imgui/imgui_impl_glfw.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/imgui_impl_opengl2.o: Libraries/Include/imgui/imgui_impl_opengl2.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/imgui.o: Libraries/Include/imgui/imgui.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/imgui_draw.o: Libraries/Include/imgui/imgui_draw.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/imgui_widgets.o: Libraries/Include/imgui/imgui_widgets.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/imgui_tables.o: Libraries/Include/imgui/imgui_tables.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build output
clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean copy_resources
