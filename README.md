# This is just a small demo of OpenCL lut application

This demo is used to analyse a bug in POCL which results in OpenCL error -59 (CL_INVALID_FUNCTION).

## Short description

As I already reported in the issue https://github.com/pocl/pocl/issues/864 I have a problem with LUT lookups implemented as OpenCL texture lookups when trying to run my code with POCL as my OpenCL device/platform. My target is to run some image processing kernels on a Raspberry Pi for continous integration so I needed OpenCL execution on the CPU.

**Note:** Currently I haven't tested the code in this state on the Raspberry but will follow up with it later on.


## Building & Running

Just running `make` should be sufficient as long as OpenCL is in the path.

**Warning:** No fancy device selection is implemented. If POCL isn't the first platform it might not be selected.

### Running

Running this tool is done by just using

`./demo MODE`

With mode being one of:

0. Basic OpenCL test
1. 1D Lut (cl::Image1D) texture
1. 2D Lut (cl::Image2D) texture
1. 3D Lut (cl::Image2D) texture

`POCL_DEBUG=1 ./demo MODE` can be used to get the POCL debug output.

## Where did I run this

**Working (nivida OpenCL)**: On my main workstation, Ubuntu 20.04 on my Ryzen Threadripper 2950X with GeForce 1080 Ti the code works well with the CUDA OpenCL driver.

**Not working (POCL)**: Ubuntu 20.04 clean install with only POCL 1.6 installed from the release tarball.


## Note on the used LUT data (test_lut_*d.h)

This are just randomly crafted by hand without a proper meaning for now, as the error occurs anyway.
