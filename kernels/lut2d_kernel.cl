__constant sampler_t sampler_2d = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;


__kernel void lut2d_kernel(
    const __global float* input_values,
    __global float* output_values,
    const uint num_entries,
    __read_only image2d_t lut2d,
    const uint lut2d_size
)
{
    uint idx = get_global_id(0);
    if (idx >= num_entries * num_entries)
        return;

    float4 input = vload4(0, &input_values[idx * 4]);
    
    if (lut2d_size > 0)
    {
        input.x = input.x > 1 ? 1 : (input.x < 0 ? 0 : input.x);
        input.x = input.x * (lut2d_size - 1);
        input.y = input.y > 1 ? 1 : (input.y < 0 ? 0 : input.y);
        input.y = input.y * (lut2d_size - 1);
        int2 pos = (int2) (floor(input.x), floor(input.y));

        float diff_x = input.x - pos.x;
        float diff_y = input.y - pos.y;

        float4 x0y0 = read_imagef(lut2d, sampler_2d, (int2) (pos.x + 0, pos.y + 0));
        float4 x1y0 = read_imagef(lut2d, sampler_2d, (int2) (pos.x + 1, pos.y + 0));
        float4 x0y1 = read_imagef(lut2d, sampler_2d, (int2) (pos.x + 0, pos.y + 1));
        float4 x1y1 = read_imagef(lut2d, sampler_2d, (int2) (pos.x + 1, pos.y + 1));

        float4 lower = x0y0 * (1 - diff_x) + x1y0 * diff_x;
        float4 upper = x0y1 * (1 - diff_x) + x1y1 * diff_x;
        input = diff_y * upper + (1 - diff_y) * lower;
    }

    vstore4(input, 0, &output_values[idx * 4]);
}