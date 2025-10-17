#pragma once

#include "imgOps.h"

namespace imgproc
{
    class Scale
    {
    public:
        enum class InterpolationMethod
        {
            NearestNeighbour,
            Bilinear
        };

        static Image scale(const Image& img,
            const InterpolationMethod intMethod,
            const uint8_t scale);

    private:

        static Image nearestNeighbour(const Image& img,
            const uint8_t scale);
        static Image bilinear(const Image& img, const uint8_t scale);
    };
}
