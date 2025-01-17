# NFDRS4 Nanobind
Python bindings for NFDRS4 using Nanobind

## Cloning the repository
```bash
## Download the source code into ./NFDRS4-nanobind
git clone git@github.com:keltonhalbert/NFDRS4-nanobind.git
cd ./NFDRS4-nanobind
## Download the submodule dependencies
git submodule update --init --recursive
```

## Building and Installing NFDRS4 + Python Bindings 

```bash
## The way NFDRS4's build process is set up is... not ideal.
## We have to build the cpp4config static library in-place first
## before we can call pip, and this will make sure that happens. 
## Also, at this time, this only works on Linux, but can be 
## mangled to work on Windows
cmake -B build .
cmake --build build
pip install .
```
