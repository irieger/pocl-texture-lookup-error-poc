__kernel void non_lut_kernel(
    const __global float* input_values,
    __global float* output_values,
    const uint num_entries
)
{
    uint idx = get_global_id(0);
    if (idx >= num_entries)
        return;

    output_values[idx] = sin(input_values[idx]);
}