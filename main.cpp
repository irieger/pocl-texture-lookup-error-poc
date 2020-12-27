#include <iostream>

#include "cl.hpp"
#include "statics.h"
#include "helpers.hpp"
#include "kernel_executor.hpp"


int main(int argc, char* argv[])
{
    std::cout << "OpenCL test code for LUT execution tests ...\n\n";
    
    //  SECTION:  Read user parameters
    if (argc <= 1)
    {
        std::cerr << "Requires one parameter, which has to be an int\n"
                  << "0 = test kernel without lut, 1 = 1D lut kernel, 2 = 2D lut kernel, 3 = 3D lut kernel\n";
        return 1;
    }

    int mode = -1;
    try
    {
        mode = std::stoi(argv[1]);
    }
    catch(const std::exception& e)
    {
        std::cerr << "[ERROR] Invalid operation mode parameter\n";
        return 1;
    }
    if (mode < 0 || mode > 3)
    {
        std::cerr << "[ERROR] Invalid operation mode parameter" << mode << "\n";
        return 1;
    }
    
    //  SECTION:  Base opencl state objects
    cl_int cl_error_val;

    bool device_found = false;

    //  SECTION:  Search and initialized first available OpenCL device
    cl::Platform::get(&platforms);
    if (platforms.size() == 0)
    {
        std::cerr << "[ERROR] No opencl platforms found!\n";
        return 2;
    }

    for (auto& platform : platforms)
    {
        std::string platform_info;
        clCheckError(platform.getInfo(CL_PLATFORM_NAME, &platform_info));
        std::cout << "\n\nOpenCL platform found: " << platform_info << '\n';

        cl_context_properties properties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)(platform)(), 0};
        cl::Context context_local = cl::Context(CL_DEVICE_TYPE_ALL, properties);

        std::vector<cl::Device> dev_list = context_local.getInfo<CL_CONTEXT_DEVICES>();
        if (dev_list.size() > 0)
        {
            device = dev_list[0];
            auto dev_type = device.getInfo<CL_DEVICE_TYPE>();
            context = cl::Context(dev_type, properties);
            device_found = true;

            commandqueue = cl::CommandQueue(context, device, 0, &cl_error_val);
            clCheckError(cl_error_val);

            std::cout << "\n  OpenCL device found: \n" << getDeviceInfoBox(device) << '\n';
            break;
        }
    }
    if (!device_found)
    {
        std::cerr << "[ERROR] No OpenCL device found, exiting\n";
        return 2;
    }

    //  SECTION:  Create and execute kernel based on user decision
    float min_input_value = 0;
    float max_input_value = 1;
    uint32_t num_entries;

    void* in_data;
    void* result_data;

    if (mode == 0)
    {
        std::cout << "\n\n[EXECUTING]: Base non-lookup based kernel for checking if basic OpenCL is working\n";
        num_entries = 10;
        executeNonLutKernel(&in_data, &result_data, num_entries, min_input_value, max_input_value);

        float* input  = reinterpret_cast<float*>(in_data);
        float* result = reinterpret_cast<float*>(result_data);
        std::cout << "Kernel results:\n";
        for (uint32_t i = 0; i < num_entries; i++)
        {
            std::cout << "  result[" << i << "] = " << result[i] << "  (for input " << input[i] <<  '\n';
        }
    }
    else if (mode == 1)
    {
        std::cout << "\n\n[EXECUTING]: 1D Lut Lookup\n";
        num_entries = 30;
        execute1DLutKernel(&in_data, &result_data, num_entries, min_input_value, max_input_value);

        float* input  = reinterpret_cast<float*>(in_data);
        float* result = reinterpret_cast<float*>(result_data);
        std::cout << "Kernel results:\n";
        for (uint32_t i = 0; i < num_entries; i++)
        {
            std::cout << "  result[" << i << "] = " << result[i] << "  (for input " << input[i] <<  '\n';
        }
    }
    else if (mode == 2)
    {
        std::cout << "\n\n[EXECUTING]: 2D Lut Lookup\n";
        num_entries = 4;
        execute2DLutKernel(&in_data, &result_data, num_entries, min_input_value, max_input_value);

        RgbaData* input  = reinterpret_cast<RgbaData*>(in_data);
        RgbaData* result = reinterpret_cast<RgbaData*>(result_data);
        std::cout << "Kernel results:\n";
        for (uint32_t i = 0; i < num_entries * num_entries; i++)
        {
            std::cout << "  result[" << i << "] = " << result[i] << "  (for input " << input[i] <<  '\n';
        }
    }
    else if (mode == 3)
    {
        std::cout << "\n\n[EXECUTING]: 3D Lut Lookup\n";
        num_entries = 3;
        execute3DLutKernel(&in_data, &result_data, num_entries, min_input_value, max_input_value);

        RgbaData* input  = reinterpret_cast<RgbaData*>(in_data);
        RgbaData* result = reinterpret_cast<RgbaData*>(result_data);
        std::cout << "Kernel results:\n";
        for (uint32_t i = 0; i < num_entries * num_entries * num_entries; i++)
        {
            std::cout << "  result[" << i << "] = " << result[i] << "  (for input " << input[i] <<  '\n';
        }
    }


    std::cout << "\n\nEnd ...\n";

    return 0;
}