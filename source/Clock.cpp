#include "Hardware/Clock.h"
#include "Hardware/PIC.h"
#include "Hardware/RTC.h"
#include "Boot/IDT.h"
#include "Graphics/Graphics.h"
#include "Tasking/Tasking.h"
#include "Boot/IDT.h"

namespace Hardware
{
    void Outb(unsigned short port,unsigned char data)
    {
        __asm volatile( "outb %0, %1" : : "a"(data), "Nd"(port) );
    }
	unsigned char Inb(unsigned short port)
	{
		unsigned char ret;
		__asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
		return ret;
	}
    namespace Clock
    {
		volatile int driftFix=0;
        volatile unsigned long long Ticks=0;

        bool Initialize()
        {
            IDT::EventHandlers[32]+=(void(*)(void*))TimerHandler;
			IDT::EventHandlers[40] += (void(*)(void*) )RTCHandler;
            PIC::SetFrequency(18);
            return true;
        }

        void SleepUntil(unsigned long long time)
        {
            while (Ticks<time)
            {
                __asm volatile("hlt");
            }
        }
        void Sleep(unsigned long long time)
        {
            unsigned long long Until=GetTime()+time;
            SleepUntil(Until);
        }

        unsigned long long GetTime()
        {
            return Ticks;
        }

        void TimerHandler(Registers* registers)
        {

        }

		void RTCHandler(Registers* registers)
		{
			Outb(0x70, 0x0C);
			Inb(0x71);

			Ticks+=9765;
			driftFix++;
			if (driftFix == 8)
			{
				Ticks += 5;
				driftFix = 0;
			}
				

			if (Tasking::TaskingEnabled)
			{

				if (--Tasking::GetRunning()->TimeLeft == 0)
					Tasking::Switch();
			}
			Graphics::RefreshConsole();
		}
    }
}