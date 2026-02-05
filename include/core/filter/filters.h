#pragma once
#include <cstdint>
#include <vector>
#include <optional>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include <cmath>

#include "core/global.h"

namespace imlib {
enum FilterType {
    MEAN = 0,
    MEDIAN,
    GAUSSIAN,
    LAPLACIAN
};
struct FilterDefinition {
    FilterType type;
    int size;
    bool operator==(FilterDefinition const& other) const noexcept {
        return type == other.type && size == other.size;
    }
};

class Filters {
public:
    static std::vector<float> GetSpacialFilterKernel(FilterType type, int size);
    static void ApplyFilter(
        FilterType type, int size,
        std::vector<uint8_t>& source,
        std::vector<uint8_t>& destination,
        int num_channels,
        int width, int height
    );
private:
    static void apply_filter_CUDA(
        FilterType type, int size,
        std::vector<uint8_t>& source,
        std::vector<uint8_t>& destination,
        int num_channels,
        int width, int height
    );
    static bool cuda_available();
    static std::unordered_map<FilterDefinition, std::vector<float>> saved_filters_;
};
}