CC=g++
CC_FLAGS=-g -Wall -pedantic -std=c++11
LD_FLAGS=
CPP_FILES=$(wildcard src/*.cpp)
OBJ_FILES=$(addprefix build/obj/,$(notdir $(CPP_FILES:.cpp=.o)))

build: build/teeton

build/teeton: $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -o $@ $^

build/obj/%.o: src/%.cpp
	@mkdir -p build/obj
	$(CC) $(CC_FLAGS) -c -o $@ $<

clean:
	@rm -rf build/*

run: build
	./build/teeton

.PHONY: test
test: build
	@cd tests && ./runner.sh

install: build
	cp build/teeton /usr/local/bin

.PHONY: uninstall
uninstall:
	rm /usr/local/bin/teeton
