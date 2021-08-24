CXX=g++
CXXFLAGS=-std=c++17 -I../include -lz -fopenmp

TARGET=compress_decompress.cpp.test

all: $(TARGET)

%.cpp.test:%.cpp
	$(CXX) $< -o $@ $(CXXFLAGS)

clean:
	rm -f *.test
