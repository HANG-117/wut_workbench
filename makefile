build:
	@mkdir -p build
run: build
	@g++ main.cpp -o build/main
	@./build/main
clean:
	@rm -rf build
.PHONY: build run clean