CXX=g++
CXXFLAGS=-std=gnu++17 -O2
TARGET=filex

all:
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
