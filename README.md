# wav-revival-lib

## How to build

### Prerequisites

* CMake version 3.13 and above
* conan

### Building with CMake

```bash
$ cmake -S . -B ./build
$ cmake --build ./build --target wav-revival
```

## How to test

```bash
$ cmake -S . -B ./build
$ cmake --build ./build --target tests_catch2
$ cd build
$ ctest 
```
