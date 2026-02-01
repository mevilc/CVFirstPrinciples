#include "similarity.h"
#include "imgOps.h"
#include "scale.h"
#include "rotate.h"
#include "translate.h"

imgproc::Image
imgproc::similarityTransform(const Image &img)
{
    // scale, rotate, translate
    return translate(
            Rotation::rotate(
                Scale::scale(img, Scale::InterpolationMethod::NearestNeighbour, 2), 
                45, Rotation::rotateMethod::INV_MAP
                ), 
                100, 100
            );
}
