.PHONY: sayflags

CXX=g++
CXXFLAGS=-O0 -g -std=c++17 -Wall

OBJ=main.o
DEPS=helpers.hpp kernel_executor.hpp rgba.h test_lut_1d.h test_lut_2d.h


%.o: %.c $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

demo: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) -lOpenCL

clean:
	rm *.o demo

sayflags:
	echo $(CXXFLAGS)