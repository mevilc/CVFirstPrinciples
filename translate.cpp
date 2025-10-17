#include "imgOps.h"
#include "translate.h"

using namespace imgproc;

Image imgproc::translate(const Image& img, int tx, int ty)
{
    // +tx = right
    // +ty = up

    if (img.empty())
        return Image{};

    ty *= -1;	// convert from regular cartesian to top-left-origin

    // determine new image size
    int newWidth = img.cols + std::abs(tx);
    int newHeight = img.rows + std::abs(ty);

    Image translatedImg(newHeight, newWidth, img.channels);

    int xStart = tx < 0 ? 0 : translatedImg.cols - img.cols;
    int yStart = ty < 0 ? 0 : translatedImg.rows - img.rows;

    /*
        original image:        translated image
                               (2 right, 2 up):
        X X                    Y Y X X
        X X                    Y Y X X
                               Y Y Y Y
    */
    auto imgIterStart = img.pixels.cbegin();

    for (int colToReplace = 0; colToReplace < img.cols; colToReplace++)
    {
        // access iterator of img vector
        auto imgIterEnd = imgIterStart + (img.cols * 3);

        // img: [rgb rgb rgb...]
        // access img vector idx to insert: idx = (y * newImageCols + x) * 3
        int idxToInsert = ( (yStart + colToReplace) * translatedImg.cols + xStart ) * 3;

        translatedImg.pixels.insert(
            translatedImg.pixels.begin() + idxToInsert,
            imgIterStart, imgIterEnd);

        imgIterStart = imgIterEnd;
    }
    return translatedImg;
}