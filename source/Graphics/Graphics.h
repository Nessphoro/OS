#pragma once
#include "Graphics/Console.h"

namespace Graphics
{
    extern void Initialise_NoGraphics();
    extern void SetConsole(Console* console);
    extern Console* GetConsole();
    extern void RefreshConsole();
    extern void ForceRefresh();
}