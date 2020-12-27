#pragma once

#include <cstdint>
#include <vector>
#include "rgba.h"

const inline uint32_t lut_2d_size = 2;
const inline std::vector<RgbaData> lut_2d_data = {
    {0.1, 0.05, 0.05, 1},
    {0.3, 0.75, 0.05, 1},
    {0.1, 0.55, 0.35, 1},
    {1.1, 0.25, 1.00, 1}
};