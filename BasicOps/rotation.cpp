// basic operations

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

#include <opencv2/highgui.hpp>

#include "Source.h"

using namespace imgproc;

const cv::Mat Rotation::rotate(cv::Mat& oImg, double angle, rotateMethod method)
{
	
	int originalImgWidth = oImg.cols;
	int originalImgHeight = oImg.rows;

	// CCW in LL system is CW in UL system
	angle *= -1;

	// new image size must accomodate the rotated image
	Point<int> originalUL { 0, 0 };
	Point<int> originalUR { originalImgWidth - 1, 0 };
	Point<int> originalLL { 0, originalImgHeight - 1 };
	Point<int> originalLR { originalImgWidth - 1, originalImgHeight - 1 };
	
	// translate corners to center so that rotation is applied about center
	originalUL.x -= originalImgWidth / 2;
	originalUL.y -= originalImgHeight / 2;

	originalUR.x -= originalImgWidth / 2;
	originalUR.y -= originalImgHeight / 2;

	originalLL.x -= originalImgWidth / 2;
	originalLL.y -= originalImgHeight / 2;

	originalLR.x -= originalImgWidth / 2;
	originalLR.y -= originalImgHeight / 2;


	// rotate each original, translated corner to get new corner
	Point<int> rotatedUL{}, rotatedUR{}, rotatedLL{}, rotatedLR{};
	double angleRad = angle / 180.0 * PI;

	rotatedLR.x = static_cast<int>
		(std::round(std::cos(angleRad) * originalLR.x - std::sin(angleRad) * originalLR.y));
	rotatedLR.y = static_cast<int>
		(std::round(std::sin(angleRad) * originalLR.x + std::cos(angleRad) * originalLR.y));

	rotatedUR.x = static_cast<int>
		(std::round(std::cos(angleRad) * originalUR.x - std::sin(angleRad) * originalUR.y));
	rotatedUR.y = static_cast<int>
		(std::round(std::sin(angleRad) * originalUR.x + std::cos(angleRad) * originalUR.y));

	rotatedUL.x = static_cast<int>
		(std::round(std::cos(angleRad) * originalUL.x - std::sin(angleRad) * originalUL.y));
	rotatedUL.y = static_cast<int>
		(std::round(std::sin(angleRad) * originalUL.x + std::cos(angleRad) * originalUL.y));
	
	rotatedLL.x = static_cast<int>
		(std::round(std::cos(angleRad) * originalLL.x - std::sin(angleRad) * originalLL.y));
	rotatedLL.y = static_cast<int>
		(std::round(std::sin(angleRad) * originalLL.x + std::cos(angleRad) * originalLL.y));

	// translate rotated coords back from center in original coordinate space
	rotatedUL.x += originalImgWidth / 2;
	rotatedUL.y += originalImgHeight / 2;

	rotatedUR.x += originalImgWidth / 2;
	rotatedUR.y += originalImgHeight / 2;

	rotatedLL.x += originalImgWidth / 2;
	rotatedLL.y += originalImgHeight / 2;

	rotatedLR.x += originalImgWidth / 2;
	rotatedLR.y += originalImgHeight / 2;

	// sort rotated points to get new image dimensions
	auto [minX, maxX] = std::minmax( { rotatedUL.x, rotatedUR.x, rotatedLL.x, rotatedLR.x } );
	auto [minY, maxY] = std::minmax( { rotatedUL.y, rotatedUR.y, rotatedLL.y, rotatedLR.y } );

	int newWidth = maxX - minX;
	int newHeight = maxY - minY;

	// create black image
	cv::Mat rotatedImg(newHeight, newWidth, CV_8UC3, cv::Scalar(0, 0, 0));	
	
	// 2 approaches: forward and inverse mapping
	// forward mapping: 
	// [x, y] (int) --> [x', y'] (float) --> [x', y'] (rounded ints)

	if (method == rotateMethod::FWD_MAP)
		rotateFwd(oImg, rotatedImg, angleRad);

	else if (method == rotateMethod::INV_MAP)
		rotateInv(oImg, rotatedImg, angleRad);

	return rotatedImg;
}

void Rotation::rotateFwd(const cv::Mat& oImg, cv::Mat& rImg, double angle)
{
	for (int y = 0; y < oImg.rows; y++)
	{
		for (int x = 0; x < oImg.cols; x++)
		{
			// translate original coords to center
			int centeredX = x - oImg.cols / 2;
			int centeredY = y - oImg.rows / 2;

			// rotate
			int newX = static_cast<int>
				(std::round(std::cos(angle) * centeredX - std::sin(angle) * centeredY));
			int newY = static_cast<int>
				(std::round(std::sin(angle) * centeredX + std::cos(angle) * centeredY));

			// translate rotated coords back from center in rotated coord space
			newX += rImg.cols / 2;
			newY += rImg.rows / 2;

			// validity check
			if (newY < 0 || newX < 0 || newY >= rImg.rows || newX >= rImg.cols)
				continue;

			// dump color to rotated image pixel position
			rImg.at<cv::Vec3b>(newY, newX) = oImg.at<cv::Vec3b>(y, x);
			
		}
	}
}

void Rotation::rotateInv(cv::Mat& oImg, cv::Mat& rImg, double angle)
{
	int posX{}, posY{}, ct{};

	for (int yPrime = 0; yPrime < rImg.rows; yPrime++)
	{
		for (int xPrime = 0; xPrime < rImg.cols; xPrime++)
		{
			// apply inverse rotation to each rotated pixel position to get original pixel position
			// round new fp position to closest int

			// translate rotated coordinates to center in rotated coord space
			int xCentered = xPrime - rImg.cols / 2;
			int yCentered = yPrime - rImg.rows / 2;

			int x = static_cast<int>
				(std::round(std::cos(angle) * xCentered + std::sin(angle) * yCentered));
			int y = static_cast<int>
				(std::round(-1 * std::sin(angle) * xCentered + std::cos(angle) * yCentered));

			// translate rotated coords back to center in original coord space
			y += oImg.rows/ 2;
			x += oImg.cols / 2;

			// validity check
			if (y < 0 || x < 0 || y >= oImg.rows || x >= oImg.cols)
				continue;

			// dump color to rotated image pixel position
			rImg.at<cv::Vec3b>(yPrime, xPrime) = oImg.at<cv::Vec3b>(y, x);
		}
	}
}

int main()
{
	std::string imgPath = "C:/CVFirstPrinciples/rgbImage.png";
	cv::Mat img = cv::imread(imgPath);

	// rotation
	cv::Mat rotatedImgFwd = Rotation::rotate(img, 15, Rotation::rotateMethod::FWD_MAP);
	cv::Mat rotatedImgInv = Rotation::rotate(img, 15, Rotation::rotateMethod::INV_MAP);
	
	cv::imshow("original", img);
	cv::imshow("rotatedImgFwd", rotatedImgFwd);
	cv::imshow("rotatedImgInv", rotatedImgInv);
	cv::waitKey();
	cv::destroyAllWindows();

	return 0;

}
