// basic operations

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

#include <opencv2/highgui.hpp>

#include "ImgOps.h"

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

const cv::Mat translate(cv::Mat img, int tx, int ty)
{
	// +tx = right
	// +ty = up

	if (img.empty())
		return cv::Mat();

	ty *= -1;	// convert from regular cartesian to opencv

	// determine new image size
	int newWidth = img.cols + std::abs(tx);
	int newHeight = img.rows + std::abs(ty);

	cv::Mat translatedImg = cv::Mat(newHeight, newWidth, CV_8UC3, cv::Scalar(0, 0, 0));

	int newX = tx > 0 ? tx : 0;
	int newY = ty < 0 ? 0 : ty;
	cv::Rect roi(newX, newY, img.cols, img.rows);

	img.copyTo(translatedImg(roi));

	return translatedImg;

}

void Scale::nearestNeighbour(const cv::Mat& oImg, cv::Mat& sImg, uint8_t scale)
{
	// for each integer position in new image, find its fp equivalent in original image
	// round to nearest integer position. 

	for (int y = 0; y < sImg.rows; y++)
	{
		for (int x = 0; x < sImg.cols; x++)
		{
			int xOriginal = static_cast<int>( std::round(x / scale) );
			int yOriginal = static_cast<int>( std::round(y / scale) );
			
			// validity
			if (xOriginal < 0 || yOriginal < 0 || xOriginal >= oImg.cols || yOriginal >= oImg.rows)
				continue;

			sImg.at<cv::Vec3b>(y, x) = oImg.at<cv::Vec3b>(yOriginal, xOriginal);
		}
	}
}

const inline bool
Scale::neighValid(const Point<int> pt, const int imgW, const int imgH)
{
	return (pt.x >= 0 && pt.y >= 0 && pt.x < imgW && pt.y < imgH);
}

const std::vector<float>
Scale::interpolate(const Point<float> dist, const Point<int>& leftNeigh, 
	const Point<int>& rightNeigh, const cv::Mat& oImg)
{
	// interpolate between left and right neighbors
	float interpR = (1.f - dist.x) * oImg.at<cv::Vec3b>(leftNeigh.y, leftNeigh.x)[2]
		+ dist.x * oImg.at<cv::Vec3b>(rightNeigh.y, rightNeigh.x)[2];

	float interpG = (1.f - dist.x) * oImg.at<cv::Vec3b>(leftNeigh.y, leftNeigh.x)[1]
		+ dist.x * oImg.at<cv::Vec3b>(rightNeigh.y, rightNeigh.x)[1];
	
	float interpB = (1.f - dist.x) * oImg.at<cv::Vec3b>(leftNeigh.y, leftNeigh.x)[0]
		+ dist.x * oImg.at<cv::Vec3b>(rightNeigh.y, rightNeigh.x)[0];

	return std::vector<float>{ interpR, interpG, interpB };
}

