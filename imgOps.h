#pragma once

#include <opencv2/core/mat.hpp>

#include <vector>

// Pixels are stored in a single contigous buffer.  If color, they are interleaved BGR (from openCV).
// Origin top-left == same as OpenCV

namespace imgproc
{
	template <typename T>
	struct Point
	{
		T x{}, y{};
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
		Image(const int _rows, const int _cols, const int _channels)
		: rows(_rows), cols(_cols), channels(_channels) 
		{
			/*
				handle black and RGB images
				
				make all pixels black/interleaved (RGB) 
				[ I  I  I  I ]
				[ R  G  B  R ] 
				[ 8B 8B 8B 8B]

				(x,y) -> (0,0) top left corner coordinate system

				3x3 RGB image:
				[ R0 G0 B0 R1 G1 B1 R2 G2 B2 
					R3 G3 B3 R4 G4 B4 R5 G5 B5 	
					R6 G6 B6 R7 G7 B7 R8 G8 B8 ]
				
				3x3 black image:
				[ I I I
					I I I 
					I I I ] 
				
				3x3 image positions:
				[ (0,0), (0,1), (0,2)
					(1,0), (1,1), (1,2)
					(2,0), (2,1), (2,2)
				]

				idx = (y + cols * x) * channels
				(x,y) -- i
				(0,0)	 0
				(0,2)	 6
				(1,0)    9
				(2,2)	 24

			*/
			
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
	Image matToImg(const cv::Mat& mat);
}

