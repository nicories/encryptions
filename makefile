cc = gcc -Wall -std=c11 -ggdb
all: bin/main
	./bin/main
bin/main: src/main.c
	mkdir -p bin
	$(cc) $^ -o $@
bin/test: src/test.c
	mkdir -p bin
	$(cc) $^ -o $@
test: bin/test
	./bin/test
.PHONY: clean
clean:
	rm -rf bin
