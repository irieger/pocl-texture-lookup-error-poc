#pragma

#include <iostream>

#include "cl.hpp"
#include "rgba.h"
#include "helpers.hpp"

#include "test_lut_1d.h"
#include "test_lut_2d.h"


bool executeNonLutKernel(void** in_data, void** out_data, const uint32_t& num_entries,
                         const float& min_inval, const float& max_inval)
{
    auto data_size = sizeof(float) * num_entries;
    *in_data = std::malloc(data_size);
    float* input = reinterpret_cast<float*>(*in_data);

    for (uint32_t i = 0; i < num_entries; i++)
    {
        input[i] = i * (max_inval - min_inval) / (num_entries - 1);
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