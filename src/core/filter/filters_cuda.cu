#include <cuda_runtime.h>
#include "core/filter/filters.h"

namespace imlib{

bool Filters::cuda_available() {
    int deviceCount = 0;
    cudaError_t error = cudaGetDeviceCount(&deviceCount);
    return (error == cudaSuccess && deviceCount > 0);
}
__global__ void filter_kernel(
    uint8_t* source, uint8_t* destination,
    int width, int height,
    const int num_channels,
    int num_color_channels,
    int kernel_size,
    float* kernel
){

    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    
    if (x >= width || y >= height) {
        return;
    }

    int half = kernel_size / 2;
    int baseIdx = (y * width + x) * num_channels;
    
    float channel_vals[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    //memset(channel_vals, 0.0f, num_color_channels);

    for (int ky = -half; ky <= half; ky++) {
        for (int kx = -half; kx <= half; kx++) {
            int ny = y + ky;
            int nx = x + kx;
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
            
                int neighbor_idx = (ny * width + nx) * num_channels;
                float weight = kernel[(ky + half) * kernel_size + (kx + half)];

                // Only iterate over color channels
                for (int c = 0; c < num_color_channels; c++) {
                    channel_vals[c] += static_cast<float>(source[neighbor_idx + c]) * weight;
                }
            }
        }
    }

    // write results
    for (int c = 0; c < num_color_channels; c++) {
        float val = channel_vals[c];
        val = (val > 255.0f) ? 255.0f : (val < 0.0f ? 0.0f : val);
        destination[baseIdx + c] = static_cast<uint8_t>(val);
    }
    if (num_channels > num_color_channels) {
        destination[baseIdx + num_color_channels] = source[baseIdx + num_color_channels];
    }
    
}
void Filters::apply_filter_CUDA(
    FilterType type, int kernel_size,
    std::vector<uint8_t>& source,
    std::vector<uint8_t>& destination,
    const int num_channels,
    int width, int height
) {
    if (kernel_size % 2 == 0) {
        kernel_size ++;
    }

    const int num_alpha_channels = (num_channels % 2 == 0) ? 1 : 0;
    const int num_color_channels = num_channels -  num_alpha_channels;
    
    std::vector<float> h_kernel = Filters::GetSpacialFilterKernel(type, kernel_size);

    size_t img_bytes = width * height * num_channels * sizeof(uint8_t);
    size_t kernel_bytes = kernel_size * kernel_size * sizeof(float);

    uint8_t *d_src, *d_dest;
    float *d_kernel;

    cudaMalloc(&d_src, img_bytes);
    cudaMalloc(&d_dest, img_bytes);
    cudaMalloc(&d_kernel, kernel_bytes); 

    cudaMemcpy(d_src, source.data(), img_bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_kernel, h_kernel.data(), kernel_bytes, cudaMemcpyHostToDevice);

    dim3 blockSize(16, 16);
    // Formula: (N + block_dim - 1) / block_dim
    dim3 gridSize((width + 15) / 16, (height + 15) / 16);

    filter_kernel<<<gridSize, blockSize>>>(
        d_src, d_dest,
        width, height,
        num_channels, num_color_channels,
        kernel_size, d_kernel 
    );

    cudaDeviceSynchronize();

    //if (destination.size() != source.size()) destination.resize(source.size());
    
    cudaMemcpy(destination.data(), d_dest, img_bytes, cudaMemcpyDeviceToHost);

    cudaFree(d_src);
    cudaFree(d_dest);
    cudaFree(d_kernel);
}
}