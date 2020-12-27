#pragma

#include <iostream>

#include "cl.hpp"
#include "rgba.h"
#include "helpers.hpp"

#include "test_lut_1d.h"
#include "test_lut_2d.h"
#include "test_lut_3d.h"


bool executeNonLutKernel(void** in_data, void** out_data, const uint32_t& num_entries,
                         const float& min_inval, const float& max_inval)
{
    auto data_size = sizeof(float) * num_entries;
    *in_data = std::malloc(data_size);
    float* input = reinterpret_cast<float*>(*in_data);

    for (uint32_t i = 0; i < num_entries; i++)
    {
        input[i] = (i - min_inval) * (max_inval - min_inval) / (num_entries - 1);
    }

    cl::Buffer in_data_cl(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, data_size, *in_data);
    cl::Buffer out_data_cl(context, CL_MEM_READ_WRITE, data_size);

    std::string kernel_name = "non_lut_kernel";
    uint8_t param_count = 0;

    cl::Program program;
    auto kernel = createKernel(kernel_name, program);
    std::cout << "Kernel built ....\n";

    clCheckError(kernel.setArg(param_count++, in_data_cl));
    clCheckError(kernel.setArg(param_count++, out_data_cl));
    clCheckError(kernel.setArg(param_count++, num_entries));

    const auto workgroup_size = cl::NDRange(256);
    const auto global_size    = cl::NDRange(((num_entries + 255) / 256) * 256);

    cl::Event event;
    commandqueue.enqueueNDRangeKernel(kernel, cl::NullRange, global_size, workgroup_size, NULL, &event);
    clCheckError(event.wait());

    *out_data = std::malloc(data_size);
    commandqueue.enqueueReadBuffer(out_data_cl, CL_TRUE, 0, data_size, *out_data);

    return true;
}



bool execute1DLutKernel(void** in_data, void** out_data, const uint32_t& num_entries,
                         const float& min_inval, const float& max_inval)
{
    auto data_size = sizeof(float) * num_entries;
    *in_data = std::malloc(data_size);
    float* input = reinterpret_cast<float*>(*in_data);

    for (uint32_t i = 0; i < num_entries; i++)
    {
        input[i] = (i - min_inval) * (max_inval - min_inval) / (num_entries - 1);
    }

    cl::Buffer in_data_cl(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, data_size, *in_data);
    cl::Buffer out_data_cl(context, CL_MEM_READ_WRITE, data_size);

    auto lut_ptr = (void*) lut_1d_data.data();
    cl::Image1D lut1d_cl_image(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            cl::ImageFormat(CL_R, CL_FLOAT), lut_1d_size, lut_ptr);

    std::string kernel_name = "lut1d_kernel";
    uint8_t param_count = 0;

    cl::Program program;
    auto kernel = createKernel(kernel_name, program);
    std::cout << "Kernel built ....\n";

    clCheckError(kernel.setArg(param_count++, in_data_cl));
    clCheckError(kernel.setArg(param_count++, out_data_cl));
    clCheckError(kernel.setArg(param_count++, num_entries));
    clCheckError(kernel.setArg(param_count++, lut1d_cl_image));
    clCheckError(kernel.setArg(param_count++, lut_1d_size));

    const auto workgroup_size = cl::NDRange(256);
    const auto global_size    = cl::NDRange(((num_entries + 255) / 256) * 256);

    cl::Event event;
    commandqueue.enqueueNDRangeKernel(kernel, cl::NullRange, global_size, workgroup_size, NULL, &event);
    clCheckError(event.wait());

    *out_data = std::malloc(data_size);
    commandqueue.enqueueReadBuffer(out_data_cl, CL_TRUE, 0, data_size, *out_data);

    return true;
}



