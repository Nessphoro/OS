#include "Hardware/Clock.h"
#include "Hardware/PIC.h"
#include "Graphics/Graphics.h"
#include "Tasking/Tasking.h"
#include "Boot/IDT.h"

namespace Hardware
{
    void Outb(unsigned short port,unsigned char data)
    {
        __asm volatile( "outb %0, %1" : : "a"(data), "Nd"(port) );
    }
    namespace Clock
    {
        volatile unsigned int Ticks=0;

        bool Initialize()
        {
            IDT::EventHandlers[32]+=(void(*)(void*))TimerHandler;
            PIC::SetFrequency(1000);
            return true;
        }

        void SleepUntil(unsigned int time)
        {
            while (Ticks<time)
            {
                __asm volatile("hlt");
            }
        }
        void Sleep(unsigned int time)
        {
            unsigned int Until=GetTime()+time;
            SleepUntil(Until);
        }

        unsigned int GetTime()
        {
            return Ticks;
        }

        void TimerHandler(Registers* registers)
        {
            Ticks++;
			if(Tasking::TaskingEnabled)
            {
			
				if(--Tasking::GetRunning()->TimeLeft==0)
                    Tasking::Switch();
            }
            Graphics::RefreshConsole();
        }
    }
}