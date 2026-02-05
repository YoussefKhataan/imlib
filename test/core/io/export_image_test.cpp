#include <string>
#include <utility>
#include <vector>
#include <filesystem>

#include <gtest/gtest.h>

#include "core/data/image.h"
#include "core/io/image_loader.h"
#include "core/filter/filters.h"
#include "core/global.h"

using namespace imlib::core::data;
using namespace imlib::core::io;
 
namespace imlib {

TEST(ExportImageTest, CPU_Blur1000x1000) {
    GlobalConfig::turn_gpu_off();
    std::vector<uint8_t> data(1000*1000*3);
    for (int y = 0; y < 1000; y++) {
        for (int x = 0; x < 1000; x++) {
            int index = (y * 1000 + x) * 3;

            int square_size = 20;
            if ( ((y / 20) + (x / 20)) % 2 == 0 ) {
                data[index] = 255;
                data[index + 1] = 255;
                data[index + 2] = 255;
            }
            else {
                data[index] = 0;
                data[index + 1] = 0;
                data[index + 2] = 0;
            }
        }
    }
    RGB_Image image2(1000, 1000, std::move(data));

    auto start = std::chrono::steady_clock::now();
    std::unique_ptr<Image> image3 = image2.ApplyFilter(imlib::FilterType::GAUSSIAN, 21);
    auto end  = std::chrono::steady_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    printf("cpu time: %i\n", time);

    export_image(std::move(image3), "checker", ImageFormat::PNG);
}

TEST(ExportImageTest, GPU_Blur1000x1000) {
    GlobalConfig::turn_gpu_on();
    std::vector<uint8_t> data(1000*1000*3);
    for (int y = 0; y < 1000; y++) {
        for (int x = 0; x < 1000; x++) {
            int index = (y * 1000 + x) * 3;

            int square_size = 20;
            if ( ((y / 20) + (x / 20)) % 2 == 0 ) {
                data[index] = 255;
                data[index + 1] = 255;
                data[index + 2] = 255;
            }
            else {
                data[index] = 0;
                data[index + 1] = 0;
                data[index + 2] = 0;
            }
        }
    }
    RGB_Image image2(1000, 1000, std::move(data));

    auto start = std::chrono::steady_clock::now();
    std::unique_ptr<Image> image3 = image2.ApplyFilter(imlib::FilterType::GAUSSIAN, 21);
    auto end  = std::chrono::steady_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    printf("gpu time: %i\n", time);

    export_image(std::move(image3), "checker", ImageFormat::PNG);
}
}
