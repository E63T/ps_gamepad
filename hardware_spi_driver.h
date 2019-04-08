#pragma once
#include "spi_driver.h"

namespace hand
{
    class hardware_spi_driver : public spi_driver
    {
    public:
        void select();
        void unselect();
        uint8_t transfer(uint8_t byte);
        void transfer(const uint8_t* buffer, size_t size);
        void exchange(uint8_t* buffer, size_t size);
        virtual void init();

        hardware_spi_driver(SPISettings settings, unsigned ss_pin);

    private:
        SPISettings m_settings;
        unsigned m_ss_pin;
    };
}
