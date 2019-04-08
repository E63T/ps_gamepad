#include "spi_driver.h"

using namespace hand;

void spi_driver::exchange(const uint8_t* in, uint8_t* out, size_t size)
{
    for(size_t i = 0; i < size; i++)
        out[i] = transfer(in[i]);
}
