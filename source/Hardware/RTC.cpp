#include "Hardware/RTC.h"
#include "Hardware/Hardware.h"

namespace Hardware
{
	namespace RTC
	{

		bool Initialize()
		{
			//Disable NMI + get register B
			Outb(0x70, 0x8B);

			unsigned char previous = Inb(0x71);
			Outb(0x70, 0x8B);
			Outb(0x71, previous|0x40);

			//Enable NMI
			Outb(0x70, Inb(0x70) & 0x7F);
			return true;
		}
	}
}