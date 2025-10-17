#include "rotate.h"
#include "translate.h"
#include "scale.h"
#include "similarity.h"
#include "GaussianFilter.h"
#include "enhancements.h"

#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>

#include <vector>
#include <cmath>

int main()
{
	using namespace imgproc;

	Image img = matToImg(cv::imread("/home/mevil/Documents/ImageProcessing/gaussian_noise.png"));

	if (img.empty())
		return -1;
           
	// rotation
    Image rotatedImgFwd = Rotation::rotate(img, 45, Rotation::rotateMethod::FWD_MAP);
	//Image rotatedImgInv = Rotation::rotate(img, 45, Rotation::rotateMethod::INV_MAP);
    cv::imshow("rotatedImgFwd", imgToMat(rotatedImgFwd));
	//cv::imshow("rotatedImgInv", imgToMat(rotatedImgInv));

	// translation
	// Image translatedImg = translate(img, 100, 100);
	// cv::imshow("translatedImg", imgToMat(translatedImg));
	
	// scale
	// Image scaledImg = Scale::scale(img, Scale::InterpolationMethod::Bilinear, 3);
	// Image scaledImg1 = Scale::scale(img, Scale::InterpolationMethod::NearestNeighbour, 3);
	// cv::imshow("scaled -- NN", imgToMat(scaledImg1));
	// cv::imshow("scaled -- bilinear", imgToMat(scaledImg));
	
	// Image similarTransformImg = similarityTransform(img);
	// cv::imshow("similarityTransform", imgToMat(similarTransformImg));

	// blur
	//auto blurredImg3 = applyGuassian(img, 3, 3.f);
	//auto blurredImg5 = applyGuassian(img, 5, 5.f);
	//auto blurredImg7 = applyGuassian(img, 7, 10.f);
	
	//cv::imshow("blurred3", imgToMat(blurredImg3));
	//cv::imshow("blurred5", imgToMat(blurredImg5));
	//cv::imshow("blurred7", imgToMat(blurredImg7));

	//std::vector<Image> pyramid = getGuassianPyramid(img);
	//
	//for (int i = 0; i < pyramid.size(); i++)
	//	cv::imshow("level " + std::to_string(i + 1), imgToMat(pyramid[i]));

	//Image darkImg = adjustBrightness(img, -100);
	//Image brightImg = adjustBrightness(img, 100);
	//Image invertedImg = invert(img);
	//Image contrastLowImg = contrast(img, 0.5);
	//Image contrastHighImg = contrast(img, 1.5);
	//cv::Mat grayImg = toGrayscale(img);
	//
	//cv::imshow("darkened",   imgToMat(darkImg));
	//cv::imshow("brightened", imgToMat(brightImg));
	//cv::imshow("inverted", imgToMat(invertedImg));
	//cv::imshow("contrast lowered", imgToMat(contrastLowImg));
	//cv::imshow("contrast Higher",  imgToMat(contrastHighImg));
    //cv::imshow("grayscale", grayImg);

	// TODO: 
	// - image pyramids (Laplacian), non-linear filters, template matching
	// - edge/corner detection
	// 	- simple feature detector
	// - stereo vision
	// - 3D reconstruction

    cv::imshow("originalImg", imgToMat(img));
    cv::waitKey();
    cv::destroyAllWindows();

    return 0;
}