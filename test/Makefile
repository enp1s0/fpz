CXX=g++
CXXFLAGS=-std=c++14 -I../include

TARGET=test_fp32.cpp.test

all: $(TARGET)

%.cpp.test:%.cpp
	$(CXX) $< -o $@ $(CXXFLAGS)

clean:
	rm -f *.test
