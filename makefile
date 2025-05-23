
OBJFILES := build/main.o build/drawing.o build/interface.o

CURRENT_DIR := $(shell pwd)

pascal: $(OBJFILES)
	clang++ $(OBJFILES) -F/Library/Frameworks/ -I/$(CURRENT_DIR)/include -framework SDL2 -o $@

build/main.o: src/main.cpp include/drawing.hpp include/interface.hpp
	clang++ -c $< -F/Library/Frameworks/ -I/$(CURRENT_DIR)/include -o $@

build/interface.o: src/interface.cpp include/interface.hpp
	clang++ -c $< -F/Library/Frameworks/ -I/$(CURRENT_DIR)/include -o $@

build/drawing.o: src/drawing.cpp include/drawing.hpp
	clang++ -c $< -F/Library/Frameworks/ -I/$(CURRENT_DIR)/include -o $@

clean:
	rm build/*
	rm pascal