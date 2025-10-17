#pragma once

#include <opencv2/core/mat.hpp>

#include <vector>

namespace imgproc
{
	template <typename T>
	struct Point
	{
		T x, y;
		Point() = default;
		Point(T _x, T _y) : x(_x), y(_y) {}
	};

	struct Pixel
	{
		uint8_t r, g, b;
	};

	struct Image
	{
		Image() = default;
		Image(const int _rows, const int _cols, const int _channels = 3)
		: rows(_rows), cols(_cols), channels(_channels) 
		{
			// make all pixels black
			pixels.resize(rows * cols * channels, 0);
		}

		int rows = 0;
		int cols = 0;
		int channels = 0;
		std::vector<uint8_t> pixels;

		Pixel getPixel(int y, int x) const;

		void setPixel(int y, int x,
			const Pixel& pixel);

		bool empty() const { return pixels.empty(); }
	};

	cv::Mat imgToMat(Image& img);
	Image matToImg(cv::Mat mat);

	inline size_t getIdxFromPt(const int x, const int y,
		const int nCols, const int nChannels);

	Point<int> 
	getPtFromIdx(const size_t idx, const int nCols);
}

