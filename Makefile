CC := g++ -Wno-pointer-arith -Wwrite-strings
SRC := main.cpp tree.cpp akinator.cpp text-sort.cpp stack.cpp
SRCSIMP := simplemain.cpp SimpleList.cpp
DIR := C:\Users\USER\Documents\Akinator

.PHONY: all clean

main: $(SRC)
	$(CC) $^ -o $(DIR)\$@

simplelist: $(SRCSIMP)
	$(CC) $^ -o $(DIR)\$@

clean:
	rm -rf *.png *.dot
