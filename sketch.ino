//#define ENABLE_SLOW_DIGITAL_READ
#include "software_spi_driver.h"
#include "ps_gamepad.h"

#ifdef ENABLE_PS2X_SUPPORT
hand::ps2x_gamepad gamepad(12, 11, 13, 10);
#else
hand::software_spi_driver driver(5, 6, 7, 4);
hand::ps2_gamepad gamepad(driver, false, false);
#endif


void setup()
{
    Serial.begin(9600);
    Serial.println("Init");
    delay(300);

    auto result = gamepad.configure();
    //interrupts();
    Serial.print("CR: ");
    Serial.println((int)result);

}
void loop()
{
    noInterrupts();
    gamepad.update(0,0);
    interrupts();
}
