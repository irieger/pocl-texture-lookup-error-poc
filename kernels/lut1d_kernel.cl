__constant sampler_t sampler_1d = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

__kernel void lut1d_kernel(
    const __global float* input_values,
    __global float* output_values,
    const uint num_entries,
    __read_only image1d_t lut1d,
    const uint lut1d_size
)
{
    uint idx = get_global_id(0);
    if (idx >= num_entries)
        return;

    float input = input_values[idx];
    
    if (lut1d_size > 0)
    {
        input = input > 1 ? 1 : (input < 0 ? 0 : input);
        input = input * (lut1d_size - 1);
        float pos = floor(input);
        float lower = read_imagef(lut1d, sampler_1d, pos).w;
        float upper = read_imagef(lut1d, sampler_1d, pos + 1).w;
        float diff = input - pos;
        
        input = diff * upper + (1 - diff) * lower;
    }

    output_values[idx] = input;
}