#pragma once

#include "Color.h"
//@TODO: This is way too slow to run real-time. Optimize later if there's
// time before deadline, otherwise just don't use it...
class GaussianBlur
{
public:
    GaussianBlur(Color* Pixels, const int& width, const int& height, const float& radius);

private:
    static constexpr int boxCount = 3;

    void GenerateBoxes(const float& radius);
    void BlurPass(Color* source, Color* dest, const float& radius);

    void BoxHorizontal(Color* source, Color* dest, const float& radius);
    void BoxVertical(Color* source, Color* dest, const float& radius);


    const int width;
    const int height;

    int boxes[boxCount];
};



#if 0


    void boxBlurH_4(scl, tcl, w, h, r)
    {
        var iarr = 1 / (r + r + 1);
        for (var i = 0; i < h; i++) {
            var ti = i * w, li = ti, ri = ti + r;
            var fv = scl[ti], lv = scl[ti + w - 1], val = (r + 1) * fv;
            for (var j = 0; j < r; j++) val += scl[ti + j];
            for (var j = 0; j <= r; j++) { val += scl[ri++] - fv;   tcl[ti++] = Math.round(val * iarr); }
            for (var j = r + 1; j < w - r; j++) { val += scl[ri++] - scl[li++];   tcl[ti++] = Math.round(val * iarr); }
            for (var j = w - r; j < w; j++) { val += lv - scl[li++];   tcl[ti++] = Math.round(val * iarr); }
        }
    }

    void boxBlurT_4(scl, tcl, w, h, r)
    {
        var iarr = 1 / (r + r + 1);
        for (var i = 0; i < w; i++) {
            var ti = i, li = ti,
                ri = ti + r * w;
            var fv = scl[ti],
                lv = scl[ti + w * (h - 1)],
                val = (r + 1) * fv;
            for (var j = 0; j < r; j++) val += scl[ti + j * w];
            for (var j = 0; j <= r; j++) { val += scl[ri] - fv;  tcl[ti] = Math.round(val * iarr);  ri += w; ti += w; }
            for (var j = r + 1; j < h - r; j++) { val += scl[ri] - scl[li];  tcl[ti] = Math.round(val * iarr);  li += w; ri += w; ti += w; }
            for (var j = h - r; j < h; j++) { val += lv - scl[li];  tcl[ti] = Math.round(val * iarr);  li += w; ti += w; }
        }
    }
}

#endif