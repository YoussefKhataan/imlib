#include "core/filter/filters.h"

namespace imlib {
std::vector<float> Filters::GetSpacialFilterKernel(FilterType type, int size) {
    // auto it = saved_filters_.find({FilterType::MEAN, size});
    // if (it != saved_filters_.end()) {
    //     return it->second;
    // }

    std::vector<float> kernel(size * size);
    switch (type)
    {
        case FilterType::MEAN: {
            std::fill(kernel.begin(), kernel.end(), 1.0f / static_cast<float>(size * size));
            //saved_filters_[{FilterType::MEAN, size}] = kernel;
            return kernel;
        }
        case FilterType::GAUSSIAN: {
            int half = size / 2;
            float sigma = (float)size / 6;
            float twoSigmaSq = 2 * sigma * sigma;
            float sum = 0.0f;

            for (int i = -half; i <= half; i++) {
                for (int j = -half; j <= half; j++) {
                    float value = expf(-(i*i + j*j) / twoSigmaSq);
                    kernel[size * (i + half) + (j + half)] = value;
                    sum += value;
                }
            }
            // normalization
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    kernel[i * size + j] /= sum;
                }
            }

            return kernel;
        }
    }
}

void Filters::ApplyFilter(
    FilterType type, int size,
    std::vector<uint8_t>& source,
    std::vector<uint8_t>& destination,
    int num_channels,
    int width,
    int height
) {
    if (GlobalConfig::is_gpu_on() && cuda_available()) {
        apply_filter_CUDA(type, size, source, destination, num_channels, width, height);
        return;
    }
    if (size % 2 == 0) {
        size ++;
    }
    const int half = size / 2;

    const int num_alpha_channels = (num_channels % 2 == 0) ? 1 : 0;
    const int num_color_channels = num_channels -  num_alpha_channels;
    
    // auto source_data = Data();
    // std::vector<uint8_t> destination(width * height * num_channels);
    std::vector<float> kernel = Filters::GetSpacialFilterKernel(type, size);

    // buffer for storing the per-channel calculate value for every iteration
    std::vector<float> channel_vals(num_color_channels);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            std::fill(channel_vals.begin(), channel_vals.end(), 0);

            // applyKernelAt(x, y, size, kernel, num_channels, num_color_channels, channel_vals, source, destination);
            for (int ky = -half; ky <= half; ky++) {
                for (int kx = -half; kx <= half; kx++) {
                    int ny = y + ky;
                    int nx = x + kx;
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                    
                        int neighbor_idx = (ny * width + nx) * num_channels;
                        float weight = kernel[(ky + half) * size + (kx + half)];

                        // Only iterate over color channels
                        for (int c = 0; c < num_color_channels; c++) {
                            channel_vals[c] += static_cast<float>(source[neighbor_idx + c]) * weight;
                        }
                    }
                }
            }

            // write result
            int center_idx = (y * width + x) * num_channels;
            for (int c = 0; c < num_color_channels; c++) {
                destination[center_idx + c] = static_cast<uint8_t>(std::clamp(channel_vals[c], 0.0f, 255.0f));
            }

            // copy alpha channel
            if (num_alpha_channels) {
                destination[center_idx + num_color_channels] = source[center_idx + num_color_channels];
            }
        }
    }
}
}