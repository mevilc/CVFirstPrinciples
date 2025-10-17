#include "GaussianFilter.h"
#include "imgOps.h"
#include "scale.h"

#include <cmath>

//using namespace imgproc;
imgproc::Kernel 
imgproc::computeKernel(const uint8_t kernelSize, const float stdDev)
{
    Kernel kernel;
    kernel.resize(kernelSize);

    // 3x3: [-1 0 1]
    // 5x5: [-2 -1 0 1 2]
    float expBottom = 2 * std::pow(stdDev, 2);
    int kernelPositionOffset = -kernelSize / 2;
    for (int i = 0; i < kernelSize; i++)
    {
        kernel[i] = std::exp(-std::pow(kernelPositionOffset,2) / 
            expBottom);
        kernelPositionOffset++;
    }
    
    // normalize kernel
    float sum = 0;
    for (const float w : kernel)
        sum += w;
    
    for (float& w : kernel)
        w /= sum;
    
    return kernel;
}

imgproc::Image 
imgproc::padImage(const Image &img, const int padBy)
{
    Image padded(img.rows + padBy * 2,
                 img.cols + padBy * 2,
                 img.channels);

    int startFromCol = padBy;
    int startFromRow = padBy; 
    for (int y = 0; y < padded.rows; y++)
    {
        for (int x = 0; x < padded.cols; x++)
        {
            if (x >= startFromCol && x < (startFromCol + img.cols) &&
                y >= startFromRow && y < (startFromRow + img.rows))
                {
                    padded.setPixel(y, x, 
                        img.getPixel(y - startFromRow, x - startFromCol));
                }
        }
    }
    return padded;
}

 
imgproc::Image imgproc::applyGuassian(
    const Image& img, const uint8_t kernelSize,
    const float stdDev)
{
    // Naive Implementation:
	// - Using a 2D Guassian Kernel
	// Optimized Implementation:
	// - Break 2D Kernel into 2 seperable 1-D identical kernels

        
    /*
        original image:
        XXXXX
        XXXXX
        XXXXX

        3x3 kernel -> padded image by 1 on all sides:
        PPPPPPP
        PXXXXXP
        PXXXXXP
        PXXXXXP
        PPPPPPP

        filtered image:
        YYYYY
        YYYYY
        YYYYY

    */

    // create copy of input image to return
    Image gaussImg = img;

    Image a = std::move(img);
    // create kernel
    Kernel kernel = computeKernel(kernelSize, stdDev);

    // pad original image
    int padEachSideBy = kernelSize / 2;
    Image originalPadded = padImage(img, padEachSideBy);

    // horizontal pass    
    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            // for each pixel in padded image, convolve its neighbourhood with the kernel
            float r{}, g{}, b{};
            for (int k = -padEachSideBy; k <= padEachSideBy; k++)
            {
                // increment position by padding
                // if original image position is (0,0) and padding is 1, then position 
                // of that original position in the padded image is (1,1). Basically scaling 
                Pixel originalPixel = originalPadded.getPixel(
                    y + padEachSideBy, x + padEachSideBy + k);
                r += kernel[k + padEachSideBy] * originalPixel.r;
                g += kernel[k + padEachSideBy] * originalPixel.g;
                b += kernel[k + padEachSideBy] * originalPixel.b;
            }

            Pixel newPixel;
            newPixel.r = std::clamp<uint8_t>(r, 0, 255);
            newPixel.g = std::clamp<uint8_t>(g, 0, 255);
            newPixel.b = std::clamp<uint8_t>(b, 0, 255);

            gaussImg.setPixel(y, x, newPixel);
        }
    }

    // pad horizontally-passed image to feed it to vertical pass
    Image horizontalPassedPadded = padImage(gaussImg, padEachSideBy);

    // vertical pass    
    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            // for each pixel in padded image, convolve its neighbourhood with the kernel
            float r{}, g{}, b{};
            for (int k = -padEachSideBy; k <= padEachSideBy; k++)
            {
                // increment 
                Pixel originalPixel = horizontalPassedPadded.getPixel(
                    y + padEachSideBy + k, x + padEachSideBy);
                r += kernel[k + padEachSideBy] * originalPixel.r;
                g += kernel[k + padEachSideBy] * originalPixel.g;
                b += kernel[k + padEachSideBy] * originalPixel.b;
            }

            Pixel newPixel;
            newPixel.r = std::clamp<uint8_t>(r, 0, 255);
            newPixel.g = std::clamp<uint8_t>(g, 0, 255);
            newPixel.b = std::clamp<uint8_t>(b, 0, 255);
            
            gaussImg.setPixel(y, x, newPixel);
        }
    }

    return gaussImg;
}

std::vector<imgproc::Image> 
imgproc::getGuassianPyramid(const imgproc::Image& img)
{
    using namespace imgproc;

    std::vector<Image> pyramid; // L1, L2...
	constexpr uint8_t minSize = 32;

	if (img.empty())
		return pyramid;
	
	pyramid.push_back(img);

	while (pyramid.back().rows > minSize || pyramid.back().cols > minSize)
	{
		// always process next level from previous finer level
		Image currPyrLevel = pyramid.back();
		Image currPyrLevelBlurred = applyGuassian(currPyrLevel, 3, 1.6f);

        Image nextPyrLevel(currPyrLevel.rows / 2, currPyrLevel.cols / 2, currPyrLevel.channels);

		//  Old   New
		// (0,0)  (0,0)
		// (0,2)  (0,1)
		// (0,4)  (0,2)
		// (2,0)  (1, 0)
		for (int y = 0; y < currPyrLevelBlurred.rows; y += 2)
		{
			for (int x = 0; x < currPyrLevelBlurred.cols; x += 2)
                nextPyrLevel.setPixel(y / 2, x / 2, currPyrLevelBlurred.getPixel(y, x));
		}
		pyramid.push_back(nextPyrLevel);
	}
	return pyramid;
}