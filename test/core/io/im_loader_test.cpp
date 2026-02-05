#include <string>
#include <utility>
#include <vector>
#include<filesystem>

#include <gtest/gtest.h>

#include "core/data/image.h"
#include "core/io/image_loader.h"

namespace imlib::test {

using namespace imlib::core::data;
using namespace imlib::core::io;


TEST(Image_Loader, Load_RGB) {
    std::unique_ptr<Image> img;
    bool ok = load_image("test_data/1x1_rgb.jpg", img);

    ASSERT_EQ(ok, true);
    
    ASSERT_EQ(img->Channels(), 3); 
    
    auto* rgb = dynamic_cast<RGB_Image*>(img.get());
    ASSERT_NE(rgb, nullptr);

    ASSERT_EQ(img->MetaData().width, 1);
    ASSERT_EQ(img->MetaData().height, 1);
    ASSERT_EQ(img->MetaData().raw_buffer_size, 3 * 8);

    ASSERT_EQ(img->Data()[0], 255);
    ASSERT_EQ(img->Data()[1], 255);
    ASSERT_EQ(img->Data()[2], 255);
}


TEST(Image_Loader, Load_RGBA) {
    std::unique_ptr<Image> img;
    bool ok = load_image("test_data/1x1_rgba.png", img);

    ASSERT_EQ(ok, true);
    
    ASSERT_EQ(img->Channels(), 4); 
    
    auto* rgb = dynamic_cast<RGBA_Image*>(img.get());
    ASSERT_NE(rgb, nullptr);

    ASSERT_EQ(img->MetaData().width, 1);
    ASSERT_EQ(img->MetaData().height, 1);
    ASSERT_EQ(img->MetaData().raw_buffer_size, 4 * 8);

    ASSERT_EQ(img->Data()[0], 255);
    ASSERT_EQ(img->Data()[1], 255);
    ASSERT_EQ(img->Data()[2], 255);
    ASSERT_EQ(img->Data()[3], 128);
}

TEST(Image_Loader, Load_BW) {
    std::unique_ptr<Image> img;
    // image with alternating b/w pixels
    bool ok = load_image("test_data/4x4_bw.jpg", img);

    ASSERT_EQ(ok, true);
    
    ASSERT_EQ(img->Channels(), 1); 
    
    auto* rgb = dynamic_cast<BW_Image*>(img.get());
    ASSERT_NE(rgb, nullptr);

    ASSERT_EQ(img->MetaData().width, 4);
    ASSERT_EQ(img->MetaData().height, 4);
    ASSERT_EQ(img->MetaData().raw_buffer_size, 4 * 4 * 8);

    for (int i = 0; i < 16; i++)
    {
        if (i % 2 == 0) {
            ASSERT_EQ(img->Data()[i], 0);
        } else {
            ASSERT_EQ(img->Data()[i], 255);
        }
    }
}

TEST(Image_Loader, Load_BWA) {
    std::unique_ptr<Image> img;
    // image with alternating b/w pixels
    bool ok = load_image("test_data/4x4_bwa.png", img);

    ASSERT_EQ(ok, true);
    
    ASSERT_EQ(img->Channels(), 2); 
    
    auto* rgb = dynamic_cast<BWA_Image*>(img.get());
    ASSERT_NE(rgb, nullptr);

    ASSERT_EQ(img->MetaData().width, 4);
    ASSERT_EQ(img->MetaData().height, 4);
    ASSERT_EQ(img->MetaData().raw_buffer_size, 4 * 4 * 2 * 8);

    for (int i = 0, j = 0; i < 32; i+=2, j += 1)
    {
        // alpha
        ASSERT_EQ(img->Data()[i + 1], 128);

        // grey
        if (j % 2 == 0) {
            ASSERT_EQ(img->Data()[i], 0);
        } else {
            ASSERT_EQ(img->Data()[i], 255);
        }
    }
}

// TEST(Image_Loader, ReinterpretPixels) {
//     std::unique_ptr<Image> img;
//     bool ok = load_image("test_data/100x100_rgba.png", img);

//     ASSERT_EQ(ok, true);
    
//     ASSERT_EQ(img->Channels(), 4); 
    
//     auto* rgb = dynamic_cast<RGBA_Image*>(img.get());
//     ASSERT_NE(rgb, nullptr);

//     auto pixels = rgb->pixels();
//     auto data = rgb->Data();
//     for (int i = 0, j = 0; i < img->Data().size(); i+= 4, j++) {
//         ASSERT_EQ(data[i], pixels[j].r);
//         ASSERT_EQ(data[i + 1], pixels[j].g);
//         ASSERT_EQ(data[i + 2], pixels[j].b);
//         ASSERT_EQ(data[i + 3], pixels[j].alpha);
//     }
    

//     // ASSERT_EQ(img->MetaData().width, 1);
//     // ASSERT_EQ(img->MetaData().height, 1);
//     // ASSERT_EQ(img->MetaData().raw_buffer_size, 4 * 8);

//     // ASSERT_EQ(img->Data()[0], 255);
//     // ASSERT_EQ(img->Data()[1], 255);
//     // ASSERT_EQ(img->Data()[2], 255);
//     // ASSERT_EQ(img->Data()[3], 128);
// }

}



