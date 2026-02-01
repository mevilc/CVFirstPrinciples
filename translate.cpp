#include "imgOps.h"
#include "translate.h"

#include <iostream>

using namespace imgproc;

Image imgproc::translate(const Image& img, int tx, int ty)
{
    // top-left origin
    // +tx = right
    // +ty = down 

    if (img.empty())
        return Image{};

    // determine new image size
    int newWidth = img.cols + std::abs(tx);
    int newHeight = img.rows + std::abs(ty);

    Image translatedImg(newHeight, newWidth, img.channels);

    int cStart = tx < 0 ? 0 : translatedImg.cols - img.cols;
    int rStart = ty < 0 ? 0 : translatedImg.rows - img.rows;

    /*
        original image:        translated image
                               (2 right, 2 down):
        X X                    Y Y Y Y 
        X X                    Y Y Y Y
                               Y Y X X
                               Y Y X X
    */

    for (int r = rStart; r < translatedImg.rows; r++)
    {
        for (int c = cStart; c < translatedImg.cols; c++)
        {
            translatedImg.setPixel(r, c, img.getPixel(r - rStart, c - cStart));
        }
    }
    
    return translatedImg;
}
