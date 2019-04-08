#pragma once
#include "spi_driver.h"

namespace hand
{
    class software_spi_driver : public spi_driver
    {
    public:
        void select();
        void unselect();
        uint8_t transfer(uint8_t byte);
        void transfer(const uint8_t* buffer, size_t size);
        void exchange(uint8_t* buffer, size_t size);
        void init();

        software_spi_driver(unsigned mosi, unsigned miso, unsigned sck, unsigned ss);

    private:
        unsigned m_mosi_pin, m_miso_pin, m_sck_pin, m_ss_pin;
        volatile register_t *m_mosi_reg, *m_miso_reg, *m_sck_reg, *m_ss_reg;
        register_t m_mosi_mask, m_miso_mask, m_sck_mask, m_ss_mask;

        unsigned m_clk_delay, m_byte_delay;

        void set_mosi();
        void reset_mosi();

        bool check_miso();

        void set_ss();
        void reset_ss();

        void set_sck();
        void reset_sck();
    };
}
