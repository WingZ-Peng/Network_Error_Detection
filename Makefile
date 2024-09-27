CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

all: error_detection.cpp
	$(CXX) $(CXXFLAGS) error_detection.cpp -o error_detection

clean:
	$(RM) error_detection