bool execute2DLutKernel(void** in_data, void** out_data, const uint32_t& num_entries,
                         const float& min_inval, const float& max_inval)
{
    auto data_size = sizeof(RgbaData) * num_entries * num_entries;
    *in_data = std::malloc(data_size);
    RgbaData* input = reinterpret_cast<RgbaData*>(*in_data);

    for (uint32_t x = 0; x < num_entries; x++)
    {
        float xval = (x - min_inval)  * (max_inval - min_inval);
        xval /= num_entries - 1;
        for (uint32_t y = 0; y < num_entries; y++)
        {
            float yval = (y - min_inval)  * (max_inval - min_inval);
            yval /= num_entries - 1;
            input[x * num_entries + y] = {xval, yval, 0, 0};
        }
    }

    cl::Buffer in_data_cl(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, data_size, *in_data);
    cl::Buffer out_data_cl(context, CL_MEM_READ_WRITE, data_size);

    auto lut_ptr = (void*) lut_2d_data.data();
    cl::Image2D lut2d_cl_image(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            cl::ImageFormat(CL_RGBA, CL_FLOAT), lut_2d_size, lut_2d_size, 0,
            lut_ptr);

    std::string kernel_name = "lut2d_kernel";
    uint8_t param_count = 0;

    cl::Program program;
    auto kernel = createKernel(kernel_name, program);
    std::cout << "Kernel built ....\n";

    clCheckError(kernel.setArg(param_count++, in_data_cl));
    clCheckError(kernel.setArg(param_count++, out_data_cl));
    clCheckError(kernel.setArg(param_count++, num_entries));
    clCheckError(kernel.setArg(param_count++, lut2d_cl_image));
    clCheckError(kernel.setArg(param_count++, lut_2d_size));

    const auto workgroup_size = cl::NDRange(256);
    const auto global_size    = cl::NDRange(((num_entries + 255) / 256) * 256);

    cl::Event event;
    commandqueue.enqueueNDRangeKernel(kernel, cl::NullRange, global_size, workgroup_size, NULL, &event);
    clCheckError(event.wait());

    *out_data = std::malloc(data_size);
    commandqueue.enqueueReadBuffer(out_data_cl, CL_TRUE, 0, data_size, *out_data);

    return true;
}



bool execute3DLutKernel(void** in_data, void** out_data, const uint32_t& num_entries,
                         const float& min_inval, const float& max_inval)
{
    auto data_size = sizeof(RgbaData) * num_entries * num_entries * num_entries;
    *in_data = std::malloc(data_size);
    RgbaData* input = reinterpret_cast<RgbaData*>(*in_data);

    for (uint32_t x = 0; x < num_entries; x++)
    {
        float xval = (x - min_inval)  * (max_inval - min_inval);
        xval /= num_entries - 1;
        for (uint32_t y = 0; y < num_entries; y++)
        {
            float yval = (y - min_inval)  * (max_inval - min_inval);
            yval /= num_entries - 1;
            for (uint32_t z = 0; z < num_entries; z++)
            {
                float zval = (z - min_inval)  * (max_inval - min_inval);
                zval /= num_entries - 1; 
                input[(x * num_entries + y) * num_entries + z] = {xval, yval, zval, 0};
            }
        }
    }

    cl::Buffer in_data_cl(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, data_size, *in_data);
    cl::Buffer out_data_cl(context, CL_MEM_READ_WRITE, data_size);

    auto lut_ptr = (void*) lut_3d_data.data();
    cl::Image3D lut3d_cl_image(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            cl::ImageFormat(CL_RGBA, CL_FLOAT), lut_3d_size, lut_3d_size, lut_3d_size,
            0, 0, lut_ptr);

    std::string kernel_name = "lut3d_kernel";
    uint8_t param_count = 0;

    cl::Program program;
    auto kernel = createKernel(kernel_name, program);
    std::cout << "Kernel built ....\n";

    clCheckError(kernel.setArg(param_count++, in_data_cl));
    clCheckError(kernel.setArg(param_count++, out_data_cl));
    clCheckError(kernel.setArg(param_count++, num_entries));
    clCheckError(kernel.setArg(param_count++, lut3d_cl_image));
    clCheckError(kernel.setArg(param_count++, lut_3d_size));

    const auto workgroup_size = cl::NDRange(256);
    const auto global_size    = cl::NDRange(((num_entries + 255) / 256) * 256);

    cl::Event event;
    commandqueue.enqueueNDRangeKernel(kernel, cl::NullRange, global_size, workgroup_size, NULL, &event);
    clCheckError(event.wait());

    *out_data = std::malloc(data_size);
    commandqueue.enqueueReadBuffer(out_data_cl, CL_TRUE, 0, data_size, *out_data);

    return true;
}