CC = g++
CXXFLAGS = -std=c++17
SRC_FILES = main.cpp SceneManager.cpp PlayScene.cpp EntityManager.cpp Entity.cpp Components.cpp
OUTPUT_NAME = raycaster 
LINKER = -lsfml-graphics -lsfml-window -lsfml-system

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	BREW_SFML := $(shell brew --prefix sfml)
	CXXFLAGS += -I$(BREW_SFML)/include
	LINKER  += -L$(BREW_SFML)/lib -Wl,-rpath,$(BREW_SFML)/lib
endif

all : run

run : main.cpp
	clear
	$(CC) $(CXXFLAGS) $(SRC_FILES) -o $(OUTPUT_NAME) $(LINKER)
	./$(OUTPUT_NAME)

clean : 
	rm $(OUTPUT_NAME) 
	clear


# g++ -std=c++17 -I/opt/homebrew/opt/sfml/include main.cpp -o app -lsfml-graphics -lsfml-window -lsfml-system -L/opt/homebrew/opt/sfml/lib -Wl,-rpath,/opt/homebrew/opt/sfml/lib
