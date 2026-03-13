CSRC := $(shell find csrc -name "*.c")

build:
	@mkdir -p build

run: build
	@gcc -Wall -I include $(CSRC) -o build/main
	@./build/main

clean:
	@rm -rf build

.PHONY: build run clean