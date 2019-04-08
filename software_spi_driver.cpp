#include "software_spi_driver.h"
#include <Arduino.h>

using namespace hand;

software_spi_driver::software_spi_driver(unsigned mosi, unsigned miso, unsigned sck, unsigned ss) :
    m_mosi_pin(mosi),
    m_miso_pin(miso),
    m_sck_pin(sck),
    m_ss_pin(ss),
    #ifdef __AVR__
        m_clk_delay(4),
        m_byte_delay(3)
    #else
        m_clk_delay(5),
        m_byte_delay(4)
    #endif
    {}

void software_spi_driver::init()
{
    pinMode(m_ss_pin, OUTPUT);
    pinMode(m_sck_pin, OUTPUT);
    pinMode(m_mosi_pin, OUTPUT);
    #if defined(__AVR__)
        pinMode(m_miso_pin, INPUT);
    #else
        pinMode(m_miso_pin, INPUT_PULLUP);
    #endif
    m_mosi_reg = portOutputRegister(digitalPinToPort(m_mosi_pin));
    m_mosi_mask = digitalPinToBitMask(m_mosi_pin);

    m_miso_reg = portInputRegister(digitalPinToPort(m_mosi_pin));
    m_miso_mask = digitalPinToBitMask(m_miso_pin);

    m_sck_reg = portOutputRegister(digitalPinToPort(m_sck_pin));
    m_sck_mask = digitalPinToBitMask(m_sck_pin);

    m_ss_reg = portOutputRegister(digitalPinToPort(m_ss_pin));
    m_ss_mask = digitalPinToBitMask(m_ss_pin);

    #if defined(__AVR__)
        //digitalWrite(m_miso_pin, HIGH);
    #endif

    set_mosi();
    set_sck();

    //unselect();
}

void software_spi_driver::select()
{
    set_mosi();
    set_sck();
    reset_ss();
    delayMicroseconds(m_byte_delay);
}

void software_spi_driver::unselect()
{
    set_mosi();
    set_sck();
    set_ss();
}

uint8_t software_spi_driver::transfer(uint8_t byte)
{
    uint8_t temp = 0;
    for(size_t i = 0; i < 8; i++)
    {
        if(byte & (1 << i))
            set_mosi();
        else
            reset_mosi();

        reset_sck();

        delayMicroseconds(m_clk_delay);

        if(check_miso()) temp |= (1 << i);

        set_sck();

        delayMicroseconds(m_clk_delay);
    }
    set_mosi();
    delayMicroseconds(m_byte_delay);
    return temp;
}

void software_spi_driver::transfer(const uint8_t *buffer, size_t size)
{
    for(size_t i = 0; i < size; i++)
        transfer(buffer[i]);
}

void software_spi_driver::exchange(uint8_t *buffer, size_t size)
{
    for(size_t i = 0; i < size; i++)
        buffer[i] = transfer(buffer[i]);
}


void software_spi_driver::set_mosi()
{
    #ifdef ENABLE_SLOW_DIGITAL
        digitalWrite(m_mosi_pin, HIGH);
    #else
        *m_mosi_reg |= m_mosi_mask;
    #endif
}

void software_spi_driver::reset_mosi()
{
    #ifdef ENABLE_SLOW_DIGITAL
        digitalWrite(m_mosi_pin, LOW);
    #else
        *m_mosi_reg &= ~m_mosi_mask;
    #endif
}

void software_spi_driver::set_sck()
{
    #ifdef ENABLE_SLOW_DIGITAL
        digitalWrite(m_sck_pin, HIGH);
    #else
        *m_sck_reg |= m_sck_mask;
    #endif
}

void software_spi_driver::reset_sck()
{
    #ifdef ENABLE_SLOW_DIGITAL
        digitalWrite(m_sck_pin, LOW);
    #else
        *m_sck_reg &= ~m_sck_mask;
    #endif
}

void software_spi_driver::set_ss()
{
    #ifdef ENABLE_SLOW_DIGITAL
        digitalWrite(m_ss_pin, HIGH);
    #else
        *m_ss_reg |= m_ss_mask;
    #endif
}

void software_spi_driver::reset_ss()
{
    #ifdef ENABLE_SLOW_DIGITAL
        digitalWrite(m_ss_pin, LOW);
    #else
        *m_ss_reg &= ~m_ss_mask;
    #endif
}

bool software_spi_driver::check_miso()
{
    #ifdef ENABLE_SLOW_DIGITAL
        return digitalRead(m_miso_pin);
    #else
        return *m_miso_reg & m_miso_mask;
    #endif
}
