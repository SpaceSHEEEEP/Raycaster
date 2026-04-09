CC = g++
SRC_FILES = main.cpp SceneManager.cpp PlayScene.cpp EntityManager.cpp Entity.cpp Components.cpp
OUTPUT_NAME = raycaster 
LINKER = -lsfml-graphics -lsfml-window -lsfml-system

all : run

run : main.cpp
	clear
	$(CC) $(SRC_FILES) -o $(OUTPUT_NAME) $(LINKER)
	./$(OUTPUT_NAME)

clean : 
	rm $(OUTPUT_NAME) 
	clear

