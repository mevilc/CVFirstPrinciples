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

imgproc::Image imgproc::matToImg(const cv::Mat& mat)
{
	Image img;
	img.rows = mat.rows;
	img.cols = mat.cols;
	img.channels = mat.channels();
	img.pixels = { mat.data, mat.data + mat.total() * mat.elemSize() };

	return img;
}

imgproc::Pixel
imgproc::Image::getPixel(int y, int x) const
{
	int idx =  (x + cols * y) * channels;
	Pixel pixel{};
	if (channels == 3)
	{
		pixel.r = pixels.at(idx);
		pixel.g = pixels.at(idx + 1);
		pixel.b = pixels.at(idx + 2);
	}
	else if (channels == 1)
	{
		pixel.r = pixels.at(idx);
	}
	return pixel;
}

void imgproc::Image::setPixel(int y, int x, const Pixel& pixel)
{
	int idx =  (x + cols * y) * channels;
	
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



