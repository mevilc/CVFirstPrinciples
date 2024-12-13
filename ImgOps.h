#pragma once

#include <vector>
#include <opencv2/imgcodecs.hpp>
#include <optional>

namespace imgproc
{
	template <typename T>
	struct Point
	{
		T x, y;
		Point() = default;
		Point(T _x, T _y) : x(_x), y(_y) {}
	};

	static constexpr double PI = 3.14159;

	class Rotation
	{	
	
	public:
		enum class rotateMethod { FWD_MAP, INV_MAP };	
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

		static const cv::Mat scale(cv::Mat img, InterpolationMethod intMethod, uint8_t scale);
	private:
		static void nearestNeighbour(const cv::Mat& oImg, cv::Mat& sImg, uint8_t scale);
		static void bilinear(const cv::Mat& oImg, cv::Mat& sImg, uint8_t scale);
		static const bool inline neighValid(const Point<int> pt, const int imgW, const int imgH);
		static const std::vector<float>
			interpolate(const Point<float> dist, const Point<int>& validNeigh,
				const Point<int>& rightNeigh, const cv::Mat& oImg);
	};     
	
	class GuassianFilter
	{
	public:
		static const std::optional<cv::Mat> applyGuassian(const cv::Mat& img, const uint8_t kernelSize,
			const float stdDev);

	private:
		using Kernel = std::vector<float>;
		static const std::optional<Kernel>
			computeKernel(const uint8_t kernelSize, const float stdDev);
	};

	enum class Brightenss { DARKEN, BRIGHTEN };
	static const cv::Mat adjustBrightness(const cv::Mat& img, uint8_t adjustBy,
		Brightenss brightness);

	static const cv::Mat invert(const cv::Mat& img);

	enum class Contrast { LOW, HIGH };
	static const cv::Mat contrast(const cv::Mat& img, float factor, 
		Contrast contrast);

	static const cv::Mat toGrayscale(const cv::Mat& img);
	
	static const std::vector<cv::Mat>
		getGuassianPyramid(const cv::Mat& img);

	static const cv::Mat similarityTransform(cv::Mat& img);

}

