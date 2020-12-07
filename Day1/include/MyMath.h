#pragma once
#include <cstdint>

class MyMath
{
public :
    MyMath() = delete;
    ~MyMath() = delete;

    static int Add(const uint16_t& a, const uint16_t& b);
    static int Multiply(const uint16_t& a, const uint16_t& b);
};