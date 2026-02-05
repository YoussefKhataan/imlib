#pragma once
#include <cstdint>
#include <vector>
#include <optional>
#include <functional>
#include <memory>

#include "core/filter/filters.h"

namespace imlib::core::data {

typedef std::uint16_t imdim;
typedef std::uint8_t pixel;
// typedef char pixel;

// Enforce byte alignment so the compiler doesn't add gaps
#pragma pack(push, 1) 
struct RGBA_Pixel { uint8_t r, g, b, alpha; };
struct RGB_Pixel  { uint8_t r, g, b; };
struct BWA_Pixel  { uint8_t gray, alpha; };
struct BW_Pixel   { uint8_t gray; };
#pragma pack(pop)

class RGB_Image;
class BWA_Image;
class BW_Image;
class Binary_Image;

struct ImageMetadata {
    imdim width;
    imdim height;
    size_t raw_buffer_size;
};

inline uint8_t rgb2gray(uint8_t r, uint8_t g, uint8_t b) {
    float ans = (0.299f * r) + (0.587f * g) + (0.114f * b);
    return static_cast<uint8_t>(ans);
}
/*
    Object for the logical representation of an image inside the library, on
    which image operations are performed
*/
class Image
{
protected: 
    imdim width_;
    imdim height_;

    /*
    layout for RGBA:
    [R G B A][R G B A]...[R G B A] 

    indexing (C=num_channels):
    idx = (y * W + x) * C + channel
    */
    std::vector<uint8_t> data_; 

    Image(imdim w, imdim h, std::vector<uint8_t> data)
        : width_(w), height_(h), data_(std::move(data)) {}
    
    // apply_kernel(x, y, kernel_size, kernel, num_channels, channel_vals, source_data)
    // inline void applyKernelAt(
    //     int x, int y,
    //     int kernel_size, std::vector<float>& kernel,
    //     int num_channels, int num_color_channels, std::vector<float>& channel_vals
    // );
public:
    virtual ~Image() = default;
    virtual int Channels() const noexcept = 0;
    ImageMetadata MetaData() const noexcept {
        return {width_, height_, data_.size() * 8};
    }
    // std::optional<Image> ApplyFilter();
    // std::optional<Image> ApplyPixelOperaton();

    /* only used for testing*/
    std::vector<uint8_t> Data() { return data_;};

    /**
     * @brief Applies an operation on the image
     * @return if inplace returns self for chaining, else returns modified copy
    */ // NOTE: Implement based on value of Channels() 
    std::optional<Image> ApplyPixelOperation(std::function<RGBA_Pixel(RGBA_Pixel)> kernel, bool inplace);
    std::unique_ptr<Image> ApplyFilter(FilterType type, int size);

};

// Image with four channels: R, G, B and alzpha
class RGBA_Image : public Image {
public:
    RGBA_Image(imdim w, imdim h, std::vector<uint8_t> d)
        : Image(w, h, std::move(d)) {}
    ~RGBA_Image() = default;

    RGB_Image ToRGB() const;
    BWA_Image ToBWA() const;
    BW_Image ToBW() const;
    int Channels() const noexcept override {return 4;};

private:
    RGBA_Pixel* pixels() {
        return reinterpret_cast<RGBA_Pixel*>(data_.data());
    }
};

class RGB_Image : public Image {
public:
    RGB_Image(imdim width, imdim height, std::vector<uint8_t> data)
        : Image(width, height, std::move(data)) {}
    ~RGB_Image() = default;
    RGBA_Image ToRGBA() const;
    BWA_Image ToBWA() const;
    BW_Image ToBW() const;
    int Channels() const noexcept override {return 3;};

private:
    RGB_Pixel* pixels() {
        return reinterpret_cast<RGB_Pixel*>(data_.data());
    }
};

// image with two channels: Grey and alpha
class BWA_Image : public Image {
public:
    BWA_Image(imdim width, imdim height, std::vector<uint8_t> data)
        : Image(width, height, std::move(data)) {}
    ~BWA_Image() = default;
    RGBA_Image ToRGBA() const;
    RGB_Image ToRGB() const;
    BW_Image ToBW() const;
    int Channels() const noexcept override {return 2;};

private:
    BWA_Pixel* pixels() {
        return reinterpret_cast<BWA_Pixel*>(data_.data());
    }
};

class BW_Image : public Image {
public:
    BW_Image(imdim width, imdim height, std::vector<uint8_t> data)
        : Image(width, height, std::move(data)) {}
    ~BW_Image() = default;
    RGBA_Image ToRGBA() const;
    RGB_Image ToRGB() const;
    BWA_Image ToBWA() const;
    int Channels() const noexcept override {return 1;};

private:
    BW_Pixel* pixels() {
        return reinterpret_cast<BW_Pixel*>(data_.data());
    }
};

// Image with only two possible pixel values: 0 or 1
class Binary_Image : public Image {
public:
    // Implement with bitset instead of vector??
    Binary_Image(imdim width, imdim height, std::vector<uint8_t> data)
        : Image(width, height, std::move(data)) {}
    ~Binary_Image() = default;
    int Channels() const noexcept override {return 1;};
};

}

