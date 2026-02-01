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
#include <iostream>

using namespace imgproc;

void rotate(const Image& img)
{
    Image rotatedImgFwd = Rotation::rotate(img, 45, Rotation::rotateMethod::FWD_MAP);
	Image rotatedImgInv = Rotation::rotate(img, 45, Rotation::rotateMethod::INV_MAP);
    cv::imshow("rotatedImgFwd", imgToMat(rotatedImgFwd));
	cv::imshow("rotatedImgInv", imgToMat(rotatedImgInv));
}


void translate(const Image& img)
{
    Image translatedImg = translate(img, 50, 50);
	cv::imshow("translatedImg", imgToMat(translatedImg));
}

void scale(const Image& img)
{
    Image scaledImg = Scale::scale(img, Scale::InterpolationMethod::Bilinear, 2);
	Image scaledImg2 = Scale::scale(img, Scale::InterpolationMethod::NearestNeighbour, 3);

    cv::imshow("scaled - bilinear - 2", imgToMat(scaledImg));
    cv::imshow("scaled -- NN - 3", imgToMat(scaledImg2));
}

void simTransform(const Image& img)
{
	Image similarTransformImg = similarityTransform(img);
	cv::imshow("similarityTransform", imgToMat(similarTransformImg));
}

int main()
{

	//Image img = matToImg(cv::imread("/Users/mevilcrasta/Documents/CVFirstPrinciples/gaussian_noise.png"));
	Image img = matToImg(cv::imread("/Users/mevilcrasta/Documents/CVFirstPrinciples/boat-compressed_36.jpg"));
    // Image img = matToImg(cv::imread("/Users/mevilcrasta/Documents/CVFirstPrinciples/boat-compressed.jpg", cv::IMREAD_GRAYSCALE ));

    // rotate(img);
    // translate(img);
    // scale(img);
    simTransform(img);


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
