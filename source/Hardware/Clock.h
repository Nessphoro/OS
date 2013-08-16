#pragma once
#include "Boot/IDT.h"

namespace Hardware
{
    namespace Clock
    {
        bool Initialize();
        unsigned int GetTime();
        void SleepUntil(unsigned int time);
        void Sleep(unsigned int time);

        void TimerHandler(Registers* registers);
    }
}