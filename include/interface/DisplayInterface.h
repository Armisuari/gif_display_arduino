#pragma once

#include <inttypes.h>

class DisplayInterface
{
public:
    virtual bool begin() = 0;
    virtual bool setImage(const uint8_t image[]) = 0;
    virtual bool setGifImage(const uint8_t gifImage[]) = 0;
    virtual bool clear() = 0;
};
