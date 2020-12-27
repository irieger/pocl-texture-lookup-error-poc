#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include "cl.hpp"


std::string getOpenClErrorText(cl_int error_code);

void clCheckError(cl_int error)
{
    if (error != CL_SUCCESS)
    {
        std::cerr << "\n[ERROR] OpenCL error: " << error << ":\n"
                  << getOpenClErrorText(error) << "\n\n";
    }
}


inline std::string loadKernel(const std::string& kernel_name)
{
    std::filesystem::path fname = "kernels/" + kernel_name + ".cl";
    if (std::filesystem::is_regular_file(fname))
    {
        std::ostringstream code;

        // std::string line;
        char line[5001];
        std::ifstream file;
        file.open(fname);
        if (file.is_open())
        {
            int max_length = 5000;
            while (file.getline(line, max_length))
            {
                code << line << "\n";
            }
            file.close();
            return code.str();
        }
    }

    return "";
}


inline cl::Kernel createKernel(const std::string& kernel_name, cl::Program& program)
{
    cl_int error_val;
    auto kernel_code = loadKernel(kernel_name);

    cl::Program::Sources source(1, std::make_pair(kernel_code.c_str(), kernel_code.length()));
    program = cl::Program(context, source, &error_val);
    clCheckError(error_val);

    error_val = program.build();
    clCheckError(error_val);
    if (error_val != CL_SUCCESS)
    {
        std::cout << "Build log: \n" << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << "\n\n";
    }

    auto kernel = cl::Kernel(program, kernel_name.c_str(), &error_val);
    clCheckError(error_val);

    return kernel;
}


std::string getDeviceInfoBox(cl::Device &dev)
{
    std::ostringstream oss;

    oss << "  OpenCL device:            "  << dev.getInfo<CL_DEVICE_NAME>() << " (by " << dev.getInfo<CL_DEVICE_VENDOR>() << ")" << std::endl
        << "    >  global memory:       "  << dev.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() << std::endl
        << "    >  local memory :       "  << dev.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() << std::endl
        << "    >  max comp units:      "  << dev.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << std::endl
        << "    >  max memory alloc:    "  << dev.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>() << std::endl
        << "    >  max workgroup size:  "  << dev.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << std::endl
        << "    >  max work item sizes: [";

    auto workgroup_sizes = dev.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
    for (uint32_t i = 0; i < workgroup_sizes.size(); i++)
    {
        if (i != 0)
            oss << ", ";
        oss << workgroup_sizes[i];
    }
    oss << "]" << std::endl
        << "    >  device type:         "  << dev.getInfo<CL_DEVICE_TYPE>() << std::endl;

    return oss.str();
}



