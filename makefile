# ==============================================================================
# MIT License

# Copyright (c) 2017 Brandon Bluemner

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
# ==============================================================================
OUTPUT_DIR= bin
COMPILER = g++
FLAGS =-W -ggdb -std=c++11
SOURCE_DIR = source/
HEADERS_DIR = headers/

all: hw_one hw_two

hw_one: $(SOURCE_DIR)hw1.cpp | make_dir
	$(COMPILER) $(FLAGS) $(HEADERS_DIR)linear_system.hpp $(SOURCE_DIR)hw1.cpp -o $(OUTPUT_DIR)/hw1.o
hw_two: $(SOURCE_DIR)hw2.cpp | make_dir
	$(COMPILER) $(FLAGS) $(HEADERS_DIR)linear_system.hpp $(SOURCE_DIR)hw2.cpp -o $(OUTPUT_DIR)/hw2.o

make_dir:
	mkdir -p $(OUTPUT_DIR) | mkdir -p data
