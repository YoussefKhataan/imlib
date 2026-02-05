#pragma once
#include<string>
#include<memory>
#include<vector>
#include<iostream>

#include "core/data/image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "third_party/stb_image_write.h"

using namespace imlib::core::data;
namespace imlib::core::io {

enum ImageFormat {
    PNG = 0,
    JPEG,
    BMP
};

bool load_image(const std::string& path, std::unique_ptr<Image>& result) {
    int width, height, channels;

    unsigned char *loaded_data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (loaded_data == NULL) {
        printf("LOADING IMAGE %s FAILED, %s\n", path.c_str(), stbi_failure_reason());
        return false;
    }

    size_t size = width * height * channels;
    std::vector<pixel> data(loaded_data, loaded_data + size); 

    stbi_image_free(loaded_data);
    
    if (channels == 4) {
        result = std::make_unique<RGBA_Image>(width, height, std::move(data));
    }
    else if (channels == 3) {
        result = std::make_unique<RGB_Image>(width, height, std::move(data));
    }
    else if (channels == 2) {
        result = std::make_unique<BWA_Image>(width, height, std::move(data));
    }
    else {
        result = std::make_unique<BW_Image>(width, height, std::move(data));
    }

    return true;
}

bool export_image(std::unique_ptr<Image> image, std::string name, ImageFormat format) {

    int w = image->MetaData().width; 
    int h = image->MetaData().height;
    int c = image->Channels();

    if (format == ImageFormat::PNG) {
        stbi_write_png((name + ".png").c_str(), w, h, c, image->Data().data(), w * c);
    }
    else if(format == ImageFormat::JPEG) {
        stbi_write_jpg((name + ".jpg").c_str(), w, h, c, image->Data().data(), 70); // quality 1-100
    }
    else if (format == ImageFormat::BMP) {
        stbi_write_bmp((name + ".bmp").c_str(), w, h, c, image->Data().data());
    } 
    else {
        return false;
    }

    return true;
}
}