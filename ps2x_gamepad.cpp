#ifdef ENABLE_PS2X_SUPPORT

#include "ps_gamepad.h"

using namespace hand;

ps2x_gamepad::ps2x_gamepad(int miso, int mosi, int sck, int ss) :
    m_miso(miso),
    m_mosi(mosi),
    m_sck(sck),
    m_ss(ss)
    {}

ps2_config_result ps2x_gamepad::configure()
{
    auto result = m_ps2x.config_gamepad(m_sck, m_mosi, m_ss, m_miso);
    switch(result)
    {
        case 0:
            return ps2_config_result::OK;
            break;
        case 1:
            return ps2_config_result::NO_CONTROLLER;
            break;
        default:
            return ps2_config_result::NO_CONNECTION;
            break;
    }
}

void ps2x_gamepad::update(uint8_t m1, uint8_t m2)
{
    m_ps2x.read_gamepad(m1, m2);
}

bool ps2x_gamepad::button(uint16_t b)
{
    return m_ps2x.Button(b);
}

bool ps2x_gamepad::released(uint16_t b)
{
    return m_ps2x.ButtonReleased(b);
}

bool ps2x_gamepad::pressed(uint16_t b)
{
    return m_ps2x.ButtonPressed(b);
}

bool ps2x_gamepad::changed(uint16_t b)
{
    return m_ps2x.NewButtonState(b);
}

bool ps2x_gamepad::changed()
{
    return m_ps2x.NewButtonState();
}

uint8_t ps2x_gamepad::analog(ps2_analog data)
{
    return m_ps2x.Analog(data);
}

#endif