std::string getOpenClErrorText(cl_int error_code)
{
    switch (error_code)
    {
        case CL_SUCCESS:
            return "SUCCESS";
            break;
        case CL_DEVICE_NOT_FOUND:
            return "No device that matches given device type found - CL_DEVICE_NOT_FOUND";
            break;
        case CL_DEVICE_NOT_AVAILABLE:
            return "No OpenCL compatible device was found - CL_DEVICE_NOT_AVAILABLE";
            break;
        case CL_COMPILER_NOT_AVAILABLE:
            return "OpenCL Compiler perhaps failed to configure itself, or check your OpenCL installation - CL_COMPILER_NOT_AVAILABLE";
            break;
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:
            return "Failed to allocate memory for buffer object - CL_MEM_OBJECT_ALLOCATION_FAILURE";
            break;
        case CL_OUT_OF_RESOURCES:
            return "failure to allocate resources required by the OpenCL implementation on the device - CL_OUT_OF_RESOURCES";
            break;
        case CL_OUT_OF_HOST_MEMORY:
            return "failure to allocate resources required by the OpenCL implementation on the host - CL_OUT_OF_HOST_MEMORY";
            break;
        case CL_PROFILING_INFO_NOT_AVAILABLE:
            return "returned by clGetEventProfilingInfo, if the CL_QUEUE_PROFILING_ENABLE flag is not set for the command-queue and if the profiling information is currently not available - CL_PROFILING_INFO_NOT_AVAILABLE";
            break;
        case CL_MEM_COPY_OVERLAP:
            return "if source and destination buffers are the same buffer object and the source and destination regions overlap - CL_MEM_COPY_OVERLAP";
            break;
        case CL_IMAGE_FORMAT_MISMATCH:
            return "src and dst image do not use the same image format<> - CL_IMAGE_FORMAT_MISMATCH";
            break;
        case CL_IMAGE_FORMAT_NOT_SUPPORTED:
            return "the image format is not supported. - CL_IMAGE_FORMAT_NOT_SUPPORTED";
            break;
        case CL_BUILD_PROGRAM_FAILURE:
            return "program build error for given device, Use clGetProgramBuildInfo API call to get the build log of the kernel compilation. - CL_BUILD_PROGRAM_FAILURE";
            break;
        case CL_MAP_FAILURE:
            return "failed to map the requested region into the host address space. This error does not occur for buffer objects created with CL_MEM_USE_HOST_PTR or CL_MEM_ALLOC_HOST_PTR - CL_MAP_FAILURE";
            break;
        case CL_MISALIGNED_SUB_BUFFER_OFFSET:
            return "no devices in given context associated with buffer for which the origin value is aligned to the CL_DEVICE_MEM_BASE_ADDR_ALIGN value - CL_MISALIGNED_SUB_BUFFER_OFFSET";
            break;
        case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
            return "returned by clWaitForEvents(), execution status of any of the events in event list is a negative integer value i.e., error - CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
            break;
        case CL_COMPILE_PROGRAM_FAILURE:
            return "failed to compile the program source. Error occurs if clCompileProgram does not return until the compile has completed - CL_COMPILE_PROGRAM_FAILURE";
            break;
        case CL_LINKER_NOT_AVAILABLE:
            return "Linker unavailable - CL_LINKER_NOT_AVAILABLE";
            break;
        case CL_LINK_PROGRAM_FAILURE:
            return "failed to link the compiled binaries and perhaps libraries - CL_LINK_PROGRAM_FAILURE";
            break;
        case CL_DEVICE_PARTITION_FAILED:
            return "given partition name is supported by the implementation but input device couldn't be partitioned further - CL_DEVICE_PARTITION_FAILED";
            break;
        case CL_KERNEL_ARG_INFO_NOT_AVAILABLE:
            return "argument information is not available for the given kernel - CL_KERNEL_ARG_INFO_NOT_AVAILABLE";
            break;
        case CL_INVALID_VALUE:
            return "values passed in the flags parameter is not valid - CL_INVALID_VALUE";
            break;
        case CL_INVALID_DEVICE_TYPE:
            return "device type specified is not valid, its returned by clCreateContextFromType / clGetDeviceIDs - CL_INVALID_DEVICE_TYPE";
            break;
        case CL_INVALID_PLATFORM:
            return "the specified platform is not a valid platform, its returned by clGetPlatformInfo /clGetDeviceIDs / clCreateContext / clCreateContextFromType - CL_INVALID_PLATFORM";
            break;
        case CL_INVALID_DEVICE:
            return "device/s specified are not valid";
            break;
        case CL_INVALID_CONTEXT:
            return "the given context is invalid OpenCL context, or the context associated with certain parameters are not the same - CL_INVALID_QUEUE_PROPERTIES";
            break;
        case CL_INVALID_QUEUE_PROPERTIES:
            return "specified properties are valid but are not supported by the device, its returned by clCreateCommandQueue / clSetCommandQueueProperty";
            break;
        case CL_INVALID_COMMAND_QUEUE:
            return "the specified command-queue is not a valid command-queue - CL_INVALID_COMMAND_QUEUE";
            break;
        case CL_INVALID_HOST_PTR:
            return "host pointer is NULL and CL_MEM_COPY_HOST_PTR or CL_MEM_USE_HOST_PTR are set in flags or if host_ptr is not NULL but CL_MEM_COPY_HOST_PTR or CL_MEM_USE_HOST_PTR are not set in flags. returned by clCreateBuffer / clCreateImage2D / clCreateImage3D - CL_INVALID_HOST_PTR";
            break;
        case CL_INVALID_MEM_OBJECT:
            return "the passed parameter is not a valid memory, image, or buffer object - CL_INVALID_MEM_OBJECT";
            break;
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
            return "image format specified is not valid or is NULL, clCreateImage2D /clCreateImage3D returns this. - CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
            break;
        case CL_INVALID_IMAGE_SIZE:
            return "Its returned by create Image functions 2D/3D, if specified image width or height are outbound or 0 - CL_INVALID_IMAGE_SIZE";
            break;
        case CL_INVALID_SAMPLER:
            return "specified sampler is an invalid sampler object - CL_INVALID_SAMPLER";
            break;
        case CL_INVALID_BINARY:
            return "program binary is not a valid binary for the specified device, returned by clBuildProgram / clCreateProgramWithBinary - CL_INVALID_BINARY";
            break;
        case CL_INVALID_BUILD_OPTIONS:
            return "the given build options are not valid - CL_INVALID_BUILD_OPTIONS";
            break;
        case CL_INVALID_PROGRAM:
            return "the given program is an invalid program object, returned by clRetainProgram / clReleaseProgram / clBuildProgram / clGetProgramInfo / clGetProgramBuildInfo / clCreateKernel / clCreateKernelsInProgram - CL_INVALID_PROGRAM";
            break;
        case CL_INVALID_PROGRAM_EXECUTABLE:
            return "if there is no successfully built executable for program returned by clCreateKernel, there is no device in program then returned by clCreateKernelsInProgram, if no successfully built program executable present for device associated with command queue then returned by clEnqueueNDRangeKernel / clEnqueueTask - CL_INVALID_PROGRAM_EXECUTABLE";
            break;
        case CL_INVALID_KERNEL_NAME:
            return "mentioned kernel name is not found in program - CL_INVALID_KERNEL_NAME";
            break;
        case CL_INVALID_KERNEL_DEFINITION:
            return "arguments mismatch for the __kernel function definition and the passed ones, returned by clCreateKernel - CL_INVALID_KERNEL_DEFINITION";
            break;
        case CL_INVALID_KERNEL:
            return "specified kernel is an invalid kernel object - CL_INVALID_KERNEL";
            break;
        case CL_INVALID_ARG_INDEX:
            return "clSetKernelArg if an invalid argument index is specified - CL_INVALID_ARG_INDEX";
            break;
        case CL_INVALID_ARG_VALUE:
            return "the argument value specified is NULL, returned by clSetKernelArg - CL_INVALID_ARG_VALUE";
            break;
        case CL_INVALID_ARG_SIZE:
            return "the given argument size (arg_size) do not match size of the data type for an argument, returned by clSetKernelArg - CL_INVALID_ARG_SIZE";
            break;
        case CL_INVALID_KERNEL_ARGS:
            return "the kernel argument values have not been specified, returned by clEnqueueNDRangeKernel / clEnqueueTask - CL_INVALID_KERNEL_ARGS";
            break;
        case CL_INVALID_WORK_DIMENSION:
            return "given work dimension is an invalid value, returned by clEnqueueNDRangeKernel - CL_INVALID_WORK_DIMENSION";
            break;
        case CL_INVALID_WORK_GROUP_SIZE:
            return "the specified local workgroup size and number of workitems specified by global workgroup size is not evenly divisible by local workgroup size - CL_INVALID_WORK_GROUP_SIZE";
            break;
        case CL_INVALID_WORK_ITEM_SIZE:
            return "no. of workitems specified in any of local work group sizes is greater than the corresponding values specified by CL_DEVICE_MAX_WORK_ITEM_SIZES in that particular dimension - CL_INVALID_WORK_ITEM_SIZE";
            break;
        case CL_INVALID_GLOBAL_OFFSET:
            return "global_work_offset is not NULL. Must currently be a NULL value. In a future revision of OpenCL, global_work_offset can be used but not until OCL 1.2 - CL_INVALID_GLOBAL_OFFSET";
            break;
        case CL_INVALID_EVENT_WAIT_LIST:
            return "event wait list is NULL and (no. of events in wait list > 0), or event wait list is not NULL and no. of events in wait list is 0, or specified event objects are not valid events - CL_INVALID_EVENT_WAIT_LIST";
            break;
        case CL_INVALID_EVENT:
            return "invalid event objects specified - CL_INVALID_EVENT";
            break;
        case CL_INVALID_OPERATION:
            return "CL_INVALID_OPERATION";
            break;
        case CL_INVALID_GL_OBJECT:
            return "not a valid GL buffer object - CL_INVALID_GL_OBJECT";
            break;
        case CL_INVALID_BUFFER_SIZE:
            return "the value of the parameter size is 0 or exceeds CL_DEVICE_MAX_MEM_ALLOC_SIZE for all devices specified in the parameter context, returned by clCreateBuffer - CL_INVALID_BUFFER_SIZE";
            break;
        case CL_INVALID_MIP_LEVEL:
            return "CL_INVALID_MIP_LEVEL";
            break;
        case CL_INVALID_GLOBAL_WORK_SIZE:
            return "specified global work size is NULL, or any of the values specified in global work dimensions are 0 or exceeds the range given by the sizeof(size_t) for the device on which the kernel will be enqueued, returned by clEnqueueNDRangeKernel - CL_INVALID_GLOBAL_WORK_SIZE";
            break;
        case CL_INVALID_PROPERTY:
            return "context property name in properties is not a supported property name, returned by clCreateContext - CL_INVALID_PROPERTY";
            break;
        case CL_INVALID_IMAGE_DESCRIPTOR:
            return "values specified in image description are invalid - CL_INVALID_IMAGE_DESCRIPTOR";
            break;
        case CL_INVALID_COMPILER_OPTIONS:
            return "compiler options specified by options are invalid, returned by clCompileProgram - CL_INVALID_COMPILER_OPTIONS";
            break;
        case CL_INVALID_LINKER_OPTIONS:
            return "linker options specified by options are invalid, returned by clLinkProgram - CL_INVALID_LINKER_OPTIONS";
            break;
        case CL_INVALID_DEVICE_PARTITION_COUNT:
            return "CL_INVALID_DEVICE_PARTITION_COUNT";
            break;
        default:
            return "Error code " + std::to_string((int) error_code) + " not found";
    }
}