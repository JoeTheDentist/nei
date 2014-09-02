
#include <fstream>
#include <sstream>
#include <gtest/gtest.h>

#include "../include/nei.h"

#include "common.h"
#include "image_number.h"

PBM_Image::PBM_Image(const std::string &file_name)
{
    std::ifstream in_file(file_name.c_str());
    std::string line;
    // get P1
    std::getline( in_file, line );
    // get comment (temp, works only for one comment, e.g. generated with GIMP)
    std::getline( in_file, line );
    // get size of the image
    in_file >> _w >> _h;

    // initialize matrix
    _inner_matrix = new unsigned int*[_h];
    for (unsigned int i=0; i<_h; ++i)
    {
        _inner_matrix[i] = new unsigned int[_w];
    }

    unsigned int count = 0;
    while (in_file.good())
    {
        char c = in_file.get();
        if (c == '1' || c == '0')
        {
            _inner_matrix[count/_w][count%_w] = c - '0';
            ++count;
        }
    }
}

PBM_Image::~PBM_Image()
{
    for (unsigned int i=0; i<_h; ++i)
    {
        delete[] _inner_matrix[i];
    }
    delete[] _inner_matrix;
}

float PBM_Image::distance(const PBM_Image &img) const
{
    float dist = 0;
    for (unsigned int i=0; i<_h; ++i)
    {
        for (unsigned int j=0; j<_w; ++j)
        {
            if (img._inner_matrix[i][j] != _inner_matrix[i][j])
            {
                ++dist;
            }
        }
    }
    return dist;
}

std::ostream& operator<<(std::ostream& os, const PBM_Image& img)
{
    for (unsigned int row = 0; row<img._h; ++row)
    {
        for (unsigned int col = 0; col<img._w; ++col)
        {
            os << img._inner_matrix[row][col] << " ";
        }
        os << std::endl;
    }
    return os;
}

TEST(PBM_Image, ReadImage)
{
    PBM_Image img(TestGlobals::TestDataDirectory + "test_16x16.pbm");
    std::stringstream ss;
    ss << img;
    EXPECT_EQ(ss.str().length(), 528);
}

TEST(PBM_Image, kNN_SelfClassification)
{
    nei::kNN<PBM_Image, image_t, PBM_Distance> classifier;
    for (unsigned int number = 1; number <= 2; ++number)
    {
        for (unsigned int i=0; i<10; ++i)
        {
            std::stringstream file_name;
            file_name << TestGlobals::TestTrainingDirectory << number << "_" << i << ".pbm";
            classifier.add_training_point(std::unique_ptr<PBM_Image>(new PBM_Image(file_name.str())), static_cast<image_t>(number));
        }
    }
    for (unsigned int number = 1; number <= 2; ++number)
    {
        for (unsigned int i=0; i<10; ++i)
        {
            std::stringstream file_name;
            file_name << TestGlobals::TestTrainingDirectory << number << "_" << i << ".pbm";
            EXPECT_EQ(classifier.classify(PBM_Image(file_name.str()), 3), static_cast<image_t>(number));
        }
    }
}
