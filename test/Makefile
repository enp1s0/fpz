CXX=g++
CXXFLAGS=-std=c++17 -I../include -lz

TARGET=test_fp32.cpp.test

all: $(TARGET)

%.cpp.test:%.cpp
	$(CXX) $< -o $@ $(CXXFLAGS)

clean:
	rm -f *.test
