#pragma once

#include <vector>
#include <opencv2/imgcodecs.hpp>

namespace imgproc
{
	template <typename T>
	struct Point
	{
		T x, y;
		Point() = default;
		Point(T _x, T _y) : x(_x), y(_y) {}
	};

	// rotation
	class Rotation
	{	
	
	public:
		enum class rotateMethod { FWD_MAP, INV_MAP };
		static constexpr double PI = 3.14159;
		static const cv::Mat rotate(cv::Mat& img, double angle, rotateMethod method);
	
	private:
		static void rotateFwd(const cv::Mat& oImg, cv::Mat& rImg, double angle);
		static void rotateInv(cv::Mat& oImg, cv::Mat& rImg, double angle);

	};
}

