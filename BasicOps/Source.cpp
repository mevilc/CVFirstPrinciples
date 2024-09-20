// basic operations

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

template <typename T>
struct Point
{
	T x, y;
	Point() = default;
	Point( T _x, T _y) : x(_x), y(_y) {}
};

cv::Mat rotate(cv::Mat oImg, double angle)
{
	constexpr double PI = 3.1418;
	int originalImgWidth = oImg.cols;
	int originalImgHeight = oImg.rows;

	// CCW in LL system is CW in UL system
	angle = -1 * angle;

	// 2 approaches: forward and inverse mapping

	// forward mapping: 
	// [x, y] (int) --> [x', y'] (float) --> [x', y'] (rounded ints)

	// new image size must accomodate the rotated image

	Point<int> originalUL { 0, 0 };
	Point<int> originalUR { originalImgWidth - 1, 0 };
	Point<int> originalLL { 0, originalImgHeight - 1 };
	Point<int> originalLR { originalImgWidth - 1, originalImgHeight - 1 };
	
	// rotate each original corner to get new corner

	Point<int> rotatedUL{}, rotatedUR{}, rotatedLL{}, rotatedLR{};

	rotatedLR.x = static_cast<int>
		(std::round(std::cos(angle / 180.0 * PI) * originalLR.x - std::sin(angle / 180.0 * PI) * originalLR.y));
	rotatedLR.y = static_cast<int>
		(std::round(std::sin(angle / 180.0 * PI) * originalLR.x + std::cos(angle / 180.0 * PI) * originalLR.y));

	rotatedUR.x = static_cast<int>
		(std::round(std::cos(angle / 180.0 * PI) * originalUR.x - std::sin(angle / 180.0 * PI) * originalUR.y));
	rotatedUR.y = static_cast<int>
		(std::round(std::sin(angle / 180.0 * PI) * originalUR.x + std::cos(angle / 180.0 * PI) * originalUR.y));

	rotatedUL.x = static_cast<int>
		(std::round(std::cos(angle / 180.0 * PI) * originalUL.x - std::sin(angle / 180.0 * PI) * originalUL.y));
	rotatedUL.y = static_cast<int>
		(std::round(std::sin(angle / 180.0 * PI) * originalUL.x + std::cos(angle / 180.0 * PI) * originalUL.y));
	
	rotatedLL.x = static_cast<int>
		(std::round(std::cos(angle / 180.0 * PI) * originalLL.x - std::sin(angle / 180.0 * PI) * originalLL.y));
	rotatedLL.y = static_cast<int>
		(std::round(std::sin(angle / 180.0 * PI) * originalLL.x + std::cos(angle / 180.0 * PI) * originalLL.y));

	// find any negative x or y rotated positions and scale all points by that in x or y 
	// this way the min x and y positions are at 0 and able to be rasterized
	int negY = std::min({ rotatedUL.y, rotatedUR.y, rotatedLL.y, rotatedLR.y });
	int negX = std::min({ rotatedUL.x, rotatedUR.x, rotatedLL.x, rotatedLR.x });
	if (negY < 0)
	{
		// translate all rotated corner Y coordinates by negY to make all y coords positive
		negY = std::abs(negY);
		rotatedUL.y += negY;
		rotatedUR.y += negY;
		rotatedLL.y += negY;
		rotatedLR.y += negY;
	}
	if (negX < 0)
	{
		// translate all rotated corner X coordinates by negX to make all x coords positive
		negX = std::abs(negX);
		rotatedUL.x += negX;
		rotatedUR.x += negX;
		rotatedLL.x += negX;
		rotatedLR.x += negX;
	}

	// sort rotated points to get new image dimensions
	auto [minX, maxX] = std::minmax( {rotatedUL.x, rotatedUR.x, rotatedLL.x, rotatedLR.x } );
	auto [minY, maxY] = std::minmax( { rotatedUL.y, rotatedUR.y, rotatedLL.y, rotatedLR.y } );

	int newWidth = maxX - minX;
	int newHeight = maxY - minY;

	// create black image
	cv::Mat rotatedImg(newHeight, newWidth, CV_8UC3, cv::Scalar(0, 0, 0));	

	for (int y = 0; y < originalImgHeight; y++)
	{
		for (int x = 0; x < originalImgWidth; x++)
		{
			// apply rotation to each original pixel position to get rotated pixel position
			// round new fp position to closest int

			int x_prime = static_cast<int>
							(std::round(std::cos(angle / 180.0 * PI) * x - std::sin(angle / 180.0 * PI) * y));
			int y_prime = static_cast<int>
							(std::round(std::sin(angle / 180.0 * PI) * x + std::cos(angle / 180.0 * PI) * y));

			// translate position to fit in viewport
			y_prime += negY;
			x_prime += negX;

			// validity check
			if (y_prime < minY || x_prime < minX || y_prime >= maxY || x_prime >= maxX)
				continue;

			// extract color at original image pixel position
			cv::Vec3b originalClr = oImg.at<cv::Vec3b>(y, x);	// 8 bit
			
			// dump color to rotated image pixel position
			rotatedImg.at<cv::Vec3b>(y_prime, x_prime) = originalClr;
		}
	}
	return rotatedImg;
}

int main()
{
	std::string imgPath = "C:/CVFirstPrinciples/rgbImage.png";
	cv::Mat bgrImg = cv::imread(imgPath);
	cv::Mat rotatedImg = rotate(bgrImg, 135);

	
	cv::imshow("bgrOriginal", bgrImg);
	cv::imshow("rotatedImg", rotatedImg);
	cv::waitKey();
	cv::destroyAllWindows();

	return 0;

}
