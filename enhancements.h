#pragma once

#include "imgOps.h"

namespace imgproc
{
    Image adjustBrightness(const Image& img, int beta);

    Image invert(const Image& img);

    Image contrast(const Image& img, float alpha);

    Image grayscale(const Image& img);
}