#include "hardware_spi_driver.h"

using namespace hand;

hardware_spi_driver::hardware_spi_driver(SPISettings settings, unsigned ss_pin) :
    m_settings(settings),
    m_ss_pin(ss_pin)
    {}

void hardware_spi_driver::select()
{
    #ifdef __AVR__
        SPI.beginTransaction(m_settings);
        digitalWrite(m_ss_pin, LOW);
    #else
        SPI.beginTransaction(m_ss_pin, m_settings);
    #endif
}

void hardware_spi_driver::unselect()
{
    #ifdef __AVR__
        digitalWrite(m_ss_pin, HIGH);
        SPI.endTransaction();
    #else
        SPI.endTransaction(m_ss_pin);
    #endif
}

void hardware_spi_driver::init()
{
    SPI.begin();
}

uint8_t hardware_spi_driver::transfer(uint8_t byte)
{
    return SPI.transfer(byte);
}

void hardware_spi_driver::exchange(uint8_t *buffer, size_t size)
{
    SPI.transfer(buffer, size);
}

void hardware_spi_driver::transfer(const uint8_t *buffer, size_t size)
{
    for(size_t i = 0; i < size; i++)
        SPI.transfer(buffer[i]);
}
