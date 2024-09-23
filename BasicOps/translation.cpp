#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

cv::Mat translate(cv::Mat img, int tx, int ty)
{
	// +tx = right
	// +ty = up

	if (img.empty())
		return cv::Mat();

	ty = -1 * ty;	// convert from regular cartesian to opencv

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

int main()
{
	std::string imgPath = "C:/CVFirstPrinciples/rgbImage.png";
	cv::Mat bgrImg = cv::imread(imgPath);
	cv::Mat translatedImg = translate(bgrImg, -100, -100);

	cv::imshow("bgrOriginal", bgrImg);
	cv::imshow("translatedImg", translatedImg);
	cv::waitKey();
	cv::destroyAllWindows();

	return 0;
}