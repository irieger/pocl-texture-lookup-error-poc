.PHONY: sayflags

CXX=g++
CXXFLAGS=-O0 -g -std=c++17 -Wall

OBJ=main.o


%.o: %.c
	$(CXX) $(CXXFLAGS) -c -o $@ $<

demo: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) -lOpenCL

sayflags:
	echo $(CXXFLAGS)