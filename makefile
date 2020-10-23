cc = gcc -Wall -std=c11
all: bin/main
	./bin/main
bin/main: src/main.c
	mkdir -p bin
	$(cc) $^ -o $@
bin/test: src/main.c
	mkdir -p bin
	$(cc) -D TEST $^ -o $@
test: bin/test
	./bin/test
.PHONY: clean
clean:
	rm -rf bin
