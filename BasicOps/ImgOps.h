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

	class Scale
	{
	public:
		enum class InterpolationMethod
		{
			NearestNeighbour,
			Bilinear,
		};

		struct Neighbors
		{
			Point<float> uL, uR, lL, lR;
		};

		static const cv::Mat scale(cv::Mat img, InterpolationMethod intMethod, uint8_t scale);
	private:
		static void nearestNeighbour(const cv::Mat& oImg, cv::Mat& sImg, uint8_t scale);
		static void bilinear(const cv::Mat& oImg, cv::Mat& sImg, uint8_t scale);
		static const bool inline neighValid(const Point<int> pt, const int imgW, const int imgH);
		static const std::vector<float>
			interpolate(const Point<float> dist, const Point<int>& validNeigh,
				const Point<int>& rightNeigh, const cv::Mat& oImg);
	};
}

