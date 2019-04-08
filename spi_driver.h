#pragma once

#include <SPI.h>

namespace hand
{

    #ifdef HAND_REGISTER_T
        typedef HAND_REGISTER_T register_t;
    #else
        typedef uint32_t register_t;
    #endif


    class spi_driver
    {
    public:
        virtual void select() = 0;
        virtual void unselect() = 0;
        virtual uint8_t transfer(uint8_t byte) = 0;
        virtual void transfer(const uint8_t* buffer, size_t size) = 0;
        virtual void exchange(uint8_t* buffer, size_t size) = 0;

        virtual void exchange(const uint8_t* input, uint8_t* output, size_t size);

        virtual void init() = 0;
    };
}
