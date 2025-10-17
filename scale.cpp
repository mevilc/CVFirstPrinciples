#include "scale.h"
#include "imgOps.h"

#include <cmath>

using namespace imgproc;

Image Scale::nearestNeighbour(const Image& img, const uint8_t scale)
{
	// for each integer position in new image, find its fp equivalent in original image
	// round to nearest integer position.
	Image sImg(img.rows * scale, img.cols * scale, img.channels);

	for (int i = 0; i < sImg.rows * sImg.cols * 3; i += 3)
	{
		auto sImgPt = getPtFromIdx(i, sImg.cols);
		// Find corresponding position in the original image (source pixel)
		int nearestOrigX = std::round(sImgPt.x / scale);
		int nearestOrigY = std::round(sImgPt.y / scale);

		// Ensure within bounds
		if (nearestOrigX >= img.cols || nearestOrigY >= img.rows)
			continue;

		// Copy pixel data from original image to scaled image
		sImg.setPixel(sImgPt.y, sImgPt.x, img.getPixel(nearestOrigY, nearestOrigX));
	}
	
	return sImg;
}

Image Scale::bilinear(const Image& img, const uint8_t scale)
{
	// linear interpolation
	// for each integer position in new image, find its fp equivalent in original image
	// get 4 integer neighbourhood pixels -- UL, UR, LL, LR
	// dx = fp.X - xPos; dy = fp.Y - yPos
	// interpolate in x:  L-R (top and bottom)
	// interpolate in y:  T-B (final)

	Image sImg(img.rows * scale, img.cols * scale, img.channels);

	for (int i = 0; i < sImg.rows * sImg.cols * 3; i += 3)
	{
		auto sImgPt = getPtFromIdx(i, sImg.cols);
		// Find corresponding position in the original image (source pixel)
		float scaledToOrigX = sImgPt.x / (float)scale;
		float scaledToOrigY = sImgPt.y / (float)scale;

		// Ensure within bounds
		if (scaledToOrigX >= img.cols || scaledToOrigY >= img.rows)
			continue;
		
		// neighbours
		Point<int> ul(std::floor(scaledToOrigX), std::floor(scaledToOrigY));
		Point<int> lr(std::ceil(scaledToOrigX), std::ceil(scaledToOrigY));
		lr.x = std::min(lr.x, img.cols - 1);
		lr.y = std::min(lr.y, img.rows - 1);
		
		Point<int> ur(lr.x, ul.y);
		ur.x = std::min(ur.x, img.cols - 1);
		ur.y = std::min(ur.y, img.rows - 1);
		
		Point<int> ll(ul.x, lr.y);

		float a = std::abs( lr.x - scaledToOrigX );
		float b = std::abs( lr.y - scaledToOrigY );
		
		// pixel = (1 - a)(1 - b)P00 + a(1 - b)P10 + (1 - a)bP01 + abP1
	
		Pixel ulPix = img.getPixel(ul.y, ul.x);
		Pixel urPix = img.getPixel(ur.y, ur.x);
		Pixel llPix = img.getPixel(ll.y, ll.x);
		Pixel lrPix = img.getPixel(lr.y, lr.x);

		// Copy pixel data from original image to scaled image
		sImg.setPixel(sImgPt.y, sImgPt.x, Pixel{
			.r = static_cast<uint8_t>( (1 - a) * (1 - b) * ulPix.r + a * (1 - b) * urPix.r + (1 - a) * b * llPix.r + a * b * lrPix.r ),
			.g = static_cast<uint8_t>( (1 - a) * (1 - b) * ulPix.g + a * (1 - b) * urPix.g + (1 - a) * b * llPix.g + a * b * lrPix.g ),
			.b = static_cast<uint8_t>( (1 - a) * (1 - b) * ulPix.b + a * (1 - b) * urPix.b + (1 - a) * b * llPix.b + a * b * lrPix.b )
			
		});
	}

	return sImg;
}

Image Scale::scale(const Image& img,
			const InterpolationMethod intMethod,
			const uint8_t scale)
{
	// uniform scaling: aspect ratio is maintained

	if (img.empty())
		return Image{};

	if (scale == 0)
		return img;

	Image sImg;
	if (intMethod == InterpolationMethod::NearestNeighbour)
		sImg = nearestNeighbour(img, scale);
	else if (intMethod == InterpolationMethod::Bilinear)
		sImg = bilinear(img, scale);

	return sImg;
}
