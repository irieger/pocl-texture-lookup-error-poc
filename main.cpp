#include <iostream>

#include "cl.hpp"
#include "helpers.hpp"


int main(int argc, char* argv[])
{
    std::cout << "OpenCL test code for LUT execution tests ...\n\n";
    
    if (argc == 1)
    {
        std::cerr << "Requires one parameter, which has to be an int\n"
                  << "0 = test kernel without lut, 1 = 1D lut kernel, 2 = 2D lut kernel, 3 = 3D lut kernel\n";
        return 1;
    }


    std::cout << "End ...\n";

    return 0;
}