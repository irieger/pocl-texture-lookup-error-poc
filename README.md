# This is just a small demo of OpenCL lut application

This demo is used to analyse a bug in POCL which results in OpenCL error -59 (CL_INVALID_FUNCTION).

## Short description

As I already reported in the issue https://github.com/pocl/pocl/issues/864 I have a problem with LUT lookups implemented as OpenCL texture lookups when trying to run my code with POCL as my OpenCL device/platform. My target is to run some image processing kernels on a Raspberry Pi for continous integration so I needed OpenCL execution on the CPU.

**Note:** Currently I haven't tested the code in this state on the Raspberry but will follow up with it later on. Tested on AMD Ryzen running Ubuntu 20.04 with POCL 1.6 running in the problem I first experienced on the Raspberry Pi.