CC := g++ -Wno-pointer-arith
SRC := main.cpp tree.cpp
SRCSIMP := simplemain.cpp SimpleList.cpp
DIR := C:\Users\USER\Documents\Akinator

.PHONY: all clean

main: $(SRC)
	$(CC) $^ -o $(DIR)\$@

simplelist: $(SRCSIMP)
	$(CC) $^ -o $(DIR)\$@
