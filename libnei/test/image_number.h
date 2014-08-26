#ifndef IMAGE_NUMBER_H
#define IMAGE_NUMBER_H

#include <string>
#include <iostream>

/**
 * @brief The PBM_ASCII_Reader class
 *
 * Reader for ASCII PBM images
 */
class PBM_ASCII_Reader
{
public:

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

#endif // IMAGE_NUMBER_H
