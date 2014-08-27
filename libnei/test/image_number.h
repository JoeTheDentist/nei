#ifndef IMAGE_NUMBER_H
#define IMAGE_NUMBER_H

#include <string>
#include <iostream>

enum image_t
{
    ONE=1, TWO
};

/**
 * @brief The PBM_Image class
 *
 * PBM image
 */
class PBM_Image
{
public:
    /**
     * @brief constructor, reads the file and fills the inner matrix
     * @param file_name : path to the file
     * @complexity O(h * w) (size of the image)
     */
    PBM_Image(const std::string &file_name);

    /**
     * @brief destructor, frees matrix memory
     * @complexity O(h) (height of the image)
     */
    ~PBM_Image();

    /**
     * @brief center
     * @complexity big theta(size of image in pixel)
     *
     * Tries to center image + expand (aiming to normalize the image)
     */
    void center();

    /**
     * @brief distance with given image
     * @param img
     * @return distance
     *
     * Works only with images with same dimention
     */
    float distance(const PBM_Image &img) const;

    /**
     * @brief dump of the image in the stream operator
     * @param os
     * @param img
     * @return
     */
    friend std::ostream& operator<<(std::ostream& os, const PBM_Image& img);

private:
    unsigned int **_inner_matrix;
    unsigned int _h;
    unsigned int _w;
};

class PBM_Distance
{
public:
    float operator()(const PBM_Image &img1, const PBM_Image &img2) { return img1.distance(img2); }
};

#endif // IMAGE_NUMBER_H
