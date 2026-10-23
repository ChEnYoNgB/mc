#pragma once
#include <array>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>

class PerlinNoise3D
{
public:
    PerlinNoise3D(unsigned int seed = 12345)
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

    float Noise(float x, float y, float z)
    {
        int xi = static_cast<int>(std::floor(x)) & 255;
        int yi = static_cast<int>(std::floor(y)) & 255;
        int zi = static_cast<int>(std::floor(z)) & 255;

        float xf = x - std::floor(x);
        float yf = y - std::floor(y);
        float zf = z - std::floor(z);

        float u = Fade(xf);
        float v = Fade(yf);
        float w = Fade(zf);

        int aaa = p_[p_[p_[xi] + yi] + zi];
        int baa = p_[p_[p_[xi + 1] + yi] + zi];
        int bab = p_[p_[p_[xi + 1] + yi + 1] + zi];
        int aab = p_[p_[p_[xi] + yi + 1] + zi];
        int aba = p_[p_[p_[xi] + yi] + zi + 1];
        int bba = p_[p_[p_[xi + 1] + yi] + zi + 1];
        int bbb = p_[p_[p_[xi + 1] + yi + 1] + zi + 1];
        int abb = p_[p_[p_[xi] + yi + 1] + zi + 1];

        float x1 = Lerp(Grad(aaa, xf, yf, zf), Grad(baa, xf - 1, yf, zf), u);
        float x2 = Lerp(Grad(aba, xf, yf - 1, zf), Grad(bba, xf - 1, yf - 1, zf), u);
        float x3 = Lerp(Grad(aab, xf, yf, zf - 1), Grad(bab, xf - 1, yf, zf - 1), u);
        float x4 = Lerp(Grad(abb, xf, yf - 1, zf - 1), Grad(bbb, xf - 1, yf - 1, zf - 1), u);
        float y1 = Lerp(x1, x2, v);
        float y2 = Lerp(x3, x4, v);
        return Lerp(y1, y2, w);
    }

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

    static float Grad(int hash, float x, float y, float z)
    {
        switch(hash & 15)
        {
            case 0: return x + y;
            case 1: return -x + y;
            case 2: return  x - y;
            case 3: return -x - y;
            case 4: return  x + z;
            case 5: return -x + z;
            case 6: return  x - z;
            case 7: return -x - z;
            case 8: return y + z;
            case 9: return -y + z;
            case 10: return y - z;
            case 11: return -y - z;
            case 12: return x + y;
            case 13: return -y + z;
            case 14: return  -x + y;
            default: return -y - z;
        };
    }
    
};