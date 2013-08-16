#include "Hardware/PIC.h"
#include "Hardware/Hardware.h"
namespace Hardware
{
    namespace PIC
    {

        bool Initialize()
        {
            return true;
        }
        
        void SetFrequency(int hz)
        {
            int divisor = 1193180 / hz;
            Hardware::Outb(0x43, 0x36);
            Hardware::Outb(0x40, divisor & 0xFF);
            Hardware::Outb(0x40, divisor >> 8);
        }
    }
}