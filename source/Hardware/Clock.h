#pragma once
#include "Boot/IDT.h"
#define Seconds 10000000
namespace Hardware
{
    namespace Clock
    {
        bool Initialize();
        unsigned long long GetTime();
        void SleepUntil(unsigned long long time);
        void Sleep(unsigned long long time);

        void TimerHandler(Registers* registers);
		void RTCHandler(Registers* registers);
    }
}