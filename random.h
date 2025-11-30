#pragma once

#include <random>

class Random
{
public:
    Random() : mt(rd()) {}

    int RandomInt(int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(mt);
    }

    float RandomFloat(float min, float max)
    {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(mt);
    }

private:
    std::random_device rd;
    std::mt19937 mt;

};