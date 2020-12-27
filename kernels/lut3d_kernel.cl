__constant sampler_t sampler_3d = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;


__kernel void lut3d_kernel(
    const __global float* input_values,
    __global float* output_values,
    const uint num_entries,
    __read_only image3d_t lut3d,
    const uint lut3d_size
)
{
    uint idx = get_global_id(0);
    if (idx >= num_entries * num_entries * num_entries)
        return;

    float4 input = vload4(0, &input_values[idx * 4]);
    
    if (lut3d_size > 0)
    {
        input.x = input.x > 1 ? 1 : (input.x < 0 ? 0 : input.x);
        input.x = input.x * (lut3d_size - 1);
        input.y = input.y > 1 ? 1 : (input.y < 0 ? 0 : input.y);
        input.y = input.y * (lut3d_size - 1);
        input.z = input.z > 1 ? 1 : (input.z < 0 ? 0 : input.z);
        input.z = input.z * (lut3d_size - 1);
        float3 pos = (float3) (floor(input.x), floor(input.y), floor(input.z));

        float diff_x = input.x - pos.x;
        float diff_y = input.y - pos.y;
        float diff_z = input.z - pos.z;

        float4 x0y0 = read_imagef(lut3d, sampler_3d, (float4) (pos.x + 0, pos.y + 0, pos.z + 0, 0));
        float4 x1y0 = read_imagef(lut3d, sampler_3d, (float4) (pos.x + 1, pos.y + 0, pos.z + 0, 0));
        float4 x0y1 = read_imagef(lut3d, sampler_3d, (float4) (pos.x + 0, pos.y + 1, pos.z + 0, 0));
        float4 x1y1 = read_imagef(lut3d, sampler_3d, (float4) (pos.x + 1, pos.y + 1, pos.z + 0, 0));

        float4 lower = x0y0 * (1 - diff_x) + x1y0 * diff_x;
        float4 upper = x0y1 * (1 - diff_x) + x1y1 * diff_x;

        x0y0 = read_imagef(lut3d, sampler_3d, (float4) (pos.x + 0, pos.y + 0, pos.z + 1, 0));
        x1y0 = read_imagef(lut3d, sampler_3d, (float4) (pos.x + 1, pos.y + 0, pos.z + 1, 0));
        x0y1 = read_imagef(lut3d, sampler_3d, (float4) (pos.x + 0, pos.y + 1, pos.z + 1, 0));
        x1y1 = read_imagef(lut3d, sampler_3d, (float4) (pos.x + 1, pos.y + 1, pos.z + 1, 0));

        lower = x0y0 * (1 - diff_x) + x1y0 * diff_x;
        upper = x0y1 * (1 - diff_x) + x1y1 * diff_x;

        input = (1 - diff_z) * input + diff_z * (diff_y * upper + (1 - diff_y) * lower);
    }

    vstore4(input, 0, &output_values[idx * 4]);
}