
#include <math.h> // for sqrt
#include <cstring> // for memcpy

#include "GaussianBlur.h"

GaussianBlur::GaussianBlur(Color* pixels, const int& width, const int& height, const float& radius) :
	width(width),
	height(height)
{
    GenerateBoxes(radius);
    // @NOTE Assuming BoxCount == 3 to allow easy memory reuse
    _STATIC_ASSERT(boxCount == 3);

    Color* temp = new Color[width * height];

    //@TODO: Boxes[] is only used here and nowhere else in the code??
    //  So we can do the (x-1) / 2 step in the generator?
    BlurPass(pixels, temp, (boxes[0] - 1) / 2);
    BlurPass(temp, pixels, (boxes[1] - 1) / 2);
    BlurPass(pixels, temp, (boxes[2] - 1) / 2);


}
void GaussianBlur::GenerateBoxes(const float & radius)
{
    float sigmaSq = radius * radius;
    float idealFilterWidth = sqrt((12.0f * sigmaSq / boxCount) + 1.0f);

    int lower = (int)floor(idealFilterWidth); 
    if (lower % 2 == 0)
        lower--;
    
    float lowerSq = lower * lower;

    // copypasta calculus
    // http://blog.ivank.net/fastest-gaussian-blur.html

    float mIdeal = (12 * sigmaSq - boxCount * lowerSq -
        4 * boxCount * lower - 3 * boxCount) /
        (-4 * lower - 4);

    int m = round(mIdeal);

    for (int i = 0; i < boxCount; i++) 
        boxes[i] = (i < m ? lower : lower + 2);
}

void GaussianBlur::BlurPass(Color* source,  Color* dest, const float& radius)
{
    memcpy(dest, source, width * height * sizeof(Color));

    BoxHorizontal(dest, source, radius);
    BoxVertical(source, dest, radius);
}

void GaussianBlur::BoxHorizontal(Color* source, Color* dest, const float& radius)
{
    float iarr = 1 / (radius + radius + 1); // ?

    for (auto y = 0; y < height; y++)
    {
        int ti = y * width;
        int li = ti;
        int ri = ti + radius;

        Color fv = source[ti]; //first value?
        Color lv = source[ti + width - 1]; // last value?
        int accumulator = (radius + 1) * fv.value; // ?!?!??!


        for (auto j = 0; j < radius; j++)
        {
            accumulator += source[ti + j].value;
        }

        for (auto j = 0; j <= radius; j++)
        { 
            accumulator += source[ri++].value - fv.value;
            dest[ti++].value = round(accumulator * iarr);
        }

        for (auto j = radius + 1; j < width - radius; j++)
        {
            accumulator += source[ri++].value - source[li++].value;
            dest[ti++].value = round(accumulator * iarr);
        }
        for (auto j = width - radius; j < width; j++)
        {
            accumulator += lv.value - source[li++].value;
            dest[ti++].value = round(accumulator * iarr);
        }
    }
}

void GaussianBlur::BoxVertical(Color* source, Color* dest, const float& radius)
{
    float iarr = 1 / (radius + radius + 1); // ?

    for (auto x = 0; x < width; x++)
    {

        int ti = x;
        int li = ti;
        int ri = ti + radius * width;

        Color fv = source[ti];
        Color lv = source[ti + width * (height - 1)];
        int accumulator = (radius + 1) * fv.value; // ?!?!??!

        for (auto j = 0; j < radius; j++)
        {
            accumulator += source[ti + j * width].value;
        }


        for (auto j = 0; j <= radius; j++)
        {
            accumulator += source[ri].value - fv.value;
            dest[ti].value = round(accumulator * iarr);

            // advance to next row
            ri += width;
            ti += width;
        }

        for (auto j = radius + 1; j < height - radius; j++)
        {
            accumulator += source[ri].value - source[li].value;
            dest[ti].value = round(accumulator * iarr);

            li += width;
            ri += width;
            ti += width;
        }

        for (auto j = height - radius; j < height; j++)
        {
            accumulator += lv.value - source[li].value;
            dest[ti].value = round(accumulator * iarr);

            li += width;
            ti += width;
        }
    }
}