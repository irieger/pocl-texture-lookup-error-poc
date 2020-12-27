#pragma once

#include <filesystem>


inline std::string loadKernel(const std::string& kernel_name)
{
    std::filesystem::path fname = "kernels/" + kernel_name + ".cl";

    return "";
}