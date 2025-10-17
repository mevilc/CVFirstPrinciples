#pragma once

#include "imgOps.h"

namespace imgproc
{
    static constexpr double PI = 3.14159;

    class Rotation
    {
    public:
        enum class rotateMethod { FWD_MAP, INV_MAP };
        static Image rotate(const Image& oImg, double angle,
            rotateMethod method);

    private:
        static void rotateFwd(const Image& oImg, Image& rImg,
            double angle);
        static void rotateInv(const Image& oImg, Image& rImg, double angle);

    };
}