void Scale::bilinear(const cv::Mat& oImg, cv::Mat& sImg, uint8_t scale)
{
	// linear interpolation
	// for each integer position in new image, find its fp equivalent in original image
	// get 4 integer neighbourhood pixels -- UL, UR, LL, LR
	// dx = fp.X - xPos; dy = fp.Y - yPos
	// interpolate in x:  L-R (top and bottom)
	// interpolate in y:  T-B (final)

	for (int y = 0; y < sImg.rows; y++)
	{
		for (int x = 0; x < sImg.cols; x++)
		{
			// fp coords in original image
			Point<float> orPt( static_cast<float>(x) / scale, 
				               static_cast<float>(y) / scale );
			if (orPt.x < 0.f || orPt.y < 0.f || orPt.x >= oImg.cols || orPt.y >= oImg.rows)
				continue;

			// int neighbors
			Point<int> uL{ static_cast<int>( std::floor(orPt.x) ), 
						     static_cast<int>( std::floor(orPt.y) ) };
			Point<int> uR{ static_cast<int>( std::floor(orPt.x) ) + 1,
						     static_cast<int>( std::floor(orPt.y) ) };
			Point<int> lL{ static_cast<int>( std::floor(orPt.x) ),
						     static_cast<int>( std::floor(orPt.y) ) + 1 };
			Point<int> lR{ static_cast<int>( std::floor(orPt.x) ) + 1,
							 static_cast<int>( std::floor(orPt.y) ) + 1 };

			// guaranteed to have the UL neighbor of fp position
			Point<float> dist{ orPt.x - uL.x, orPt.y - uL.y };

			// check which neighbours are valid. Invalid neighbors need to be clamped
			// to valid ones. 
			bool ulValid = neighValid(uL, oImg.cols, oImg.rows);
			bool uRValid = neighValid(uR, oImg.cols, oImg.rows);
			bool lLValid = neighValid(lL, oImg.cols, oImg.rows);
			bool lRValid = neighValid(lR, oImg.cols, oImg.rows);

			// top interpolation:	 ITop = (1 - dx) * Iul + dx * Iur
			// bottom interpolation: IBot = (1- dx) * Ill + dx + Ilr
			std::vector<float> topInterp(3), botInterp(3), finInterp(3);

			if (ulValid && !uRValid && lLValid && !lRValid)
			{
				// UR: only UL, LL are valid
				// clamp UL to UR
				topInterp = interpolate(dist, uL, uL, oImg);
				// clamp LL to LR
				botInterp = interpolate(dist, lL, lL, oImg);
			}
			else if (ulValid && uRValid && !lLValid && !lRValid)
			{
				// LL: only UR, UL are valid

				topInterp = interpolate(dist, uL, uR, oImg);
				// make LL = UL, UR = LR. This makes top and bottom same intensities.
				botInterp = topInterp;
			}
			else if (ulValid && !uRValid && !lLValid && !lRValid)
			{
				// LR: only UL is valid
				
				// make UR = UL, LL, LR. This makes all neighbors same intensities.
				topInterp = interpolate(dist, uL, uL, oImg);
				botInterp = topInterp;
			}
			else if (ulValid && uRValid && lLValid && lRValid)
			{
				// UL: all neighbors are valid
				topInterp = interpolate(dist, uL, uR, oImg);
				botInterp = interpolate(dist, lL, lR, oImg);
			}
			
			// final interpolation:  IFin = (1 - dy) * ITop + dy * IBot
			finInterp[0] = (1 - dist.y) * topInterp[0] + dist.y * botInterp[0]; // R
			finInterp[1] = (1 - dist.y) * topInterp[1] + dist.y * botInterp[1]; // G
			finInterp[2] = (1 - dist.y) * topInterp[2] + dist.y * botInterp[2]; // B

			sImg.at<cv::Vec3b>(y, x) = cv::Vec3b( static_cast<uint8_t>(finInterp[2]),
				                                  static_cast<uint8_t>(finInterp[1]),
				                                  static_cast<uint8_t>(finInterp[0]) );

		}
	}
}

const cv::Mat Scale::scale(cv::Mat img, InterpolationMethod intMethod, uint8_t scale)
{
	// uniform scaling: aspect ratio is maintained

	if (img.empty())
		return cv::Mat();

	if (scale == 0)
		return img;

	cv::Mat scaledImg(img.rows * scale, img.cols * scale, CV_8UC3, cv::Scalar(0, 0, 0));

	if (intMethod == InterpolationMethod::NearestNeighbour)
		nearestNeighbour(img, scaledImg, scale);
	else if (intMethod == InterpolationMethod::Bilinear)
		bilinear(img, scaledImg, scale);

	return scaledImg;
}

int main()
{
	std::string imgPath = "C:/CVFirstPrinciples/rgbImage.png";
	cv::Mat img = cv::imread(imgPath);

	// rotation
	//cv::Mat rotatedImgFwd = Rotation::rotate(img, 15, Rotation::rotateMethod::FWD_MAP);
	cv::Mat rotatedImgInv = Rotation::rotate(img, 15, Rotation::rotateMethod::INV_MAP);
	
	cv::Mat translatedImg = translate(rotatedImgInv, 100, 100);
	//cv::Mat scaledImg = Scale::scale(img, Scale::InterpolationMethod::Bilinear, 2);
	cv::Mat scaledImg1 = Scale::scale(translatedImg, Scale::InterpolationMethod::NearestNeighbour, 2);

	cv::imshow("original", img);
	//cv::imshow("rotatedImgFwd", rotatedImgFwd);
	//cv::imshow("rotatedImgInv", rotatedImgInv);
	//cv::imshow("translatedImg", translatedImg);
	//cv::imshow("scaledImg", scaledImg);
	cv::imshow("scaledImg1", scaledImg1);

	cv::waitKey();
	cv::destroyAllWindows();

	return 0;

}
