# meta build system
# invokes the typical CMake build process

.PHONY: configure build clean run test

build/:
	cmake -B build -S .

build: configure build/
	cmake --build build

clean:
	rm -rf build

run: build
	cmake --build build --target run

test: build
	cmake --build build --target test

test-verbose: build
	env CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test