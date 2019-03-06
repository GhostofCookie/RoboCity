SRC_DIR 	= src/
INC_DIR 	= inc/
BIN_DIR 	= bin/

CC		= g++ 
LDFLAGS 	= -Wall
CPPFLAGS 	= -std=c++11
CXXFLAGS	= -I$(INC_DIR) -I$(INC_DIR)Objects/

SRC_FILES 	= $(wildcard $(SRC_DIR)*.cpp) $(wildcard $(SRC_DIR)Objects/*.cpp)
OBJ_FILES 	= $(patsubst $(SRC_DIR)%.cpp,$(BIN_DIR)%.o,$(SRC_FILES))

LIBDIR 		= -L/usr/lib/
LIBRARIES 	= -lX11 -lglut -lGL -lGLU -lm

app_default 	= Engine
app 		?= $(app_default)

define cc-bin-build
$(CC) $(CPPFLAGS) $(CXXFLAGS) $(INC_DIR) -c -o $@ $(LIBDIR) $< $(LIBRARIES)
endef

all: clean-all mkbin build

build: $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $(app) $^ $(LIBDIR) $(LIBRARIES)

$(BIN_DIR)%.o: $(SRC_DIR)%.cpp
	$(cc-bin-build)	

.PHONY: clean clean-all
clean:
	rm -rf $(BIN_DIR)

clean-all: clean
	rm -rf *~ $(SRC_DIR)*~ $(INC_DIR)*~
	rm -rf $(app) $(app_default)

mkbin:
	mkdir $(BIN_DIR) $(BIN_DIR)/Objects/
