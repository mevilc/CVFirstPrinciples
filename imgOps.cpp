// basic operations

#include "imgOps.h"
#include <opencv2/core/hal/interface.h>
#include <opencv2/opencv.hpp>

// helper functions
cv::Mat imgproc::imgToMat(Image &img)
{
	int nChannels = 0;
	if (img.channels == 3)
		nChannels = CV_8UC3;
	else if (img.channels == 1)
		nChannels = CV_8UC1;

	return cv::Mat(img.rows, img.cols, nChannels,
			img.pixels.data());
}

imgproc::Image imgproc::matToImg(cv::Mat mat)
{
	Image img;
	img.rows = mat.rows;
	img.cols = mat.cols;
	img.channels = mat.channels();
	img.pixels = { mat.data, mat.data + mat.total() * mat.elemSize() };

	return img;
}

size_t imgproc::getIdxFromPt(const int x, const int y,
	const int nCols, const int nChannels)
{
	return (y * nCols + x) * nChannels;
}

imgproc::Point<int>
imgproc::getPtFromIdx(const size_t idx, const int nCols)
{
	return Point<int>(idx % nCols, idx / nCols);
}


imgproc::Pixel
imgproc::Image::getPixel(int y, int x) const
{
	// int idx =  (x + cols * y) * 3;
	int idx = getIdxFromPt(y, x, cols, channels);
	Pixel pixel{};
	if (channels == 3)
	{
		pixel.r = pixels.at(idx);
		pixel.g = pixels.at(idx + 1);
		pixel.b = pixels.at(idx + 2);
		printf("breaks here");
	}
	else if (channels == 1)
	{
		pixel.r = pixels.at(idx);
	}
	return pixel;
}

void imgproc::Image::setPixel(int y, int x, const Pixel& pixel)
{
	// int idx =  (x + cols * y) * 3;
	int idx = getIdxFromPt(y, x, cols, channels);
	printf("idx %d from point (%d, %d)", idx, x, y);
	if (channels == 3)
	{
		pixels.at(idx) = pixel.r;
		pixels.at(idx + 1) = pixel.g;
		pixels.at(idx + 2) = pixel.b;
	}
	else if (channels == 1)
	{
		pixels.at(idx) = pixel.r;
	}
}



