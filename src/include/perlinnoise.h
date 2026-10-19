#pragma once
#include <array>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>

class PerlinNoise
{
public:
    PerlinNoise(unsigned int seed = 12345)
    {
        std::array<int, 256> permutation{};
        std::iota(permutation.begin(), permutation.end(), 0);
        std::mt19937 engine(seed);
        std::shuffle(permutation.begin(), permutation.end(), engine);

        for (int i = 0; i < 256; ++i)
        {
            p_[i] = permutation[i];
            p_[256 + i] = p_[i];
        }
    }

    float Noise(float x, float y) const
    {
        int xi = static_cast<int>(std::floor(x)) & 255;
        int yi = static_cast<int>(std::floor(y)) & 255;

        float xf = x - std::floor(x);
        float yf = y - std::floor(y);

        float u = Fade(xf);
        float v = Fade(yf);

        int aa = p_[p_[xi] + yi];
        int ab = p_[p_[xi] + yi + 1];

        int ba = p_[p_[xi + 1] + yi];
        int bb = p_[p_[xi + 1] + yi + 1];

        float x1 = Lerp(Grad(aa, xf, yf), Grad(ba, xf - 1.0f, yf), u);
        float x2 = Lerp(Grad(ab, xf, yf - 1.0f), Grad(bb, xf - 1.0f, yf - 1.0f), u);

        return Lerp(x1, x2, v);
    };

private:
    std::array<int, 512> p_{};

    static float Fade(float t)
    {
        return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }

    static float Lerp(float a, float b, float t)
    {
        return a + t * (b - a);
    }

    static float Grad(int hash, float x, float y)
    {
        switch(hash & 7)
        {
            case 0:return x + y;
            case 1: return -x + y;
            case 2: return  x - y;
            case 3: return -x - y;
            case 4: return  x;
            case 5: return -x;
            case 6: return  y;
            default:return -y;
        };
    }
};