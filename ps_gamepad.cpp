#include "ps_gamepad.h"

using namespace hand;

ps2_gamepad::ps2_gamepad(spi_driver &driver, bool rumble, bool pressures) :
    m_driver(driver),
    m_rumble(rumble),
    m_pressures(pressures)
{

}

ps2_config_result ps2_gamepad::configure()
{

    m_driver.init();

    #ifdef ENABLE_DEBUG
        m_log = logger_instance;

        if(m_log)
        {
            m_log->begin();
            m_log->print("PS2 Gamepad initialized");
            m_log->end();
        }
    #endif

    m_last_read = millis();
    m_read_delay = 10; // TODO

    readGamepad(0, 0);
    readGamepad(0, 0);

    #ifdef ENABLE_DEBUG
        if(m_log)
        {
            m_log->begin();
            m_log->print("Read gamepad, byte at [1] : 0x");
            m_log->print((int)m_buffer[1], HEX);
            m_log->end();
        }
    #endif

    switch (m_buffer[1])
    {
        case 0x41:
        case 0x73:
        case 0x79:
            break;
        default:
            m_last_config_result = NO_CONTROLLER;
            return NO_CONTROLLER;
    }

    uint8_t type[sizeof(m_type_read)];

    //sendCommand(m_enter_cfg, sizeof(m_enter_cfg));
    //sendCommand(m_type_read, type, sizeof(m_type_read));

    m_driver.select();
    m_driver.transfer(m_enter_cfg, sizeof(m_enter_cfg));
    m_driver.unselect();

    m_driver.select();
    m_driver.exchange(m_type_read, type, sizeof(m_type_read));
    m_driver.unselect();

    m_controller_type = type[3];

    #ifdef ENABLE_DEBUG
        if(m_log)
        {
            m_log->begin();
            m_log->print("Controller type : 0x");
            m_log->print((int)type[3], HEX);
            m_log->end();
        }
    #endif

    reconfigure();

    readGamepad(0, 0);

    m_last_config_result = (m_buffer[1] == 0x73 && m_pressures) ? NO_CONNECTION : OK;
    return m_last_config_result;
}

void ps2_gamepad::readGamepad(uint8_t motor1, uint8_t motor2)
{
    m_last_buttons = m_buttons;
    unsigned long last_delay = millis() - m_last_read;

    if(last_delay > 1500)
    {
        #ifdef ENABLE_DEBUG
            if(m_log)
            {
                m_log->begin();
                m_log->print("Timeout over 1500 ms");
                m_log->end();
            }
        #endif
        reconfigure();
    }
    if(last_delay < m_read_delay) delay(m_read_delay - last_delay);

    for(int i = 0; i < 21; i++)
    {
        m_buffer[i] = 0;
    }

    m_buffer[0] = 0x01;
    m_buffer[1] = 0x42;
    m_buffer[3] = motor1;
    m_buffer[4] = motor2;

    for(unsigned attempt = 0; attempt < 5; attempt++)
    {
        m_driver.select();
        m_driver.exchange(m_buffer, 9);

        if(m_buffer[1] == 0x79) // Full Data Return Mode
            m_driver.exchange(m_buffer + 9, 12);

        m_driver.unselect();

        if((m_buffer[1] & 0xF0) == 0x70) break;

        #ifdef ENABLE_DEBUG
            if(m_log)
            {
                m_log->begin();
                m_log->print("Trying once more (");
                m_log->print(attempt+1);
                m_log->print("/5)");
                m_log->end();
            }
        #endif

        reconfigure();
        delay(m_read_delay);
    }

    m_buttons = *(uint16_t*)(m_buffer + 3);

    m_last_read = millis();

    #ifdef ENABLE_DEBUG
        if(m_log)
        {
            m_log->begin();
            m_log->print("Buffer: ");
            for(int i = 0; i < 21; i++)
            {
                m_log->print(m_buffer[i], HEX);
                m_log->print(" ");
            }
            m_log->end();
        }
    #endif
}

void ps2_gamepad::reconfigure()
{
    m_driver.select();
    m_driver.transfer(m_set_mode, sizeof(m_set_mode));
    m_driver.unselect();

    if(m_rumble)
    {
        m_driver.select();
        m_driver.transfer(m_enable_rumble, sizeof(m_enable_rumble));
        m_driver.unselect();
    }

    if(m_pressures)
    {
        m_driver.select();
        m_driver.transfer(m_set_bytes_large, sizeof(m_set_bytes_large));
        m_driver.unselect();
    }
    m_driver.select();
    m_driver.transfer(m_exit_cfg, sizeof(m_exit_cfg));
    m_driver.unselect();
}

void ps2_gamepad::dumpBuffer(Print &p)
{
    for(unsigned i = 0; i < 21; i++)
    {
        p.print(m_buffer[i], HEX);
        p.print(" ");
    }
}
