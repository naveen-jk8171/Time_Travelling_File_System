#!/bin/bash

# Clear previous build
echo "Cleaning previous builds..."
rm -f a.out

# Compile all cpp files in src/, include headers from include/
echo "Compiling project..."
g++ -std=c++17 -Iinclude src/*.cpp -o a.out

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo "Running program..."
    ./a.out
else
    echo "Compilation failed!"
fi