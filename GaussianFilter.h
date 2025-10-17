#pragma once

#include "imgOps.h"

namespace imgproc
{
    Image applyGuassian(const Image& img, const uint8_t kernelSize,
        const float stdDev);

    Image padImage(const Image& img, const int padBy);
    
    using Kernel = std::vector<float>;
    Kernel
    computeKernel(const uint8_t kernelSize, const float stdDev);
  
    std::vector<Image> getGuassianPyramid(const Image& img);
}
