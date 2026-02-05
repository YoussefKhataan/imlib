#include "core/data/image.h"
#include <vector>

namespace imlib::core::data
{
    // ==========================================
    // Spacial Filters
    // ==========================================

    std::unique_ptr<Image> Image::ApplyFilter(FilterType type, int size) {
        const int num_channels = Channels();

        std::vector<uint8_t> result_data(width_ * height_ * num_channels);
        Filters::ApplyFilter(type, size, data_, result_data, num_channels, width_, height_);

        if (num_channels == 1) {
            return std::make_unique<BW_Image>(width_, height_, std::move(result_data));
        }
        else if (num_channels == 2) {
            return std::make_unique<BWA_Image>(width_, height_, std::move(result_data));
        }
        else if (num_channels == 3) {
            return std::make_unique<RGB_Image>(width_, height_, std::move(result_data));
        }
        else {
            return std::make_unique<RGBA_Image>(width_, height_, std::move(result_data));
        }
    }


    // ==========================================
    // Conversions from RGBA
    // ==========================================

    RGB_Image RGBA_Image::ToRGB() const
    {
        size_t num_pixels = width_ * height_;
        std::vector<uint8_t> other_data(num_pixels * 3);

        for (size_t i = 0; i < num_pixels; ++i)
        {
            size_t src = i * 4;
            size_t dst = i * 3;

            other_data[dst] = data_[src];         
            other_data[dst + 1] = data_[src + 1]; 
            other_data[dst + 2] = data_[src + 2]; 
        }
        return RGB_Image(width_, height_, std::move(other_data));
    }

    BWA_Image RGBA_Image::ToBWA() const
    {
        size_t num_pixels = width_ * height_;
        std::vector<uint8_t> other_data(num_pixels * 2);

        for (size_t i = 0; i < num_pixels; ++i)
        {
            size_t src = i * 4;
            size_t dst = i * 2;

            other_data[dst] = rgb2gray(data_[src], data_[src + 1], data_[src + 2]);
            other_data[dst + 1] = data_[src + 3];
        }
        return BWA_Image(width_, height_, std::move(other_data));
    }

    BW_Image RGBA_Image::ToBW() const
    {
        size_t num_pixels = width_ * height_;
        std::vector<uint8_t> other_data(num_pixels); 

        for (size_t i = 0; i < num_pixels; ++i)
        {
            size_t src = i * 4;
            other_data[i] = rgb2gray(data_[src], data_[src + 1], data_[src + 2]);
        }
        return BW_Image(width_, height_, std::move(other_data));
    }

    // ==========================================
    // Conversions from RGB 
    // ==========================================

    RGBA_Image RGB_Image::ToRGBA() const
    {
        size_t num_pixels = width_ * height_;
        std::vector<uint8_t> other_data(num_pixels * 4);

        for (size_t i = 0; i < num_pixels; ++i)
        {
            size_t src = i * 3;
            size_t dst = i * 4;

            other_data[dst] = data_[src];                  
            other_data[dst + 1] = data_[src + 1];          
            other_data[dst + 2] = data_[src + 2];          
            other_data[dst + 3] = static_cast<uint8_t>(255); 
        }
        return RGBA_Image(width_, height_, std::move(other_data));
    }

    BWA_Image RGB_Image::ToBWA() const
    {
        size_t num_pixels = width_ * height_;
        std::vector<uint8_t> other_data(num_pixels * 2);

        for (size_t i = 0; i < num_pixels; ++i)
        {
            size_t src = i * 3;
            size_t dst = i * 2;

            other_data[dst] = rgb2gray(data_[src], data_[src + 1], data_[src + 2]);
            other_data[dst + 1] = static_cast<uint8_t>(255); 
        }
        return BWA_Image(width_, height_, std::move(other_data));
    }

    BW_Image RGB_Image::ToBW() const
    {
        size_t num_pixels = width_ * height_;
        std::vector<uint8_t> other_data(num_pixels);

        for (size_t i = 0; i < num_pixels; ++i)
        {
            size_t src = i * 3;
            other_data[i] = rgb2gray(data_[src], data_[src + 1], data_[src + 2]);
        }
        return BW_Image(width_, height_, std::move(other_data));
    }

    // ==========================================
    // Conversions from BWA 
    // ==========================================

    RGBA_Image BWA_Image::ToRGBA() const
    {
        size_t num_pixels = width_ * height_;
        std::vector<uint8_t> other_data(num_pixels * 4);

        for (size_t i = 0; i < num_pixels; ++i)
        {
            size_t src = i * 2;
            size_t dst = i * 4;

            uint8_t gray_val = data_[src];
            uint8_t alpha_val = data_[src + 1];

            other_data[dst] = gray_val;      
            other_data[dst + 1] = gray_val;  
            other_data[dst + 2] = gray_val;  
            other_data[dst + 3] = alpha_val; 
        }
        return RGBA_Image(width_, height_, std::move(other_data));
    }

    RGB_Image BWA_Image::ToRGB() const
    {
        size_t num_pixels = width_ * height_;
        std::vector<uint8_t> other_data(num_pixels * 3);

        for (size_t i = 0; i < num_pixels; ++i)
        {
            size_t src = i * 2;
            size_t dst = i * 3;

            uint8_t gray_val = data_[src];

            other_data[dst] = gray_val;
            other_data[dst + 1] = gray_val;
            other_data[dst + 2] = gray_val;
        }
        return RGB_Image(width_, height_, std::move(other_data));
    }

    BW_Image BWA_Image::ToBW() const
    {
        size_t num_pixels = width_ * height_;
        std::vector<uint8_t> other_data(num_pixels);

        for (size_t i = 0; i < num_pixels; ++i)
        {
            other_data[i] = data_[i * 2];
        }
        return BW_Image(width_, height_, std::move(other_data));
    }

    // ==========================================
    // Conversions from BW 
    // ==========================================

    RGBA_Image BW_Image::ToRGBA() const
    {
        size_t num_pixels = width_ * height_;
        std::vector<uint8_t> other_data(num_pixels * 4);

        for (size_t i = 0; i < num_pixels; ++i)
        {
            size_t dst = i * 4;
            uint8_t gray_val = data_[i];

            other_data[dst] = gray_val;
            other_data[dst + 1] = gray_val;
            other_data[dst + 2] = gray_val;
            other_data[dst + 3] = static_cast<uint8_t>(255); 
        }
        return RGBA_Image(width_, height_, std::move(other_data));
    }

    RGB_Image BW_Image::ToRGB() const
    {
        size_t num_pixels = width_ * height_;
        std::vector<uint8_t> other_data(num_pixels * 3);

        for (size_t i = 0; i < num_pixels; ++i)
        {
            size_t dst = i * 3;
            uint8_t gray_val = data_[i];

            other_data[dst] = gray_val;
            other_data[dst + 1] = gray_val;
            other_data[dst + 2] = gray_val;
        }
        return RGB_Image(width_, height_, std::move(other_data));
    }

    BWA_Image BW_Image::ToBWA() const
    {
        size_t num_pixels = width_ * height_;
        std::vector<uint8_t> other_data(num_pixels * 2);

        for (size_t i = 0; i < num_pixels; ++i)
        {
            size_t dst = i * 2;

            other_data[dst] = data_[i];                    
            other_data[dst + 1] = 255; 
        }
        return BWA_Image(width_, height_, std::move(other_data));
    }
}