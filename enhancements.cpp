#include "enhancements.h"
#include "imgOps.h"

#include <algorithm>

imgproc::Image 
imgproc::adjustBrightness(const Image& img, int beta)
{
	// beta < 0 --> darken = p - beta
	// beta > 0 --> brighten = p + beta

	if (beta == 0)
		return img;

	Image newImg = img;

	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{
            Pixel oldPixel = img.getPixel(y, x);
            
            int r = (int)oldPixel.r + beta;
            int g = (int)oldPixel.g + beta;
            int b = (int)oldPixel.b + beta;

            oldPixel.r = (uint8_t)(std::max(0, std::min(r, 255)));
            oldPixel.g = (uint8_t)(std::max(0, std::min(g, 255)));
            oldPixel.b = (uint8_t)(std::max(0, std::min(b, 255)));

            newImg.setPixel(y, x, oldPixel);
		}
	}
	return newImg;
}

imgproc::Image imgproc::invert(const Image& img)
{
	// 255 - f
	if (img.empty())
		return img;

	Image invertedImg(img.rows, img.cols, img.channels);

	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{
            Pixel oldPixel = img.getPixel(y, x);
			
            int r = 255 - (int)oldPixel.r;
            int g = 255 - (int)oldPixel.g;
            int b = 255 - (int)oldPixel.b;

            Pixel newPixel;
            newPixel.r = (uint8_t)(std::max(0, std::min(r, 255)));
            newPixel.g = (uint8_t)(std::max(0, std::min(g, 255)));
            newPixel.b = (uint8_t)(std::max(0, std::min(b, 255)));

            invertedImg.setPixel(y, x, newPixel);
		}
	}

	return invertedImg;
}

imgproc::Image imgproc::contrast(const Image& img, float alpha)
{
    // a < 1 --> lower constrast; reducing dynamic range
	// a > 1 --> higher contrast; increasing dynamic range (clipped to 255)

	if (img.empty() || alpha == 0.f)
		return img;

	Image contrastImg(img.rows, img.cols, img.channels);

	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{	
            Pixel oldPixel = img.getPixel(y, x);
			
            int r = (int)oldPixel.r * alpha;
            int g = (int)oldPixel.g * alpha;
            int b = (int)oldPixel.b * alpha;

            Pixel newPixel;
            newPixel.r = (uint8_t)(std::max(0, std::min(r, 255)));
            newPixel.g = (uint8_t)(std::max(0, std::min(g, 255)));
            newPixel.b = (uint8_t)(std::max(0, std::min(b, 255)));
			contrastImg.setPixel(y, x, newPixel);
		}
	}

	return contrastImg;
}

//imgproc::Image imgproc::grayscale(const Image& img)
//{
//    if (img.empty())
//		return img;
//	
//	// gray = 0.3 * R + 0.6 * G + 0.1 * B
//	Image grayImg(img.rows, img.cols);
//
//	for (int y = 0; y < img.rows; y++)
//	{
//		for (int x = 0; x < img.cols; x++)
//		{
//			Pixel oldPixel = img.getPixel(y, x);
//			
//            float gray = 0.3 * (int)oldPixel.r + 0.6 * (int)oldPixel.g +
//                0.1 * (int)oldPixel.b;  
//
//            Pixel newPixel;
//            newPixel.r = (uint8_t)(std::max(0, std::min(r, 255)));
//            newPixel.g = (uint8_t)(std::max(0, std::min(g, 255)));
//            newPixel.b = (uint8_t)(std::max(0, std::min(b, 255)));
//			contrastImg.setPixel(y, x, newPixel);
//		}
//	}
//	return grayImg;
//}